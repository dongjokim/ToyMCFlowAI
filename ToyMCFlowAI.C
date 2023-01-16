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
//#include "include/toyflowinputs.h"
#include <TSystem.h> 
#include "src/JBaseEventHeader.h"
#include "src/JBaseTrack.h"
#include "src/JHistos.h"
#include "src/JFlowAnalysis.h"

const Double_t kEtaMax = 0.8;
void NormalizeSample(TH1D *hist); //to normalize the inclusive phi histo to match fNUE
bool b2holes = 1; //1=two holes, 0=1 hole
Int_t GetCentralityBin( Double_t dice);

int main(int argc, char **argv)
{
	TROOT root("flow","run mc");
	if ( argc<4 ) {
		cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"+  "<<argv[0]<<" <fOutRootFile> <Nevt> <random seed> <bgfrac> <bNUE>"<<endl;
		cout << endl << endl;
		exit(1);
	}
    // Arguments 
	char *outFile = argv[1];
	Int_t Nevt= atoi(argv[2]);
	Int_t random_seed = atoi(argv[3]);
	Double_t bgfrac = atof(argv[4]);
	Int_t bNUE = atoi(argv[5]); //if bNUE = false = 0, uses the uniform acceptance for sampling
	// Declare variables
	cout<< strCentrality[0]<<endl;
	TFile *fOutRoot = new TFile(outFile, "RECREATE");
	TClonesArray *event = new TClonesArray("JBaseEventHeader",1000);
	TClonesArray *tracks = new TClonesArray("JBaseTrack",1000);
    TTree *jTree = new TTree("jTree","Tree from ToyMC");
    jTree->Branch("JTrackList",&tracks);
    jTree->Branch("JEventHeaderList",&event);

    // Define some simple structures
    typedef struct {Double_t phi,eta,pt,mass,E,correction;} JTRACKS;
    typedef struct {
       Int_t runnumber,event,ntrack,icent;
       Double_t eCM,psi_2,psi_3,v_2,v_3;
    } JEVENT;
    JTRACKS jtracks;
    JEVENT jevent;
	TTree *vTree = new TTree("vTree","vTree from ToyMC");
	//vTree->Branch("jtracks",&jtracks,"phi/D:eta/D:pt/D:E/D:correction/D");
    //vTree->Branch("jevent",&jevent,"ntrack/I:icent/i:psi_2/D:psi_3/D");
    vTree->Branch("phi",&jtracks.phi,"phi/D");
    vTree->Branch("eta",&jtracks.eta,"eta/D");
    vTree->Branch("pt",&jtracks.pt,"pt/D");
    vTree->Branch("mass",&jtracks.mass,"mass/D");
    vTree->Branch("E",&jtracks.E,"E/D");
    vTree->Branch("correction",&jtracks.correction,"correction/D");
    vTree->Branch("runnumber",&jevent.event,"runnumber/I");
    vTree->Branch("event",&jevent.event,"event/I");
    vTree->Branch("ntrack",&jevent.ntrack,"ntrack/I");
    vTree->Branch("icent",&jevent.icent,"icent/I");
    vTree->Branch("eCM",&jevent.eCM,"eCM/D");
    vTree->Branch("psi_2",&jevent.psi_2,"psi_2/D");
    vTree->Branch("psi_3",&jevent.psi_3,"psi_3/D");
    vTree->Branch("v_2",&jevent.v_2,"v_2/D");
    vTree->Branch("v_3",&jevent.v_3,"v_3/D");
	//Define uniform function for sampling centrality
	TF1 *centSamp = new TF1("centSamp", "[0]",0.0,0.9);
	centSamp->SetParameter(0,1.0);
	TF1 *uniform[NH]; // uniform distribution of psi for each harmonic, range 0 to 2*pi
	for (Int_t ih=0; ih<NH; ih++){ //harmonic loop
		//------Symmetry planes random ------
		uniform[ih]= new TF1(Form("uniform%02d",ih+1),"[0]", -1*TMath::Pi()/(ih+1), 1.0*TMath::Pi()/(ih+1));
		uniform[ih]->SetParameter(0,1.);
	}
	// pizero spectra from pythia event generator->Levy Para
	Double_t lpt = 0.15, upt = 70;
	Double_t par_Lpi14tev_pythia[3] = {2.62912e+04, 6.17808e+00, 5.26791e+00};
	TF1 *Lpi14tev_pythia = new TF1("Lpi14tev_pythia","[0]*exp(-[2]/x)/pow(x,[1])",lpt, upt);
	for(int i=0;i<3;i++) Lpi14tev_pythia->SetParameter(i,par_Lpi14tev_pythia[i]);

	JHistos *jhisto = new JHistos();
	jhisto->CreateQAHistos();
	jhisto->CreateFlowHistos();

    JFlowAnalysis *jflowana = new JFlowAnalysis();
    jflowana->RegisterHistos(jhisto);
    //-----------------------------Generating pdfs--------------------------------------
	//signal
	TString strformula = "[0]*(1";
	for (Int_t ih=0; ih<NH; ih++){
		strformula += Form("+2*[%d]*TMath::Cos(%d*(x-[%d]))",ih+1,ih+1,NH+ih+1);
	}
	strformula+=")";
	cout<<strformula<<endl;
	TF1 *fourier = new TF1("Fourier", strformula, 0.0, 2.0*TMath::Pi());
	//background
	TF1 *bgUniform = new TF1("bgUniform","[0]",0.0, 2.0*TMath::Pi());
	bgUniform->SetParameter(0,1.0);

	//Making acceptance function fNUE
	TString NUEFormula = "[0]*(1-(x > 1.65)*(x < 2.2)*0.5";
	if(b2holes)NUEFormula+="-(x > 0.3)*(x < 0.4)*0.7";//background with two gaps in phi
	NUEFormula+=")";
	if(!bNUE) NUEFormula="[0]";
	TF1 *fNUE = new TF1("fNUE",NUEFormula,0.0,2.0*TMath::Pi());
	fNUE->SetParameter(0,1.0);

	//-------Random number needed for sampling
	TRandom3 *prng = new TRandom3(random_seed);
	gRandom->SetSeed(random_seed);
	//---------------------------End of generating pdfs-------------------------------------
	int ieout = Nevt/20;
	if (ieout<1) ieout=1;
	TStopwatch timer;
	timer.Start();
	Double_t Psi_n[NH]={0.0};// symmetry plane angles for each n
	vector <Double_t> phiarray, phiarrayWeight; //pharray is now vector
	Int_t ic=0;
	Double_t cent = -999; // for centrality
	Int_t Nch=0, N_bg=0;
	Double_t px=-1, py=-1, pz=-1, E=-1, mass = 0.139;
    Double_t phi = -999., eta = -999., pt  = -999.;
	//Eventloop to fill hSample
	for (Int_t iEvent=0; iEvent<Nevt; iEvent++) {
		event->Clear(); tracks->Clear(); phiarray.clear(); phiarrayWeight.clear();
		if(iEvent % ieout == 0) { cout << iEvent << "\t" << int(float(iEvent)/Nevt*100) << "%" << endl ;}
        JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
        hdr->SetEventID(iEvent);
		//--------Sample randomly from centSamp---------
		cent = centSamp->GetRandom(); hdr->SetCentrality(cent);
		ic = 0;//GetCentralityBin(cent);
    	//-----------End of random sampling of centrality-------------
		jhisto->hCentSample->Fill(ic);
		Nch=inputNch[ic];
		//Get Psi for different harmonics
		for (Int_t n=0; n<NH; n++) Psi_n[n]=uniform[n]->GetRandom();//harmonic loop
		// Event information for jflowana
		jflowana->SetCentBin(ic);
		jflowana->SetSymmetryPlanes(Psi_n);
		// Setting parameter values of pdf
		fourier->SetParameter(0,Nch); 
		//https://www.bnl.gov/isd/documents/88233.pdf
		double v2 = prng->Gaus(inputVn[1][ic],0.2*inputVn[1][ic]);
		double v3 = prng->Gaus(inputVn[2][ic],0.2*inputVn[2][ic]);
		/*for (Int_t i=0; i<NH-1; i++){
			fourier->SetParameter(i+1,inputVn[i][ic]); //Setting the vn parameters
		}*/
		fourier->SetParameter(1,v2);
		fourier->SetParameter(2,v3);
		for (Int_t i=NH; i<2*NH; i++){
			fourier->SetParameter(i+1,Psi_n[i-NH]); //Setting the Psi parameters
		}
		if(iEvent<NPhiHist){ fourier->Write(Form("fourierE%02d",iEvent));}
		//Signal
		for (Int_t t=0; t<Nch; t++){
			phi=fourier->GetRandom();
			if(prng->Uniform(0,1) > fNUE->Eval(phi,0,0))//For a 1-d function give y=0 and z=0 
				continue;
			phiarray.push_back(phi); // generating signal
		}
		//Background
		N_bg = Nch*bgfrac;
		for (Int_t t=0; t<N_bg; t++){ //generating background
			phi = bgUniform->GetRandom();
			if(prng->Uniform(0,1) > fNUE->Eval(phi,0,0))//For a 1-d function give y=0 and z=0 
				continue;
			phiarray.push_back(phi); // if bNUE'is kTRUE
			if(iEvent<NPhiHist){ jhisto->hBGEvent[iEvent]->Fill(phi); }
			jhisto->hBgPhi->Fill(phi);
		}
		// Signal + background
		jevent.runnumber = random_seed; // just for MC
		jevent.event = iEvent;
		jevent.ntrack = phiarray.size();
		jevent.icent  = ic;
		jevent.eCM  = TMath::Log(5020.);
		jevent.psi_2  = Psi_n[0];
		jevent.psi_3  = Psi_n[1];
		jevent.v_2  = v2;//inputVn[1][ic];
		jevent.v_3  = v3;//inputVn[2][ic];

		for (Int_t t=0; t<phiarray.size(); t++){ 
			jhisto->hSample->Fill(phiarray[t]); //fill hSample with phiarray following shape of fNUE
			jhisto->hSignalPhi->Fill(phiarray[t]); 
			if(iEvent<NPhiHist){
				jhisto->hTruthEvent[iEvent]->Fill(phiarray[t]);
				jhisto->hPhiEvent[iEvent]->Fill(phiarray[t]);
			}
			jhisto->hInclusivePhi->Fill(phiarray[t]);
			eta  = prng->Uniform(-kEtaMax, kEtaMax);//generate bg flat in eta
			pt = Lpi14tev_pythia->GetRandom();;
			px = pt*cos(phiarray[t]); py = pt*sin(phiarray[t]); pz = pt*sinh(eta);
			E = sqrt(mass*mass+px*px+py*py+pz*pz);
			new ( (*tracks)[t] )TLorentzVector(px, py, pz, E);
			jtracks.phi=phiarray[t];
			jtracks.eta = eta;
			jtracks.pt  = pt;
			jtracks.E   = E;
			jtracks.mass   = mass;
			jtracks.correction = 1.0;
			vTree->Fill();
		}
		//jflowana->Run(phiarray, phiarrayWeight);
		jTree->Fill(); // fill event
		
	}// End of event loop
	NormalizeSample(jhisto->hSample); 
	fNUE->Write("fNUE");
	fOutRoot->Write();
	fOutRoot->Close();
	cout <<"Successfully finished."<<endl;
	timer.Print();
}
//depending on the shape of the histo, this may need to be changed
void NormalizeSample(TH1D *hist)
{
		Int_t binxmin = hist->GetXaxis()->FindBin(TMath::Pi());	
		Int_t binxmax = hist->GetNbinsX();
		Double_t yav =  2.0*hist->Integral(binxmin,binxmax)/binxmax;
		hist->Scale(1/yav);
		return;
}
Int_t GetCentralityBin( Double_t dice) {
	Int_t ic = -1;
	if(dice>= 0.0 && dice<0.3) ic=0;
	if(dice>=0.3 && dice<0.6) ic=1;
	if(dice>=0.6 && dice<=0.9) ic=2;

	return ic;
}
