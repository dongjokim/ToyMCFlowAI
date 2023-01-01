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

Int_t gMarkers[]= {20,24,21,25,22,26,23,27,32,28};
Int_t gColors[]={kRed+1, kOrange+2, kCyan+2, kSpring-6, kRed-7, kOrange+1,kCyan-6,kGreen+7,kRed-9,kOrange-9,kAzure+6,kGreen-9};
Int_t gStyles[]={1,2,3,4,5,6,7,8,9,10};
const int NMethod=3;
const int Nconfigs=2;
TGraphErrors *gr_pvn[Nconfigs][NC][NMethod];
TString gr_Names[NMethod]={"SP","TP","EP"};
Double_t vnIn[NC][NH]={{0.}};
TGraphErrors *gr_vnin[NC];
string strHDummy[] = {"2","3","4","5","6","7","8","9","10","11","12"};//vn
TString ConfigNames[NMethod]={
	"Sample from Function",
	"Sample from Histogram"
};
const int Nbg = 2;
TString BGNames[Nbg]={
	"Uniform",
	"NUE",
};

TString rootfiles[]={
	"../results/VnOutput_BG00_1Hole_SampF.root",
	"../results/VnOutput_BG00_1Hole_SampH.root"
};

void LoadData(); //Loading TGraphs
void DrawPSpectra(int ic=0, int i=0);
//void SaveGraphs(int);

//---Main Function------
void CompSampCorr()
{
	LoadData();
	for(int ic=0; ic<NC; ic++) {
		for(int i=0; i<1; i++){
			DrawPSpectra(ic,i);
			//SaveGraphs(ic);
		}
		
	}
}

//------Member Functions-------
void LoadData()
{
	for(int icon=0; icon<Nconfigs; icon++){
		TFile *fIn = TFile::Open(rootfiles[icon],"read");
		cout<< "config"<<icon<<endl;
		for(int i=0; i<NMethod; i++){
			cout<<"method"<< i <<endl;
			for (int ic=0; ic<NC; ic++){
				cout<< "centrality"<<ic <<endl;
				gr_pvn[icon][ic][i]=(TGraphErrors*)fIn->Get(Form("gr_pv%02d_%s",ic+1, gr_Names[i].Data()));
				gr_pvn[icon][ic][i]->Print();
				gr_pvn[icon][ic][i]->RemovePoint(0);
			}
		}
	}// end of config loop
	//for loop for drawing input values
	for(int ic=0; ic<NC; ic++){
	  	for (int ih=0; ih<NH; ih++){
	  		vnIn[ic][ih]=inputVn[ih][ic];
	  	}
	}
  	double px[NH] = {0.};
	double pxe[NH] = {0.};
	double vnInError[NC][NH] = {{0.}};
	for (int ih=0; ih<NH; ih++){px[ih]=ih;pxe[ih]=0.;vnInError[0][ih]=vnInError[1][ih]=vnInError[2][ih]=0.;}
	// loop over NMethod
	for(int ic=0; ic<NC; ic++){
		gr_vnin[ic] = new TGraphErrors(NH,px,vnIn[ic],pxe,vnInError[ic]);
		gr_vnin[ic]->Print();
		gr_vnin[ic]->RemovePoint(0);
	} 
	//------end of drawing input values 
}

void DrawPSpectra(int ic=0, int i=0)
{
	gStyle->SetOptStat(0);
	TCanvas *can = new TCanvas(Form("C%02d",ic),"canvas",1024,740);
	can->SetFillStyle(4000);
	can->SetLeftMargin(0.15);
   	can->SetBottomMargin(0.15);
   	can->SetLogy(1);
   	Filipad *fpad= new Filipad(1,1.1,0.4,100,100,1.2,5);
	fpad->Draw();
	//====Upper pad
	TPad *p = fpad->GetPad(1); 
	p->SetTickx(); p->SetLogx(0); p->SetLogy(0); p->cd();
	TLegend *legend = new TLegend(0.4,0.5,0.8,0.75,"","brNDC");
    legend->SetTextSize(0.04);legend->SetBorderSize(0);legend->SetFillStyle(0);//legend settings;
	double lowx = 0.5,highx=6.5;
  	double ly=-1.2*TMath::MinElement(NC,gr_vnin[ic]->GetY()),hy=2.0*TMath::MaxElement(NC,gr_vnin[ic]->GetY());
  	TH2F *hfr = new TH2F("hfr"," ", 7,lowx, highx, 10, ly, hy); // numbers: tics x, low limit x, upper limit x, tics y, low limit y, upper limit y
  	hset( *hfr, "n+1", "v_{n}",0.7,0.7, 0.07,0.07, 0.01,0.01, 0.04,0.04, 510,505);//settings of the upper pad: x-axis, y-axis
  	//Changelabel(hfr,gr_pvn[0][ic],strHDummy);
  	hfr->Draw();
  	legend->AddEntry((TObjArray*)NULL,Form("Centrality %s - %s method for NUA with 1 hole",strCentrality[ic].Data(), gr_Names[i].Data())," ");

	gr_vnin[ic]->SetLineColor(kSpring-6);
	gr_vnin[ic]->SetLineWidth(3);
	gr_vnin[ic]->Draw("lsame");
	legend->AddEntry(gr_vnin[ic],"Input","l");

  	for(int icon=0; icon<Nconfigs; icon++){
  	
		gr_pvn[icon][ic][i]->SetLineColor(gColors[icon]);
		gr_pvn[icon][ic][i]->SetMarkerStyle(gMarkers[icon]);
		gr_pvn[icon][ic][i]->SetMarkerColor(gColors[icon]);
		gr_pvn[icon][ic][i]->Draw("plsame");
		legend->AddEntry(gr_pvn[icon][ic][i],Form("%s", ConfigNames[icon].Data()),"pl");
  		
  	}
  	legend -> Draw("same");

  	//====Lower pad, i.e. ratios
	TGraphErrors *grRatio[Nconfigs];
	
	grRatio[0]=GetRatio(gr_pvn[0][ic][i],gr_vnin[ic]);
	grRatio[1]=GetRatio(gr_pvn[1][ic][i],gr_vnin[ic]);
	
	p = fpad->GetPad(2);
	p->SetTickx(); p->SetGridy(0); p->SetLogx(0), p->SetLogy(0); p->cd();
	TH2F *hfr1 = new TH2F("hfr1"," ", 100, lowx, highx, 10, 0.2*TMath::MinElement(NC,grRatio[0]->GetY()), 2.0*TMath::MaxElement(NC,grRatio[0]->GetY()));
	//TH2F *hfr1 = new TH2F("hfr1"," ", 100, lowx, highx, 10, 0.0, 2.0);
	hset( *hfr1, "n+1", "Calculation/True Value",0.7,0.7, 0.07,0.07, 0.01,0.01, 0.06,0.06, 510,505);
	hfr1->Draw();
	for(int ir=0;ir<Nconfigs;ir++){
		for(int icon=1; icon<Nconfigs; icon++){

			grRatio[ir]->SetLineColor(gColors[ir]);
			grRatio[ir]->SetMarkerStyle(gMarkers[ir]);
			grRatio[ir]->SetMarkerColor(gColors[ir]);
			grRatio[ir]->Draw("lpsame");
		
		}
	}

   	gPad->GetCanvas()->SaveAs(Form("../figs/PSpectraC%02dFuncVSHist%s_00Bg.pdf",ic,gr_Names[i].Data()));
}

