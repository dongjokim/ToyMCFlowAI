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

TH1D *hPhiPsi[NH][NC];
TF1 *fFit[NH][NC];
Double_t vn[NH][NC]={{-999}};
Double_t vnError[NH][NC]={{-999}};


void LoadData(TString);
void FitDrawPhi(int);
void SaveVns();

//-----------Main Function------------------
void FitSingle(TString infile="../output/toymcflowao_1d0eb42_10k.root")
{
	LoadData(infile);
	for (int ic=0; ic<NC; ic++)FitDrawPhi(ic);
	SaveVns();
}

//-------Member Functions-----------------
void LoadData(TString inputname)
{
	TFile *fIn = TFile::Open(inputname,"read");
	for(int ih=1; ih<=NH; ih++){
		for(int ic=0; ic<NC; ic++){
			hPhiPsi[ih][ic]=(TH1D*)fIn->Get(Form("hPhiPsiC%02dH%02d",ic,ih));
			hPhiPsi[ih][ic]->Rebin(4);
		}
	}

}

void FitDrawPhi(int ic=0)
{	
	for (Int_t ih=1; ih<=NH; ih++){
		TString formula = Form("[0]*(1 + 2*[1]*TMath::Cos(%d*x))",ih);
		fFit[ih][ic] = new TF1(Form("fFitH%02d_C%02d",ih,ic), formula,0, 2.0*TMath::Pi());
		fFit[ih][ic]->SetParameter(0,1E4);
		fFit[ih][ic]->SetParameter(ih,inputVn[ih][ic]);
	}
	for (Int_t ih=1; ih<=NH; ih++) hPhiPsi[ih][ic]->Fit(fFit[ih][ic]->GetName());
	for (Int_t ih=1; ih<=NH; ih++){
		vn[ih][ic]=fFit[ih][ic]->GetParameter(1);
		vnError[ih][ic]=fFit[ih][ic]->GetParError(1);
	}


	gStyle->SetOptStat(0);
	TCanvas *can = new TCanvas("C2","canvas",1024,740);
	
	can->SetFillStyle(4000);
	can->SetLeftMargin(0.15);
   	can->SetBottomMargin(0.15);
   	
   	//For editing canvas #include "include/rootcommon.h"
   	double lowx = 0.,highx=2*TMath::Pi();
  	//double ly=hDeltaPhiSum[ic]->GetMinimum()*0.99,hy=hDeltaPhiSum[ic]->GetMaximum()*1.01;
  	Double_t ly = 8500, hy=16500;
  	TH2F *hfr = new TH2F("hfr"," ", 100,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
  	hset( *hfr, "#Delta#phi=#phi-#psi_{n}", "dN/d#Delta#phi",0.9,0.9, 0.15,0.05, 0.01,0.01, 0.03,0.03, 510,505);//settings of the upper pad: x-axis, y-axis
  	hfr->Draw();
  	TLegend *legend = new TLegend(0.5,0.6,0.8,0.85,"","brNDC");
    legend->SetTextSize(0.04);legend->SetBorderSize(0);legend->SetFillStyle(0);//legend settings;
  	legend->AddEntry((TObjArray*)NULL,Form("Centrality %s",strCentrality[ic].Data())," ");

  	for(int ih=2; ih<=NH; ih++){
  		cout << ih << endl;
  		//hPhiPsi[ih][ic]->SetTitle(Form("Cent%02d",ic));
  		hPhiPsi[ih][ic]->SetMarkerStyle(20);
		hPhiPsi[ih][ic]->SetMarkerColor(gColors[ih]);
		hPhiPsi[ih][ic]->Draw("psame");
		fFit[ih][ic]->SetLineColor(gColors[ih]);
		fFit[ih][ic]->Draw("same");
		legend->AddEntry(fFit[ih][ic],Form("v_{%d} = %.3f #pm %.4f ",ih, vn[ih][ic], vnError[ih][ic]),"l");
  	}
  	legend->Draw();
	gPad->GetCanvas()->SaveAs(Form("../figs/SingleParticle_C%02d.pdf",ic));
}
void SaveVns()
{
	double Cent[NC] = {0.,1.,2.};
	double eCent[NC] = {0.,0.,0.};
	TGraphErrors *gr_fitsingle[NH];
	for(int ih=1; ih<=NH; ih++)gr_fitsingle[ih]= new TGraphErrors(NC,Cent,vn[ih],eCent,vnError[ih]);
	TFile *output = new TFile("out_VnFitSingle.root", "recreate");
	for(int ih=1; ih<=NH; ih++)gr_fitsingle[ih]->Write(Form("gr_fitsingle_H%02d_cent",ih));
	output->Write();
	output->Close();
}
