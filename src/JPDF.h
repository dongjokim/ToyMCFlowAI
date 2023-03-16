//===========================================================
// JPDF.h
//===========================================================

#define JPDF_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "JFlowInput.h"
#include "TRandom3.h"

#include <TF1.h>

class JPDF  {
	public:
		JPDF(JFlowInput *jinput);

		virtual ~JPDF(){;}		//destructor

		void CreatePDF();
        void GeneratePDF(TRandom3 *random);
    public:
        JFlowInput *jinput;
        Double_t Psi_n[NH];
        Double_t v_n[NH];
        TF1 *fpdf;
};
