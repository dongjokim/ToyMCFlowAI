//===========================================================
// JHistos.h
//===========================================================

#ifndef JHISTOS_H
#define JHISTOS_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TFormula.h>
#include "toyflowinputs.h"

const int NPhiHist = 12; // Number of events for event-by-event phi dist.

class JHistos {

	public:
		JHistos(); 
		virtual ~JHistos(){;}	  //destructor

		// ALICE methods =====================================================
		void CreateQAHistos();
		void CreateFlowHistos();

	public:
		char  hname[140], htit[140];
		float b1, b2, pb1, pb2;
		TH1D *fhPt;
		TH1D *hCentSample; //sampling centrality
		TH1D *hSample; //sampling fNUE histo
		//one for each harmonic
		TH1D *hPhiPsi[NH][NC]; //phi-psi_n (symmetry plane)
		TH1D *hPhiPsiQ[NH][NC]; // Q vector (event plane)
		TH1D *hEventPlane[NH][NC]; //single particle delta phi hist phi-psi_n (symmetry plane)
		TH1D *hEventPlaneEP[NH][NC]; // single particle delta phi hist Q vector (event plane)
		TH1D *h2PCumulantVn[NH][NC]; // cumulant method
		TH1D *hTPcosDeltaPhi[NH][NC]; // two particle delta phi ( phi_i-phi_j)
		TH1D *hResolution[NH][NC];
		TH1D *hResolutionDist[NH][NC];
		TH1D *hResolutionDistA[NH][NC];
		//event-by-event
		TH1D *hPhiEvent[NPhiHist]; // Event-by-event phi 
		TH1D *hBGEvent[NPhiHist]; // Event-by-event background 
		TH1D *hTruthEvent[NPhiHist]; // Event-by-event truth 
		//phi-distibution histos
		TH1D *hBgPhi; 
		TH1D *hSignalPhi;
		TH1D *hInclusivePhi;
		TH1D *hDeltaPhiSum[NC];

	protected:

};

#endif

