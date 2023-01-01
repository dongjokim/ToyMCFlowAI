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
#include <vector>
#include "../include/toyflowinputs.h"

using namespace std;

const int NPhiHist = 12; // Number of events for event-by-event phi dist.
//Input Histograms from ToyMCFlowAO.C
TH1D *hPhiPsi[NH][NC]; //phi-psi_n (symmetry plane)
TH1D *hPhiPsiQ[NH][NC]; // Q vector (event plane)
TH1D *hEventPlane[NH][NC]; //single particle delta phi hist phi-psi_n (symmetry plane)
TH1D *hEventPlaneEP[NH][NC]; // single particle delta phi hist Q vector (event plane)
TH1D *h2PCumulantVn[NH][NC]; // cumulant method
TH1D *hTPcosDeltaPhi[NH][NC]; // two particle delta phi ( phi_i-phi_j)
TH1D *hPhiEvent[NPhiHist][NC]; // Event-by-event phi 
TH1D *hResolution[NH][NC];
TH1D *hResolutionDist[NH][NC];
TH1D *hResolutionDistA[NH][NC];

TH1D *hDeltaPhiSum[NC];
TH1D *hBGEvent[NPhiHist][NC]; // Event-by-event background 
TH1D *hTruthEvent[NPhiHist][NC]; // Event-by-event truth 

TH1D *hCentSample = new TH1D("hCentSample","hCentSample",3,-0.1,2.1); //sampling centrality
TH1D *hSample = new TH1D("hSample","hSample",200, 0.0, 2.0*TMath::Pi()); //sampling fNUE histo
//phi-distibution histos
TH1D *hBgPhi = new TH1D("hBgPhi","hBgPhi",200, 0.0, 2.0*TMath::Pi()); 
TH1D *hSignalPhi = new TH1D("hSignalPhi","hSignalPhi",200, 0.0, 2.0*TMath::Pi());
TH1D *hInclusivePhi = new TH1D("hInclusivePhi","hInclusivePhi",200, 0.0, 2.0*TMath::Pi());


//-----output-----
const int NMethod=4;
TGraphErrors *gr_vn_cent[NMethod][NH];
TGraphErrors *gr_pvn[NC][NMethod];
TGraphErrors *gr_vn_true[NH];
TString gr_Names[NMethod]={"SP","TP","EP","TPAz"}; //TP = cumulant 2p, TPaz = two part azimuthal correlation 
void LoadData(TString inputfile);
void Calculate();
void SaveToRootfile(TString outputfile);

// Main Function
void CalculateFlow(TString inputfilename,TString outputfilename){
	LoadData(inputfilename);
	Calculate();
	SaveToRootfile(outputfilename);
}


//---------------Member functions------------------
void LoadData(TString inputfilename){
	TFile *fIn = TFile::Open(inputfilename,"read");
	for(int ic=0; ic<NC; ic++){
		hDeltaPhiSum[ic]=(TH1D*)fIn->Get(Form("hDeltaPhiSum_C%02d",ic));
		hDeltaPhiSum[ic]->Rebin(2);
		for(int ih=0; ih<NH; ih++){
			hPhiPsi[ih][ic]=(TH1D*)fIn->Get(Form("hPhiPsiC%02dH%02d",ic,ih+1));
			hPhiPsiQ[ih][ic]=(TH1D*)fIn->Get(Form("hPhiPsiQC%02dH%02d",ic,ih+1));
			hEventPlane[ih][ic]=(TH1D*)fIn->Get(Form("hEventPlaneC%02dH%02d",ic,ih+1));
			hEventPlaneEP[ih][ic]=(TH1D*)fIn->Get(Form("hEventPlaneEPC%02dH%02d",ic,ih+1));
			hTPcosDeltaPhi[ih][ic]=(TH1D*)fIn->Get(Form("hTPcosDeltaPhiC%02dH%02d",ic,ih+1));
			h2PCumulantVn[ih][ic]=(TH1D*)fIn->Get(Form("h2PCumulantVnC%02dH%02d",ic,ih+1));
			//h2PCumulantVn[ih][ic]=(TH1D*)fIn->Get(Form("hTPcosDeltaPhiC%02dH%02d",ic,ih+1)); //comment this in if no TP cumulant histo available
			hResolution[ih][ic]=(TH1D*)fIn->Get(Form("hResolutionC%02dH%02d",ic,ih+1));
			hResolutionDist[ih][ic]=(TH1D*)fIn->Get(Form("hResolutionDistC%02dH%02d",ic,ih+1));
			hResolutionDistA[ih][ic]=(TH1D*)fIn->Get(Form("hResolutionDistAC%02dH%02d",ic,ih+1));
		}

		for(int ie=0; ie<NPhiHist; ie++){
			hPhiEvent[ie][ic]	= (TH1D*)fIn->Get(Form("hPhiEvent_C%02d_E%02d",ic,(ie+1)));
			hBGEvent[ie][ic] 	= (TH1D*)fIn->Get(Form("hBGEvent_C%02d_E%02d",ic,(ie+1)));
			hTruthEvent[ie][ic] = (TH1D*)fIn->Get(Form("hTruthEvent_C%02d_E%02d",ic,(ie+1)));
		}
	}
	hCentSample = (TH1D*)fIn->Get("hCentSample"); 
	hSample = (TH1D*)fIn->Get("hSample");
	hBgPhi = (TH1D*)fIn->Get("hBgPhi"); 
	hSignalPhi = (TH1D*)fIn->Get("hSignalPhi");
	hInclusivePhi = (TH1D*)fIn->Get("hInclusivePhi");

}

