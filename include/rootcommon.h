void hset(TH1& hid, TString xtit="", TString ytit="",
        double titoffx = 1.1, double titoffy = 1.1,
        double titsizex = 0.06, double titsizey = 0.06,
        double labeloffx = 0.01, double labeloffy = 0.001,
        double labelsizex = 0.05, double labelsizey = 0.05,
        int divx = 505, int divy=505)
{
    hid.GetXaxis()->CenterTitle(1);
    hid.GetYaxis()->CenterTitle(1);

    hid.GetXaxis()->SetTitleOffset(titoffx);
    hid.GetYaxis()->SetTitleOffset(titoffy);

    hid.GetXaxis()->SetTitleSize(titsizex);
    hid.GetYaxis()->SetTitleSize(titsizey);

    hid.GetXaxis()->SetLabelOffset(labeloffx);
    hid.GetYaxis()->SetLabelOffset(labeloffy);

    hid.GetXaxis()->SetLabelSize(labelsizex);
    hid.GetYaxis()->SetLabelSize(labelsizey);

    hid.GetXaxis()->SetNdivisions(divx);
    hid.GetYaxis()->SetNdivisions(divy);

    hid.GetXaxis()->SetTitle(xtit);
    hid.GetYaxis()->SetTitle(ytit);

    hid.GetXaxis()->SetLabelFont(42);
    hid.GetYaxis()->SetLabelFont(42);
    hid.GetXaxis()->SetTitleFont(42);
    hid.GetYaxis()->SetTitleFont(42);
}
/*
void hsetChangeXaxisLabel(TH1& hid, TString xtit="", TString ytit="",
        double titoffx = 1.1, double titoffy = 1.1,
        double titsizex = 0.06, double titsizey = 0.06,
        double labeloffx = 0.01, double labeloffy = 0.001,
        double labelsizex = 0.05, double labelsizey = 0.05,
        int divx = 505, int divy=505,
        TGraphErrors *gr, 
        int ndatapoints = 3, TString ticklabel= "0-5\%")
{
    hid.GetXaxis()->CenterTitle(1);
    hid.GetYaxis()->CenterTitle(1);

    hid.GetXaxis()->SetTitleOffset(titoffx);
    hid.GetYaxis()->SetTitleOffset(titoffy);

    hid.GetXaxis()->SetTitleSize(titsizex);
    hid.GetYaxis()->SetTitleSize(titsizey);

    hid.GetXaxis()->SetLabelOffset(labeloffx);
    hid.GetYaxis()->SetLabelOffset(labeloffy);

    hid.GetXaxis()->SetLabelSize(labelsizex);
    hid.GetYaxis()->SetLabelSize(labelsizey);

    hid.GetXaxis()->SetNdivisions(divx);
    hid.GetYaxis()->SetNdivisions(divy);

    hid.GetXaxis()->SetTitle(xtit);
    hid.GetYaxis()->SetTitle(ytit);

    hid.GetXaxis()->SetLabelFont(42);
    hid.GetYaxis()->SetLabelFont(42);
    hid.GetXaxis()->SetTitleFont(42);
    hid.GetYaxis()->SetTitleFont(42);

    TAxis* xAxis = gr->GetXaxis();
    int nbins = xAxis->GetNbins();
    for(int ind=0; ind < ndatapoints; ind++){
        int binindex = xAxis->FindBin(ind*1.0);
        xAxis->ChangeLabel(binindex,-1,1,-1,3,-1,ticklabel);
    }
}
*/
// Theory-Data/Theory
TGraphAsymmErrors *GetDataOverTheory(TGraphAsymmErrors *gr, TF1 *ftheory ){
    double x[300], y[300], exl[300], exh[300], eyl[300], eyh[300];
    int NC =  gr->GetN();
    for(int ii=0;ii<NC;ii++){
        gr->GetPoint(ii,x[ii],y[ii]);
        exl[ii] = gr->GetErrorXlow(ii);
        exh[ii] = gr->GetErrorXhigh(ii);
        eyl[ii] = gr->GetErrorYlow(ii);
        eyh[ii] = gr->GetErrorYhigh(ii);
    }
    for(int ii=0;ii<NC;ii++){
        y[ii]   = ( ftheory->Eval(x[ii]) - y[ii] ) / ftheory->Eval(x[ii]);
        eyl[ii] = eyl[ii]/ftheory->Eval(x[ii]);
        eyh[ii] = eyh[ii]/ftheory->Eval(x[ii]);
    }
    return new TGraphAsymmErrors(NC, x, y, exl, exh, eyl, eyh);
}

