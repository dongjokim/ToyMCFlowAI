
TF1 *fNUE;
TH1D *hBgPhi;
TH1D *hSignalPhi;

void checkmap(TString input){
	TFile *fIn= TFile::Open(input);
	fNUE= (TF1*)fIn->Get("fNUE");
	hBgPhi = (TH1D*)fIn->Get("hBgPhi");
	hSignalPhi = (TH1D*)fIn->Get("hSignalPhi");

	TCanvas *can = new TCanvas();
	can->SetLogy(1);
	double ymaxsignal = hSignalPhi->GetBinContent(150);
	double ymaxbg = hBgPhi->GetBinContent(150);
	fNUE->SetParameter(0,ymaxsignal);
	fNUE->SetLineColor(kRed);
	hBgPhi->SetLineColor(kGreen);

	hSignalPhi->Draw();
	fNUE->Draw("same");
	hBgPhi->Draw("same");
	TF1 *fNUEbg=(TF1*)fNUE->Clone();
	fNUEbg->SetParameter(0,ymaxbg);
	fNUEbg->SetLineColor(kBlue);
	fNUEbg->Draw("same");
	
}


