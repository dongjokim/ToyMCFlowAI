//===========================================================
// JTreeGenerator.h
//===========================================================

#ifndef JTREEGENERATOR_H
#define JTREEGENERATOR_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

class JTreeGenerator {

	public:
		JTreeGenerator(TFile *fin); 
		virtual ~JTreeGenerator(){;}	  //destructor

		void GenerateTree();
		void AddEvent(int i, int cent, int Ntrk, double Psi_n[3], double v2, double v3);
        void AddTrack(int i, double px, double py, double pz, double E,double phi, double eta, double pt, double mass);
		void FillTree();
		void WriteTree();
		void AddEvent(Int_t random_seed, int iEvent, double cent, int ic, int Ntrk, double Psi_n[3], double v2, double v3);
		void ClearEvent(){event->Clear();tracks->Clear();}

	public:
		TFile *fOutRoot;
        TTree *jTree;
        TTree *vTree;
		TClonesArray *event;
		TClonesArray *tracks;
};

#endif