// error must be corrected later
TGraphAsymmErrors *GetDataOverTheory(TGraphAsymmErrors *grData, TGraphAsymmErrors *grTheo ){
    double x[300], y[300], exl[300], exh[300], eyl[300], eyh[300];
    int NC =  grData->GetN();
    for(int ii=0;ii<NC;ii++){
        grData->GetPoint(ii,x[ii],y[ii]);
        exl[ii] = grData->GetErrorXlow(ii);
        exh[ii] = grData->GetErrorXhigh(ii);
        eyl[ii] = grData->GetErrorYlow(ii);
        eyh[ii] = grData->GetErrorYhigh(ii);
    }
    for(int ii=0;ii<NC;ii++){
        y[ii]   = ( grTheo->Eval(x[ii]) - y[ii] ) / grTheo->Eval(x[ii]);
        eyl[ii] = eyl[ii]/grTheo->Eval(x[ii]);
        eyh[ii] = eyh[ii]/grTheo->Eval(x[ii]);
    }
    return new TGraphAsymmErrors(NC, x, y, exl, exh, eyl, eyh);
}

// Data/Fit
TGraphAsymmErrors *GetRatio(TGraphAsymmErrors *gr, TF1 *ftheory ){
    double x[300], y[300], exl[300], exh[300], eyl[300], eyh[300];
    int NC =  gr->GetN();
    for(int ii=0;ii<NC;ii++){
        gr->GetPoint(ii,x[ii],y[ii]);
        exl[ii] = gr->GetErrorXlow(ii);
        exh[ii] = gr->GetErrorXhigh(ii);
        eyl[ii] = gr->GetErrorYlow(ii);
        eyh[ii] = gr->GetErrorYhigh(ii);
    }
    for(int ii=0;ii<NC;ii++){
        y[ii]   = y[ii]  / ftheory->Eval(x[ii]);
        eyl[ii] = eyl[ii]/ftheory->Eval(x[ii]);
        eyh[ii] = eyh[ii]/ftheory->Eval(x[ii]);
    }
    return new TGraphAsymmErrors(NC, x, y, exl, exh, eyl, eyh);
}

// error must be corrected later
TGraphAsymmErrors *GetRatio(TGraphAsymmErrors *grData, TGraphAsymmErrors *grTheo ){
    double x[300], y[300], exl[300], exh[300], eyl[300], eyh[300];
    int NC =  grData->GetN();
    for(int ii=0;ii<NC;ii++){
        grData->GetPoint(ii,x[ii],y[ii]);
        exl[ii] = grData->GetErrorXlow(ii);
        exh[ii] = grData->GetErrorXhigh(ii);
        eyl[ii] = grData->GetErrorYlow(ii);
        eyh[ii] = grData->GetErrorYhigh(ii);
    }
    for(int ii=0;ii<NC;ii++){
        y[ii]   = y[ii]/grTheo->Eval(x[ii]);
        eyl[ii] = eyl[ii]/grTheo->Eval(x[ii]);
        eyh[ii] = eyh[ii]/grTheo->Eval(x[ii]);
    }
    return new TGraphAsymmErrors(NC, x, y, exl, exh, eyl, eyh);
}

TGraphErrors* GetRatio( TGraphErrors * l, TGraphErrors *r ){
        TGraphErrors * gr_ratio = new TGraphErrors( l->GetN() );
        TGraph ger( r->GetN(),  r->GetX(),l->GetEY() );
        for( int i=0; i< l->GetN(); i++ ){
                double x = l->GetX()[i];
                double y1 = l->GetY()[i];
                double ey1 = l->GetEY()[i];
                double y2 = r->Eval(x);
                double ey2 = ger.Eval(x);


                double ratio = y1 / y2;
                gr_ratio->SetPoint( i,  x, ratio);
                gr_ratio->SetPointError( i,  0, ratio*TMath::Sqrt( ey1*ey1/y1/y1+ey2*ey2/y2/y2));
        }
        return gr_ratio;
}

// need to check the error!!!!!
TGraphErrors* GetRatio( TGraphErrors * l, TGraphAsymmErrors *r ){
        TGraphErrors * gr_ratio = new TGraphErrors( l->GetN() );
        TGraph ger( r->GetN(),  r->GetX(),l->GetEY() );
        for( int i=0; i< l->GetN(); i++ ){
                double x = l->GetX()[i];
                double y1 = l->GetY()[i];
                double ey1 = l->GetEY()[i];
                double y2 = r->Eval(x);
                double ey2 = ger.Eval(x);


                double ratio = y1 / y2;
                gr_ratio->SetPoint( i,  x, ratio);
                gr_ratio->SetPointError( i,  0, ratio*TMath::Sqrt( ey1*ey1/y1/y1+ey2*ey2/y2/y2));
        }
        return gr_ratio;
}


void makeHistHEPDATA(TH1F *srch, TH1F *he1, TH1F *he2, TH1F *tarh)
{
    const int nb=srch->GetNbinsX();
    for(int i=1;i<=nb;i++) {
        double rel_e1 = he1->GetBinContent(i);
        double rel_e2 = he2->GetBinContent(i);
        double toterr = TMath::Sqrt(rel_e1*rel_e1+rel_e2*rel_e2);
        //cout << i <<"\t"<< rel_e1<<"\t"<< rel_e2<<"\t"<<toterr<<endl;
        tarh->SetBinContent(i,srch->GetBinContent(i));
        tarh->SetBinError(i,toterr);
    }
}
