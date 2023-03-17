#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TString.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <TSystem.h> 
#include "src/JBaseEventHeader.h"
#include "src/JBaseTrack.h"
#include "src/JHistos.h"
#include "src/JFlowAnalysis.h"
#include "src/JTreeGenerator.h"
#include "src/JPDF.h"

const Double_t kEtaMax = 0.8;
bool b2holes = 1; //1=two holes, 0=1 hole
Int_t GetCentralityBin( Double_t dice);
void NormalizeSample(TH1D *hist);

int main(int argc, char **argv)
{
TROOT root("flow","run mc");
if ( argc<3 ) {
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"+  "<<argv[0]<<" <fOutRootFile> <Nevt> <random seed>  <bNUE>"<<endl;
	cout << endl << endl;
	exit(1);
}
// Arguments 
char *outFile = argv[1];
Int_t Nevt= atoi(argv[2]);
Int_t random_seed = atoi(argv[3]);
Int_t bNUE = atoi(argv[4]); //if bNUE = false = 0, uses the uniform acceptance for sampling
// Declare variables
cout<< strCentrality[0]<<endl;

// pizero spectra from pythia event generator->Levy Para
Double_t lpt = 0.15, upt = 70;
Double_t par_Lpi14tev_pythia[3] = {2.62912e+04, 6.17808e+00, 5.26791e+00};
TF1 *Lpi14tev_pythia = new TF1("Lpi14tev_pythia","[0]*exp(-[2]/x)/pow(x,[1])",lpt, upt);
for(int i=0;i<3;i++) Lpi14tev_pythia->SetParameter(i,par_Lpi14tev_pythia[i]);

// Tree generator
// Open the output file
TFile *fOutRoot = new TFile(outFile, "RECREATE");

JTreeGenerator *jtreegen = new JTreeGenerator(fOutRoot);
jtreegen->GenerateTree();

JFlowInput *jinput = new JFlowInput();
jinput->LoadAliceData();
JPDF *jpdf = new JPDF(jinput);
jpdf->CreatePDF(); 

JHistos *jhisto = new JHistos();
jhisto->CreateQAHistos();
jhisto->CreateFlowHistos();

JFlowAnalysis *jflowana = new JFlowAnalysis();
jflowana->RegisterHistos(jhisto); // connect jflowana to jhisto with pointer
//-----------------------------Generating pdfs--------------------------------------
//Making acceptance function fNUE
TString NUEFormula = "[0]*(1-(x > 1.65)*(x < 2.2)*0.5";
if(b2holes)NUEFormula+="-(x > 0.3)*(x < 0.4)*0.7";//background with two gaps in phi
NUEFormula+=")";
if(!bNUE) NUEFormula="[0]";
TF1 *fNUE = new TF1("fNUE",NUEFormula,0.0,2.0*TMath::Pi());
fNUE->SetParameter(0,1.0);

//-------Random number needed for sampling
TRandom3 *prng = new TRandom3(random_seed);
gRandom->SetSeed(2*random_seed); //used for GetRandom()
//---------------------------End of generating pdfs---------------------------------
int ieout = Nevt/20;
if (ieout<1) ieout=1;

TStopwatch timer;
timer.Start();

const double mass = 0.139;
vector <Double_t> phiarray, phiarrayWeight; //pharray is now vector
Double_t v_n[NH] = {0.0,0.0,0.0};
//Eventloop to fill hSample
for (Int_t iEvent=0; iEvent<Nevt; iEvent++) {
	jtreegen->ClearEvent(); phiarray.clear(); phiarrayWeight.clear();
	if(iEvent % ieout == 0) { cout << iEvent << "\t" << int(float(iEvent)/Nevt*100) << "%" << endl ;}

	double cent = prng->Uniform(0.0,60.0);
	Int_t ic = GetCentralityBin(cent);
	if(ic < 0)
		continue;
	 
	jhisto->hCentSample->Fill(ic);
	UInt_t Nch=inputNch[ic];
	
	jpdf->GeneratePDF(prng,ic);
	TF1 *fourier = jpdf->GetPDF();
	
	// Event information for jflowana
	jflowana->SetCentBin(ic);
	jflowana->SetSymmetryPlanes(jpdf->GetSymmetryPlanes());
	
	//v_n = jpdf->GetVn();
	if(iEvent<NPhiHist)
		fourier->Write(Form("fourierE%02d",iEvent));

	jtreegen->AddEvent(random_seed, iEvent, cent, ic, Nch, jpdf->GetSymmetryPlanes(), v_n[1], v_n[2]);

	//Signal
	for (UInt_t t=0; t<Nch; t++){
		double phi = fourier->GetRandom();
		if(prng->Uniform(0,1) > fNUE->Eval(phi,0,0))//For a 1-d function give y=0 and z=0 
			continue;
		phiarray.push_back(phi); // generating signal
	}

	for (UInt_t t=0; t<phiarray.size(); t++){ 
		jhisto->hSample->Fill(phiarray[t]); //fill hSample with phiarray following shape of fNUE
		jhisto->hSignalPhi->Fill(phiarray[t]); 
		if(iEvent<NPhiHist){
			jhisto->hTruthEvent[iEvent]->Fill(phiarray[t]);
			jhisto->hPhiEvent[iEvent]->Fill(phiarray[t]);
		}
		jhisto->hInclusivePhi->Fill(phiarray[t]);
		double eta  = prng->Uniform(-kEtaMax, kEtaMax);//generate bg flat in eta
		double pt = Lpi14tev_pythia->GetRandom();
		double px = pt*cos(phiarray[t]);
		double py = pt*sin(phiarray[t]);
		double pz = pt*sinh(eta);
		double E = sqrt(mass*mass+px*px+py*py+pz*pz);
		jtreegen->AddTrack(t,pz,py,pz,E,phiarray[t],eta,pt,mass);
		
	}
	jflowana->Run(phiarray, phiarrayWeight);
	jtreegen->FillTree(); // fill event
	
}// End of event loop

NormalizeSample(jhisto->hSample); 
fNUE->Write("fNUE");
jtreegen->WriteTree();
delete prng;
delete fNUE;
delete jflowana;
delete jhisto;
delete Lpi14tev_pythia;
delete jtreegen;
delete fOutRoot;
cout <<"Successfully finished."<<endl;
timer.Print();

return 0;
}

Int_t GetCentralityBin(Double_t cent) {
for(UInt_t i = 0; i < NC; ++i)
	if(cent < centBins[i])
		return i;
return -1;
}

//depending on the shape of the histo, this may need to be changed
void NormalizeSample(TH1D *hist)
{
Int_t binxmin = hist->GetXaxis()->FindBin(TMath::Pi());	
Int_t binxmax = hist->GetNbinsX();
Double_t yav =  2.0*hist->Integral(binxmin,binxmax)/binxmax;
hist->Scale(1.0/yav);
return;
}