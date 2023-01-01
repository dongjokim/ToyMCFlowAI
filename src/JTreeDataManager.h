// $Id: JTreeDataManager.h,v 1.8 2008/02/12 15:51:27 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JTreeDataManager.h
  \brief
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@cc.jyu.fi
  \version $Revision: 1.8 $
  \date $Date: 2008/02/12 15:51:27 $
 */
////////////////////////////////////////////////////

#ifndef JTREEDATAMANAGER_H
#define JTREEDATAMANAGER_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TClonesArray.h>

#include  "JBaseTrack.h"
#include  "JBaseEventHeader.h"

class JTreeDataManager  {

	public:
		JTreeDataManager();

		virtual ~JTreeDataManager();		                    //destructor

		void ChainInputStream(const char* infileList);
		int LoadEvent( int ievt );
		virtual void RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill);
		virtual bool IsGoodEvent();
		// GETTER
		TChain * GetChain(){ return fChain; };
		int GetNEvents(){ return fChain->GetEntries(); } 
        JBaseEventHeader * GetEventHeader(){ return fEventHeader; };
        TClonesArray  *GetEventHeaderList(){ return fEventHeaderList; };

	protected:
		TChain * fChain;
		TClonesArray  *fTrackList;
		JBaseEventHeader * fEventHeader;
		TClonesArray  *fEventHeaderList;
};

#endif

