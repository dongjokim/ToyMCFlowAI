#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"

class Filipad {

public:

  // ---- Costructor ------------
  Filipad(int inID=1, float inRelSize=1.1, float inR = 0.4, int inXOffset = 100, int inYOffset=100, float inAspect=0.7, int ichop=5){
    aspektCanvas = inAspect;
    sizeCanvas   = 600*inRelSize;

    MarginLeft   = 0.21;
    MarginBottom = 0.1; 
    MarginRight  = 0.05;
    MarginTop    = 0.09;
    ID           = inID;
    mcpad        = Form("c%d",ID);
    ratio = inR;

    //sdxCanvas    = inXOffset;
    int inID0 = inID-1;
    sdxCanvas    = inXOffset*(inID0%ichop)+10;
    sdyCanvas    = inYOffset*(inID0-inID0%ichop)/ichop+10;
    //cout <<"sdxCanvas= "<<  sdxCanvas <<" sdyCanvas="<< sdyCanvas <<endl; 
    space        = 0;
  }

  // ---- Destructor ------------
  ~Filipad(){
    cout<<"Destructor"<<endl;
    if(C)      delete C;
    if(toppad) delete toppad;
  }


  TPad* GetPad(int padID){ return (TPad*) toppad->cd(padID);}//coordinates 0,0 = upper left 

  //-------------------------------------------------------------------------------------
  void Draw(){ 

    char name[200];
    // cout<<"Draw"<<endl;
    C = new TCanvas(mcpad, mcpad, sdxCanvas, sdyCanvas, sizeCanvas*aspektCanvas, sizeCanvas);//the main canvas
    C->SetFillStyle(4000); C->SetFillColor(10);
    gStyle->SetOptStat(0);    gStyle->SetOptTitle(0);
    C->SetTopMargin(0.); C->SetBottomMargin(0.);//our pads will have no margin
    C->SetLeftMargin(0.);C->SetRightMargin(0.);
    //C->cd();
    C->Draw();

    toppad = gPad;
    toppad->Clear();

    TPad   *pp      = NULL;
    //sprintf(name, Form("UPad%d",ID) ); //some dummy name
    pp = new TPad( Form("UPad%d",ID),Form("UPad%d",ID),0, 0 + ratio + space  , 1, 1, 0); //create pad

    pp->SetNumber(1);   //assign a number to it. Possible to access it via :  toppad->cd(ih);  
    pp->SetTopMargin(MarginTop/(1-ratio)); pp->SetBottomMargin(0.0015);//our pads will have no margin
    pp->SetLeftMargin(MarginLeft);pp->SetRightMargin(MarginRight);
    pp->Draw();

    //sprintf(name, Form("LPad%d",ID) ); //some dummy name
    pp = new TPad(Form("LPad%d",ID),Form("LPad%d",ID),0, 0, 1, ratio ,0); //create pad
    pp->SetNumber(2);   //assign a number to it. Possible to access it via :  toppad->cd(ih); 
    pp->SetTopMargin(0.0015); pp->SetBottomMargin(MarginBottom/ratio);//our pads will have no margin
    pp->SetLeftMargin(MarginLeft);pp->SetRightMargin(MarginRight);
    pp->Draw();
  }


  //-------------------------------------------------------------------------------------
  void Hset(TH1* hid, TString xtit="", TString ytit="",
      double titoffx = 2.5, double titoffy = 1.5,
      double titsizex = 20, double titsizey = 20,
      double labeloffx = 0.01, double labeloffy = 0.001,
      double labelsizex = 16, double labelsizey = 16,
      int divx = 505, int divy=505){

    hid->GetXaxis()->CenterTitle(1);
    hid->GetYaxis()->CenterTitle(1);

    hid->GetXaxis()->SetTitleOffset(titoffx);
    hid->GetYaxis()->SetTitleOffset(titoffy);

    hid->GetXaxis()->SetTitleFont(43);
    hid->GetYaxis()->SetTitleFont(43);
    hid->GetXaxis()->SetTitleSize(titsizex);
    hid->GetYaxis()->SetTitleSize(titsizey);

    hid->GetXaxis()->SetLabelOffset(labeloffx);
    hid->GetYaxis()->SetLabelOffset(labeloffy);

    hid->GetXaxis()->SetLabelFont(43);
    hid->GetYaxis()->SetLabelFont(43);
    hid->GetXaxis()->SetLabelSize(labelsizex);
    hid->GetYaxis()->SetLabelSize(labelsizey);

    hid->GetXaxis()->SetNdivisions(divx);
    hid->GetYaxis()->SetNdivisions(divy);

    hid->GetXaxis()->SetTitle(xtit);
    hid->GetYaxis()->SetTitle(ytit);
  }

