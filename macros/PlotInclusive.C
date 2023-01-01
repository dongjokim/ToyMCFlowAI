#include "TMath.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TGraphPainter.h"
#include "TFile.h"
#include "TString.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <vector>
#include "../include/toyflowinputs.h"
#include "../include/rootcommon.h"
#include "../include/Filipad.h"

using namespace std;

int gMarkers[]= {20,24,21,25,22,26,23,27,32,28};
int gColors[]={kRed+1, kOrange+2, kCyan+2, kSpring-6, kRed-7, kOrange+1,kCyan-6,kGreen+7,kRed-9,kOrange-9,kAzure+6,kGreen-9};
int gStyles[]={1,2,3,4,5,6,7,8,9,10};

TH1D *hInclusivePhi;
TF1 *fNUE;
void LoadGraphs(TString inputfile);
void DrawInclusive();

void PlotInclusive(TString inputfile = "../outputs/flowoutput_BF50_NUE_9385abc_1000Evts.root"){
	LoadGraphs(inputfile);
	DrawInclusive();
}

void LoadGraphs(TString inputfile){
	TFile *fIn = TFile::Open(inputfile,"read");
	cout<<"Opening file: "<< endl;
	hInclusivePhi = (TH1D*)fIn->Get("hInclusivePhi");
	fNUE = (TF1*)fIn->Get("fNUE");
}

void DrawInclusive(){
	gStyle->SetOptStat(0);
	TCanvas *can = new TCanvas("C","canvas",1024,740);
	can->SetLeftMargin(0.15);
	can->SetBottomMargin(0.15);
	can->SetFillStyle(4000);

	TLegend *legend = new TLegend(0.2,0.6,0.6,0.85,"","brNDC");
	legend->SetTextSize(0.04);legend->SetBorderSize(0);legend->SetFillStyle(0);//legend settings;
	double lowx = 0.0,highx=2*TMath::Pi();
	double ly=0.01,hy=hInclusivePhi->GetMaximum()*1.01;
	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
	hset( *hfr, "#phi", "counts",0.7,0.7, 0.07,0.07, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
	hfr->Draw();

	hInclusivePhi->SetLineColor(gColors[1]);
	hInclusivePhi->SetMarkerStyle(gMarkers[0]);
	hInclusivePhi->SetMarkerColor(gColors[1]);
	hInclusivePhi->Draw("lpsame");

	double ymaxinclusive = hInclusivePhi->GetBinContent(150);
	fNUE->SetParameter(0,ymaxinclusive);
	fNUE->SetLineColor(gColors[2]);
	fNUE->Draw("same");

	gPad->GetCanvas()->SaveAs("../figs/Inclusive.pdf");
}