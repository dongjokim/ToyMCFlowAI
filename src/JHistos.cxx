#include <TVector3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TFormula.h>


#include  "JHistos.h"


//______________________________________________________________________________
JHistos::JHistos(){   // constructor

}

//__________________________________________________________________________________________________
void JHistos::CreateQAHistos() {
	const int nBins =  74 ;
	double ptt[nBins] = {0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
		1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 60, 70, 80, 90, 100};
	fhPt = new TH1D("hPt", "pT spectra", nBins-1, ptt );
	fhPt->Sumw2();
}

void JHistos::CreateFlowHistos() {

	hCentSample = new TH1D("hCentSample","hCentSample",3,-0.1,2.1); //sampling centrality
	hSample = new TH1D("hSample","hSample",200, 0.0, 2.0*TMath::Pi()); //sampling fNUE histo
	//phi-distibution histos
	hBgPhi = new TH1D("hBgPhi","hBgPhi",200, 0.0, 2.0*TMath::Pi()); 
	hSignalPhi = new TH1D("hSignalPhi","hSignalPhi",200, 0.0, 2.0*TMath::Pi());
	hInclusivePhi = new TH1D("hInclusivePhi","hInclusivePhi",200, 0.0, 2.0*TMath::Pi());
	

	for (Int_t ih=0; ih<NH; ih++){ //harmonic loop
		for (Int_t ic=0; ic<NC; ic++){
			//-----Histograms---------
			hEventPlane[ih][ic]     = new TH1D(Form("hEventPlaneC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-1.0, 1.0);
			hEventPlaneEP[ih][ic]   = new TH1D(Form("hEventPlaneEPC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-1.0, 1.0);
			hTPcosDeltaPhi[ih][ic]    = new TH1D(Form("hTPcosDeltaPhiC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-1.0, 1.0);
			h2PCumulantVn[ih][ic]    = new TH1D(Form("h2PCumulantVnC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-1.0, 1.0);
			hPhiPsi[ih][ic]         = new TH1D(Form("hPhiPsiC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,0.0, 2.0*TMath::Pi());
			hPhiPsiQ[ih][ic]        = new TH1D(Form("hPhiPsiQC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,0.0, 2.0*TMath::Pi());
			hResolution[ih][ic]     = new TH1D(Form("hResolutionC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-100, 100);
			hResolutionDist[ih][ic] = new TH1D(Form("hResolutionDistC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-10, 10);
			hResolutionDistA[ih][ic]= new TH1D(Form("hResolutionDistAC%02dH%02d",ic,ih+1),Form("n=%d,%s",ih+1,strCentrality[ic].Data()),200,-10, 10);
		}
	}
	for (Int_t ic=0; ic<NC; ic++){
		hDeltaPhiSum[ic] = new TH1D(Form("hDeltaPhiSum_C%02d",ic),Form("%s",strCentrality[ic].Data()),200, 0.0, 2.0*TMath::Pi());
	}
	//event-by-event
	for (Int_t iPhiEvt=0; iPhiEvt<NPhiHist; iPhiEvt++){
		hPhiEvent[iPhiEvt] = new TH1D(Form("hPhiEvent_E%02d",(iPhiEvt+1)),Form("Event=%02d",(iPhiEvt+1)),100,0.0, 2.0*TMath::Pi());
		hBGEvent[iPhiEvt] 	 = new TH1D(Form("hBGEvent_E%02d",(iPhiEvt+1)),Form("Event=%02d",(iPhiEvt+1)),100,0.0, 2.0*TMath::Pi());
		hTruthEvent[iPhiEvt] = new TH1D(Form("hTruthEvent_E%02d",(iPhiEvt+1)),Form("Event=%02d",(iPhiEvt+1)),100,0.0, 2.0*TMath::Pi());
	}
//--------------------------End of histogram ----------------------------
}