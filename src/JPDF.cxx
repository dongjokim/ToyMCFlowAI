#include "TMath.h"
#include  "JPDF.h"
// #include  "JFlowInput.h"


//______________________________________________________________________________
JPDF::JPDF(JFlowInput* jin)    // constructor
{
     //members
    jinput = jin;
    for (int i=0; i<NH; i++){
        Psi_n[i] = -999;
        v_n[i] = -999;
    }
    
}

void JPDF::CreatePDF()
{
    TString strformula = "[0]*(1";
    for (UInt_t ih=0; ih<NH; ih++){
        strformula += Form("+2*[%d]*TMath::Cos(%d*(x-[%d]))",ih+1,ih+1,NH+ih+1);
    }
    strformula+=")";
    std::cout<<strformula<<std::endl;
    fpdf = new TF1("Fourier", strformula, 0.0, 2.0*TMath::Pi());
}

void JPDF::GeneratePDF(TRandom3 *random, UInt_t ic)
{
    fpdf->SetParameter(0,inputNch[ic]);
    for (UInt_t n=0; n<NH; n++)
		Psi_n[n] = random->Uniform(-TMath::Pi()/(double)(n+1),TMath::Pi()/(double)(n+1)); //uniform[n]->GetRandom();//harmonic loop
	// v_n[1] = jinput->GetRandomV2(random, ic)*inputVn[1][ic]; // Fix warnings (Maxim)
    v_n[1] = random->Gaus(inputVn[1 ][ic],0.1*inputVn[1][ic]);
	v_n[2] = random->Gaus(inputVn[2][ic],0.1*inputVn[2][ic]);

    fpdf->SetParameter(1,v_n[1]); 
    fpdf->SetParameter(2,v_n[2]); 
    for (UInt_t i=NH; i<2*NH; i++)
		fpdf->SetParameter(i+1,Psi_n[i-NH]); //Setting the Psi parameters
}