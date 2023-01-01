// $Id: JFlowAnalysis.cxx,v 1.0 2022/12/22 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JFlowAnalysis.cxx
  \brief
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 1.0 2022/12/22 $
 */
////////////////////////////////////////////////////


#include "JFlowAnalysis.h"

bool calculateTwoP = 1; //1=use 2p method (note: this is a lot slower than the others)

bool useSampleHisto = 0; //0=use fNUE function, 1=use phi histogram for evaluating the correction terms
using namespace std;
enum{kK0, kK1, kK2, nKL}; // order
TComplex QvectorQC[NH][nKL];


//______________________________________________________________________________
JFlowAnalysis::JFlowAnalysis():
	fTrackList(NULL), 
	fphiList(0),
	fphiListWeight(0)
{

}

//______________________________________________________________________________
JFlowAnalysis::~JFlowAnalysis(){
}

//______________________________________________________________________________


//______________________________________________________________________________
void JFlowAnalysis::RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill) { 

	int noIn    = fTrackList->GetEntriesFast();
	int counter = 0;
	for(int ii=0;ii<noIn;ii++){ // loop for all tracks 
		JBaseTrack *cgl = (JBaseTrack*)fTrackList->At(ii);
		if(1
		  ) {
			cgl->SetID(ii);
			new ((*listToFill)[counter++]) JBaseTrack(*cgl);
		}
	}
}

//______________________________________________________________________________
void JFlowAnalysis::Run(vector <double> phiarray, vector <double> phiarrayWeight){
		//Initializing 
		Double_t Qn_x[NH] = {0.0};//Should be 0 since we sum the qvectors
		Double_t Qn_y[NH] = {0.0};//
		TComplex QvectorsEP[NH];
		Double_t Psi_n_EP[NH]={0.0};
		Double_t AngleDiff[NH]={0.0};
		for(int iH=0;iH<NH;iH++) QvectorsEP[iH] = TComplex(0,0);
		//Declareing correction NUE factor
		Double_t corrNUEi= 1.; Double_t corrNUEj= 1.;// i is used for single particle
	  Int_t N_tot = phiarray.size();
		for (Int_t t=0; t<N_tot; t++)// start track loop 1
		{
			if (useSampleHisto){
				corrNUEi = 1./jhisto->hSample->GetBinContent(phiarray[t]);
				if(corrNUEi > 100) corrNUEi = 1.;
				if(corrNUEi < 0.001) corrNUEi = 1.;
				//cout << "corrNUEi = " << corrNUEi<< "at phi = " << phiarray[t] << endl;
			}
			fphiListWeight.push_back(corrNUEi);
			//Harmonic loop
			for (Int_t n=0; n<NH; n++)
			{
				// Analytic Event plane method
				jhisto->hPhiPsi[n][ic]->Fill(DeltaPhi(phiarray[t],Psi_n[n]));
				jhisto->hEventPlane[n][ic]->Fill(corrNUEi*TMath::Cos((n+1)*(DeltaPhi(phiarray[t], Psi_n[n]))));
				
				// calculating eventplane with Q-vectors
				Qn_x[n] += corrNUEi*TMath::Cos((n+1)*phiarray[t]);
				Qn_y[n]+= corrNUEi*TMath::Sin((n+1)*phiarray[t]);
				QvectorsEP[n] += TComplex(corrNUEi*TMath::Cos((n+1)*phiarray[t]),corrNUEi*TMath::Sin((n+1)*phiarray[t]));
			}
		}//End of track loop 1
		//This only after the track loop, must sum over the tracks first
		for (Int_t n=0; n<NH; n++) {
			Psi_n_EP[n]=(1/double(n+1))*TMath::ATan2(Qn_y[n],Qn_x[n]);
		}
		//Resolution for every event
		for (Int_t n=0; n<NH; n++)
		{
			AngleDiff[n] = TMath::Cos((n+1)*(DeltaPhi(Psi_n[n], Psi_n_EP[n]))); //Analystical resultion
			//if(n==1) cout <<  Form("n=%d,psi=%.3f, %.3f, %.3f",n,Psi_n[n], Psi_n_EP[n], Psi_n_EPQ[n]) << endl;
			jhisto->hResolution[n][ic]->Fill(AngleDiff[n]);
			jhisto->hResolutionDist[n][ic]->Fill(DeltaPhi(Psi_n[n], Psi_n_EP[n]));
			jhisto->hResolutionDistA[n][ic]->Fill(Psi_n[n]-Psi_n_EP[n]);
		}
		//End EP
		// Start EP and two-particle correlation
		double correlation[NH][NC] = {{0.0}};
		double weightsProdSum[NH][NC] = {{0.0}};
		for (Int_t i=0; i<N_tot; i++){ //start track loop 2
			if (useSampleHisto){
				corrNUEi = 1./jhisto->hSample->GetBinContent(phiarray[i]); 
			}
			//Event plane method calculated vn
			for (Int_t n=0; n<NH; n++) {
				// Q-vector calculated Event plane method using Q-vectors
				jhisto->hEventPlaneEP[n][ic]->Fill(TMath::Cos((n+1)*(DeltaPhi(phiarray[i], Psi_n_EP[n])))); 
				jhisto->hPhiPsiQ[n][ic]->Fill(DeltaPhi(phiarray[i], Psi_n_EP[n]));
			}
			//2 particle correlation method, if option on
			if (!calculateTwoP){continue;}
			for (Int_t j=0; j<N_tot;j++){
				if(i==j) continue;
				if (useSampleHisto){
					corrNUEj = 1./jhisto->hSample->GetBinContent(phiarray[j]);
				}
				jhisto->hDeltaPhiSum[ic]->Fill(DeltaPhi(phiarray[i], phiarray[j]));//For fitting
				for (Int_t n=0; n<NH; n++){
					correlation[n][ic] += corrNUEi*corrNUEj*TMath::Cos((n+1)*(DeltaPhi(phiarray[i], phiarray[j])));
					weightsProdSum[n][ic] += corrNUEi*corrNUEj;
					//hTPcosDeltaPhi[n][ic]->Fill(corrNUEi*corrNUEj*TMath::Cos((n+1)*(DeltaPhi(phiarray[i], phiarray[j]))));
				}
			}
		} // end of track loop 2
		//Start 2P cumulant
		CalculateQvectors(phiarray,fphiListWeight);
		for( int n=1; n<NH+1; n++){
			TComplex sctwo = Two(n, -(n)) / Two(0,0).Re();
			//cout << "sctwo" << sctwo.Re() << endl;
			jhisto->h2PCumulantVn[n-1][ic]->Fill(sctwo.Re());
		}
		for (Int_t n=0; n<NH; n++)
		{
			correlation[n][ic] /= weightsProdSum[n][ic];
			jhisto->hTPcosDeltaPhi[n][ic]->Fill(correlation[n][ic]);
		}
}