  void SetMarginLeft(float x){ MarginLeft = x;}
  void SetMarginRight(float x){ MarginRight = x;}
  void SetMarginTop(float x){ MarginTop = x;}
  void SetMarginBottom(float x){ MarginBottom = x;}

  void HSetX(double x0, double x1, TString xtitle, int logx=0, int gridx=0 ){ 
    fX0=x0;fX1=x1;fXTitle=xtitle; 
    fLogx=logx;fGridx=gridx;
  }
  void HSet1(double y0, double y1,TString ytitle, int logy=0 ,int gridy=0 ){
    //this->Draw();
    TPad *p = this->GetPad(1); //upper pad
    p->SetTickx(); p->SetLogx(fLogx); p->SetLogy(logy); p->cd();
    p->SetGridx(fGridx);p->SetGridy(gridy);
    p->SetLeftMargin(0.21);
    gStyle->SetOptStat(0);gStyle->SetOptTitle(0);
    gStyle->SetMarkerSize(1.6);
    TH2F * hfr = new TH2F(Form("hfr%dA",ID) ,"", 10, fX0, fX1, 10,y0,y1);
    this->Hset(hfr, fXTitle, ytitle);
    hfr->Draw();
  }

  void HSet2(double y0, double y1,TString ytitle, int logy=0 ,int gridy=0 ){
    //this->Draw();
    TPad *p = this->GetPad(2); //upper pad
    p->SetTickx(); p->SetGridy(0); p->SetLogx(fLogx); p->SetLogy(logy);p->cd();
    p->SetGridx(fGridx);p->SetGridy(gridy);

    TH2F * hfr = new TH2F(Form("hfr%dB",ID) ,"", 10, fX0, fX1, 10,y0,y1);
    this->Hset(hfr, fXTitle, ytitle);
    hfr->Draw();
  }




  //  M A I N     C A N V A S 
  int   ID;
  float aspektCanvas;      //size and positioning of the main canvas   
  int   sizeCanvas;
  int   sdxCanvas; 
  int   sdyCanvas;

  float MarginLeft; //Margins around the latice of nx times ny pads 
  float MarginBottom;
  float MarginRight;
  float MarginTop;

  float space;
  float ratio;
  TString mcpad;
  TCanvas *C;
  TVirtualPad *toppad;

  double fX0,fX1;
  TString fXTitle;
  int   fLogx, fGridx;

};


TGraphErrors *GetDataOverTheory(TGraphErrors *gr, TF1 *ftheory ){
    double x[300], y[300], ex[300], ey[300];
    int NC =  gr->GetN();
    for(int ii=0;ii<NC;ii++){
        gr->GetPoint(ii,x[ii],y[ii]);
        ex[ii] = gr->GetErrorX(ii);
        ey[ii] = gr->GetErrorY(ii);
    }
    for(int ii=0;ii<NC;ii++){
        y[ii]   = ( ftheory->Eval(x[ii]) - y[ii] ) / ftheory->Eval(x[ii]);
        ey[ii] = ey[ii]/ftheory->Eval(x[ii]);
    }
    return new TGraphErrors(NC, x, y, ex, ey);
}

TGraphErrors *GetDataOverTheory(TH1 *h, TF1 *ftheory, double lx, double hx ){
	double x[700], y[700], ex[700], ey[700];
	int NC =  h->GetNbinsX();
	cout << "NC = "<< NC << endl;
	int counter=0;
	for(int ii=1;ii<=NC;ii++){
		double xin = h->GetBinCenter(ii);
		if(xin>lx && xin<hx) {
			y[counter] = h->GetBinContent(ii);
			x[counter] = xin;
			ex[counter] = 0;
			ey[counter] = h->GetBinError(ii);
			y[counter]   = y[counter]  / ftheory->Eval(xin);
			ey[counter] = ey[counter]/ftheory->Eval(xin);
			counter++;
		}
	}
	return new TGraphErrors(counter, x, y, ex, ey);
}