void Calculate(){
	Double_t MeanArrayTwoParticle[NH][NC]={{0.}}; //cumulant 2p
	Double_t MeanArrayTwoPartError[NH][NC]={{0.}};
	Double_t MeanArrayTPTwoParticle[NH][NC]={{0.}}; //azim. 2p
	Double_t MeanArrayTPTwoPartError[NH][NC]={{0.}};
	Double_t MeanArrayEventPlane[NH][NC]={{0.}};
	Double_t MeanArrayEvtPlError[NH][NC]={{0.}};
	Double_t MeanArrayEventPlaneQVec[NH][NC]={{0.}};
	Double_t MeanArrayEvtPlErrorQvec[NH][NC]={{0.}};
	Double_t MeanArrayResolution[NH][NC]={{0.}};
	Double_t MeanArrayResolutionError[NH][NC]={{0.}};
	Double_t vn_obs_ERROR[NH][NC]={{0.}};
	Double_t vn_TwoPart[NH][NC]={{0.}};	//cumulant 2p
	Double_t vn_TwoPartError[NH][NC]={{0.}};
	Double_t vn_TPTwoPart[NH][NC]={{0.}}; //azim. 2p
	Double_t vn_TPTwoPartError[NH][NC]={{0.}};
	Double_t vn_EvtPl[NH][NC]={{0.}};
	Double_t vn_EvtPlQvec[NH][NC]={{0.}};

	// Calculating the avarage over event
	for (Int_t n=0; n<NH; n++)
	{
		for (Int_t ic=0; ic<NC; ic++){
			MeanArrayTwoParticle[n][ic]=h2PCumulantVn[n][ic]->GetMean();
			MeanArrayTwoPartError[n][ic]=h2PCumulantVn[n][ic]->GetMeanError();
			MeanArrayTPTwoParticle[n][ic]=hTPcosDeltaPhi[n][ic]->GetMean();
			cout<< MeanArrayTPTwoParticle[n][ic]<<endl;
			MeanArrayTPTwoPartError[n][ic]=hTPcosDeltaPhi[n][ic]->GetMeanError();
			MeanArrayEventPlane[n][ic]=hEventPlane[n][ic]->GetMean();
			MeanArrayEvtPlError[n][ic]=hEventPlane[n][ic]->GetMeanError();
			MeanArrayEventPlaneQVec[n][ic]=hEventPlaneEP[n][ic]->GetMean();
			MeanArrayEvtPlErrorQvec[n][ic]=hEventPlaneEP[n][ic]->GetMeanError();
			MeanArrayResolution[n][ic]=hResolution[n][ic]->GetMean();
			MeanArrayResolutionError[n][ic]=hResolution[n][ic]->GetMeanError();

			//for loop for swapping  vn arrays for vn[ic][n] !transformation!

			vn_TPTwoPart[n][ic]=TMath::Sqrt(TMath::Abs(MeanArrayTPTwoParticle[n][ic]));
			//cout<< "calculating loop: "<< vn_TPTwoPart[n][ic]<< endl;
			vn_TPTwoPartError[n][ic]=0.5*(1.0/(TMath::Power(MeanArrayTPTwoParticle[n][ic],0.5))*MeanArrayTPTwoPartError[n][ic]);
			vn_TwoPart[n][ic]=TMath::Sqrt(TMath::Abs(MeanArrayTwoParticle[n][ic]));
			vn_TwoPartError[n][ic]=0.5*(1.0/(TMath::Power(MeanArrayTwoParticle[n][ic],0.5))*MeanArrayTwoPartError[n][ic]);
			vn_EvtPl[n][ic]=MeanArrayEventPlane[n][ic];
			vn_EvtPlQvec[n][ic]=MeanArrayEventPlaneQVec[n][ic]/MeanArrayResolution[n][ic];
			vn_obs_ERROR[n][ic]=TMath::Abs(vn_EvtPlQvec[n][ic])*TMath::Sqrt(TMath::Power(MeanArrayEvtPlErrorQvec[n][ic]/MeanArrayEventPlaneQVec[n][ic],2)+TMath::Power(MeanArrayResolutionError[n][ic]/MeanArrayResolution[n][ic],2));	
		}

	}
	// For Power spectra
	Double_t pvn_obs_ERROR[NC][NH]={{0.}};
	Double_t pvn_TwoPart[NC][NH]={{0.}};
	Double_t pvn_TwoPartError[NC][NH]={{0.}};
	Double_t pvn_TPTwoPart[NC][NH]={{0.}};
	Double_t pvn_TPTwoPartError[NC][NH]={{0.}};
	Double_t pvn_EvtPl[NC][NH]={{0.}};
	Double_t pvn_EvtPlQvec[NC][NH]={{0.}};
	Double_t pMeanArrayEvtPlError[NC][NH]={{0.}};

	for (int ic=0; ic<NC; ic++){
		for (int ih=0; ih<NH; ih++){
			pvn_obs_ERROR[ic][ih]        = vn_obs_ERROR[ih][ic];
			pvn_TwoPart[ic][ih]          = vn_TwoPart[ih][ic];
			pvn_TwoPartError[ic][ih]     = vn_TwoPartError[ih][ic];
			pvn_TPTwoPart[ic][ih]        = vn_TPTwoPart[ih][ic];
			pvn_TPTwoPartError[ic][ih]   = vn_TPTwoPartError[ih][ic];
			pvn_EvtPl[ic][ih]            = vn_EvtPl[ih][ic];
			pvn_EvtPlQvec[ic][ih]        = vn_EvtPlQvec[ih][ic];
			pMeanArrayEvtPlError[ic][ih] = pMeanArrayEvtPlError[ih][ic];
			cout << vn_TPTwoPart[ih][ic]<< endl;
		}
	}

	//Fill graphs I drew in the sktech
	double Cent[NC] = {0.,1.,2.};
	double eCent[NC] = {0.,0.,0.};
	double evntrue[NC] = {0.,0.,0.};
	
	for (int ih=0; ih<NH; ih++) {
		gr_vn_true[ih]= new TGraphErrors(NC,Cent,inputVn[ih],eCent,evntrue); //add error here
	}

	for (int ih=0; ih<NH; ih++) {
		gr_vn_cent[0][ih]= new TGraphErrors(NC,Cent,vn_EvtPl[ih],eCent,MeanArrayEvtPlError[ih]); //add error here
		gr_vn_cent[1][ih]= new TGraphErrors(NC,Cent,vn_TwoPart[ih],eCent,vn_TwoPartError[ih]); //add error here
		gr_vn_cent[2][ih]= new TGraphErrors(NC,Cent,vn_EvtPlQvec[ih],eCent,vn_obs_ERROR[ih]); //add error here
		gr_vn_cent[3][ih]= new TGraphErrors(NC,Cent,vn_TPTwoPart[ih],eCent,vn_TPTwoPartError[ih]); //add error here
	}
	//Fill graphs I drew in the sktech
	double px[NH] = {0.};
	double pxe[NH] = {0.};
	for (int ih=0; ih<NH; ih++){px[ih]=ih;pxe[ih]=0.;}

	for (int ic=0; ic<NC; ic++) {
		gr_pvn[ic][0]= new TGraphErrors(NH,px,pvn_EvtPl[ic],pxe,pMeanArrayEvtPlError[ic]); //add error here
		gr_pvn[ic][1]= new TGraphErrors(NH,px,pvn_TwoPart[ic],pxe,pvn_TwoPartError[ic]); //add error here
		gr_pvn[ic][2]= new TGraphErrors(NH,px,pvn_EvtPlQvec[ic],pxe,pvn_obs_ERROR[ic]); //add error here
		gr_pvn[ic][3]= new TGraphErrors(NH,px,pvn_TPTwoPart[ic],pxe,pvn_TPTwoPartError[ic]); //add error here
	}
		for(int i=0; i<NMethod; i++){
			for (int ih=0; ih<NH; ih++){
				gr_vn_cent[i][ih]->SetTitle(Form("Centrality dependence %s Method", gr_Names[i].Data()));
				
			}
			for (int ic=0; ic<NC; ic++){
				gr_pvn[ic][i]->SetTitle(Form("n dependence %s Method", gr_Names[i].Data()));
			}
		}

}


void SaveToRootfile(TString outputfilename){
	TFile *output = new TFile(outputfilename, "recreate");
	output->cd();
	for(int i=0; i<NMethod; i++){
		for (int ih=0; ih<NH; ih++){
			gr_vn_cent[i][ih]->Write(Form("gr_v%02d_%s_cent",ih+1, gr_Names[i].Data()));
			gr_vn_true[ih]->Write(Form("gr_v%02d_true_cent",ih+1));
		}
		for (int ic=0; ic<NC; ic++){
			gr_pvn[ic][i]->Write(Form("gr_pv%02d_%s",ic+1, gr_Names[i].Data()));
		}
	}
	output->Write();
	output->Close();
}
