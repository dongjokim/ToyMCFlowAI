#include "JFlowInput.h"
#include <TFile.h>

//______________________________________________________________________________
JFlowInput::JFlowInput(){   // constructor
    
}

void JFlowInput::LoadAliceData() // This will load the fluctuation graphs
{
    TFile *pfVnPDF = new TFile("HEPData-ins1666817-v1-root.root","read");
    
    for(UInt_t i = 0; i < vnPDFN; ++i)
	   pgrVnPDF[i] = (TGraphErrors*)pfVnPDF->Get(Form("Table %u/Graph1D_y1",tableId[i]));   
}

// Create random v2 fluctuation based on ALICE data
Double_t JFlowInput::GetRandomV2(TRandom3 *rng, UInt_t ic) {
    if (ic < 2) {
        temp_pdf = pgrVnPDF[ic]; 
    }else{
        randint = rng->Integer(2);
        temp_pdf = pgrVnPDF[ic+randint];
    }

    UInt_t N = temp_pdf->GetN();
    xmin = temp_pdf->GetPointX(0);
    xmax = temp_pdf->GetPointX(N-1);

    f1 = new TF1("f1",[&](double*x, double *){ return temp_pdf->Eval(x[0]); }, xmin, xmax, 1);

    return f1->GetRandom();

}
