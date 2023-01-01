// $Id: JFlowAnalysis.h,v 1.8 2008/02/12 15:51:27 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JFlowAnalysis.h
  \brief
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@cc.jyu.fi
  \version $Revision: 1.8 $
  \date $Date: 2008/02/12 15:51:27 $
 */
////////////////////////////////////////////////////

#ifndef JFlowAnalysis_H
#define JFlowAnalysis_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TComplex.h>
#include <vector>
#include  "JBaseTrack.h"
#include "JHistos.h"

class JFlowAnalysis  {

	public:
		JFlowAnalysis();

		virtual ~JFlowAnalysis();		                    //destructor
		virtual void RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill);
		virtual void Run(vector <double> listToFill, vector <double> listFromToFill);

		// SETTER
		void RegisterHistos(JHistos *jh) {jhisto = jh;}
		void SetCentBin(int ib) {ic=ib;}
		void SetSymmetryPlanes(Double_t *psi) {for (Int_t n=0; n<NH; n++) Psi_n[n] = psi[n];}
		double DeltaPhi(double phi1, double phi2); // relative angle
		void CalculateQvectors(vector <double> phiarray, vector <double> phiweight);
		TComplex Q(int n, int p);
		TComplex Two(int n1, int n2 );

	protected:
		TClonesArray  *fTrackList;
		vector <double> fphiList;
		vector <double> fphiListWeight;
		JHistos *jhisto;
		int ic;
		Double_t Psi_n[NH];
};

#endif

