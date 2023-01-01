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
#include "../include/rootcommon.h"

using namespace std;

Int_t gMarkers[]= {20,24,21,25,22,26,23,27,32,28};
Int_t gColors[]={kRed+1, kOrange+2, kCyan+2, kSpring-6, kRed-7, kOrange+1,kCyan-6,kGreen+7,kRed-9,kOrange-9,kAzure+6,kGreen-9};
Int_t gStyles[]={1,2,3,4,5,6,7,8,9,10};

TH1D *hDeltaPhiSum[NC];
Double_t vn[NH][NC]={-999};
Double_t vnError[NH][NC]={-999};

void LoadData(TString);
void FitDrawTwo(int);
void SaveVns();

void FitTwoPart(TString infile="../output/toymcflowao_1d0eb42_10k.root")
{
	LoadData(infile);
	//Functions below should be in a for loop over NC
	for(Int_t ic=0; ic<NC; ic++) FitDrawTwo(ic);
	SaveVns();
}
//-------Member functions------------
void LoadData(TString inputname)
{
	TFile *fIn = TFile::Open(inputname,"read");
	for(int ic=0; ic<NC; ic++){
		hDeltaPhiSum[ic]=(TH1D*)fIn->Get(Form("hDeltaPhiSum_C%02d",ic));
		hDeltaPhiSum[ic]->Rebin(2);
	}

}

void FitDrawTwo(int ic=0)
{
	cout<<"FitDrawTwo " << ic <<endl;
	TString ParName[NH+1]={"const","v_1","v_2","v_3","v_4","v_5","v_6","v_7"};
	TString strformula = "[0]*(1";
	for (Int_t ih=1; ih<=NH; ih++){
		strformula += Form("+2*TMath::Power([%d],2)*TMath::Cos(%d*x)",ih,ih);
	}
	strformula+=")";
	TF1 *fFit = new TF1("fFit", strformula, 0, 2.0*TMath::Pi());
	fFit->SetParameter(0,1E4);
	for (Int_t i=1; i<=NH; i++) fFit->SetParameter(i,0.06);
	for (Int_t i=0; i<=NH+1; i++) fFit->SetParName(i,ParName[i]);
	TF1 *fFitvn[NH];

	gStyle->SetOptStat(0);

	TCanvas *can = new TCanvas("C","canvas",1024,740);
	can->SetFillStyle(4000);
	//fit
	hDeltaPhiSum[ic]->Fit("fFit"); 

	double lowx = 0.,highx=2*TMath::Pi();
  	double ly=hDeltaPhiSum[ic]->GetMinimum()*0.99,hy=hDeltaPhiSum[ic]->GetMaximum()*1.01;
  	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
  	hset( *hfr, "#Delta#phi=#phi_{1}-#phi_{2}", "dN/d#Delta#phi",0.9,0.9, 0.05,0.05, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
  	hfr->Draw();
  	
	hDeltaPhiSum[ic]->SetMarkerStyle(20);
	hDeltaPhiSum[ic]->Draw("psame");
	fFit->SetLineColor(1);
	fFit->Draw("same");

    
    for (Int_t i=1; i<=NH; i++){
    	TString formula = Form("[0]*(1 + 2*TMath::Power([1],2)*TMath::Cos(%d*x))",i);
		fFitvn[i]= new TF1(Form("fFitvn%02d",i),formula, 0, 2.0*TMath::Pi());//Just for drawing
	}

	//get vn's from fit
	cout << "ok, 2!" <<endl;
    TLegend *legendPhi = new TLegend(0.6,0.6,0.8,0.85,"","brNDC");
    legendPhi->SetTextSize(0.03);legendPhi->SetBorderSize(0);legendPhi->SetFillStyle(0);//legend settings;
	legendPhi->AddEntry((TObjArray*)NULL,Form("Centrality %s",strCentrality[ic].Data())," ");
	//for loop 
	for (Int_t i=2; i<=NH; i++) {
		vn[i][ic]=fFit->GetParameter(i);
		vnError[i][ic]=fFit->GetParError(i);
		fFitvn[i]->SetParameter(1,vn[i][ic]);// Setting individual component vn
		fFitvn[i]->SetParameter(0, fFit->GetParameter(0));//Normalization
		fFitvn[i]->SetLineColor(gColors[i]);
		fFitvn[i]->Draw("same");
		legendPhi->AddEntry(fFitvn[i],Form("v_{%d} = %.3f #pm %.4f ",i, vn[i][ic], vnError[i][ic]),"l");
	}	
	cout << " ok 3!" << endl;
	legendPhi->Draw();
	gPad->GetCanvas()->SaveAs(Form("../figs/TwoPartDecomposeC%02d.pdf",ic));

}

void SaveVns()
{
	double Cent[NC] = {0.,1.,2.};
	double eCent[NC] = {0.,0.,0.};
	TGraphErrors *gr_fittwo[NH];
	for(int ih=1; ih<=NH; ih++)gr_fittwo[ih]= new TGraphErrors(NC,Cent,vn[ih],eCent,vnError[ih]);
	TFile *output = new TFile("out_VnFitTwo.root","recreate");
	for(int ih=1; ih<=NH; ih++)gr_fittwo[ih]->Write(Form("gr_fittwo_H%02d_cent",ih));
	output->Write();
	output->Close();
}