//---------Member function---------
double JFlowAnalysis::DeltaPhi(double phi1, double phi2) {
	// dphi
	double res =  atan2(sin(phi1-phi2), cos(phi1-phi2));
	return res>0 ? res : 2.*TMath::Pi()+res ;
}

void JFlowAnalysis::CalculateQvectors(vector <double> phiarray, vector <double> phiweight){
	//initiate qvectors as complex zeroes
	for(int n=0; n<NH; n++){
		for(int ik=0; ik<nKL; ++ik){
			QvectorQC[n][ik] = TComplex(0,0);
		}
	} 

	Int_t N_tot = phiarray.size();
	for (int t=0; t<N_tot; t++){
		for(int n=0; n<NH+1; n++){
			Double_t tf = 1.0;
			TComplex q[nKL];
			for(int ik=0; ik<nKL; ik++){
				q[ik] = TComplex(tf*TMath::Cos((n)*phiarray[t]),tf*TMath::Sin((n)*phiarray[t]));
				QvectorQC[n][ik] += q[ik];
				tf *= phiweight[t];
			}
		}
	}
}

TComplex JFlowAnalysis::Q(int n, int p){
	// Return QvectorQC
	// Q{-n, p} = Q{n, p}*
	if(n >= 0)
		return QvectorQC[n][p];
	return TComplex::Conjugate(QvectorQC[-n][p]);
}

TComplex JFlowAnalysis::Two(int n1, int n2 ){
	// two-particle correlation <exp[i(n1*phi1 + n2*phi2)]>
	//	cout << "TWO FUNCTION " << Q(n1,1) << "*" << Q(n2,1) << " - " << Q(n1+n2 , 2) << endl;
	TComplex two = Q(n1, 1) * Q(n2, 1) - Q( n1+n2, 2);
	return two;
}

