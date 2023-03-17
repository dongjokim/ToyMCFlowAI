//===========================================================
// JPDF.h
//===========================================================
#ifndef JPDF_H
#define JPDF_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <TRandom3.h>
#include <TF1.h>
#include "JFlowInput.h"

class JPDF  {
	public:
		JPDF(JFlowInput *jinput);

		virtual ~JPDF(){;}		//destructor

		void CreatePDF();
        void GeneratePDF(TRandom3 *random, UInt_t ic);
        Double_t *GetSymmetryPlanes() {return Psi_n;}
        Double_t *GetVn() {return v_n;}
        TF1 *GetPDF() {return fpdf;}
    public:
        JFlowInput *jinput;
        Double_t Psi_n[NH];
        Double_t v_n[NH];
        TF1 *fpdf;
};

#endif