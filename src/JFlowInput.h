//===========================================================
// JFlowInput.h
//===========================================================
#ifndef JFLOWINPUT_H
#define JFLOWINPUT_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "TRandom3.h"
#include "TF1.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TGraphErrors.h>
#include "toyflowinputs.h" // mean vn stored here

// Fluctuation tables
const UInt_t tableId[] = {223,220,224,225,226,221,227,228,229,222,230,231}; //indexing table because the HEPdata table order was messed up by its author
const UInt_t vnPDFN = sizeof(tableId)/sizeof(tableId[0]);

class JFlowInput {
	public:
		JFlowInput(); 
		virtual ~JFlowInput(){;}	  //destructor

		void LoadAliceData();
		Double_t GetRandomV2(TRandom3 *rng, UInt_t ic);


	public:
		UInt_t randint;
		Double_t v2;
		Double_t xmin;
		Double_t xmax;

		TF1 *f1;
		TGraphErrors *temp_pdf;
		TGraphErrors *pgrVnPDF[vnPDFN];
		
};

#endif