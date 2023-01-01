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

TH1D *hResolutionDist[NH][NC];

void LoadData(TString);
void DrawRes(int);
//void SaveVns();

//-----Main Function------
void Resolution(TString infile="../output/toymcflowao_1d0eb42_10k.root")
{
	LoadData(infile);
	for(Int_t ic=0; ic<NC; ic++) DrawRes(ic);
}

//-----Member Functions------
void LoadData(TString inputname)
{
	TFile *fIn = TFile::Open(inputname,"read");
	fIn->Print();
	for (Int_t ih=0; ih<NH; ih++){
		for(Int_t ic=0; ic<NC; ic++){
			cout<< ih << " " << ic <<endl;
			hResolutionDist[ih][ic]=(TH1D*)fIn->Get(Form("hResolutionDistC%02dH%02d",ic,ih+1));
		} 
    }
}

void DrawRes(int ic=0)
{
	gStyle->SetOptStat(0);
	TCanvas *can = new TCanvas("C","canvas",1024,740);
	can->SetLeftMargin(0.15);
   	can->SetBottomMargin(0.15);
	can->SetFillStyle(4000);

    double lowx = -5,highx=5;
  	double ly=hResolutionDist[0][ic]->GetMinimum()*0.1,hy=hResolutionDist[0][ic]->GetMaximum()*1.4;
  	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
  	hset( *hfr, "#psi_{n}-#psi_{EP}", "dN/d#phi",0.7,0.7, 0.07,0.07, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
  	hfr->Draw();

  	TLegend *legendRes = new TLegend(0.5,0.6,0.8,0.85,"","brNDC");
    legendRes->SetTextSize(0.04);legendRes->SetBorderSize(0);legendRes->SetFillStyle(0);//legend settings;

    for(Int_t i=1; i<NH; i++){
    	hResolutionDist[i][ic]->SetLineColor(gColors[i]);
   		hResolutionDist[i][ic]->SetLineWidth(5);
   		hResolutionDist[i][ic]->Draw("lpsame");
   		legendRes->AddEntry(hResolutionDist[i][ic],Form("Cent=%02d, n=%d",ic,i+1));
   	}
   	legendRes -> Draw("same");
   	gPad->GetCanvas()->SaveAs(Form("../figs/ResolutionC%02d.pdf",ic));
}