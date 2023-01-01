// $Id: JTreeDataManager.cxx,v 1.0 2022/12/22 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JTreeDataManager.cxx
  \brief
  \author D.J.Kim (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 1.0 2022/12/22 $
 */
////////////////////////////////////////////////////


#include  "JTreeDataManager.h"


//______________________________________________________________________________
JTreeDataManager::JTreeDataManager():
	fChain(NULL), 
	fTrackList(NULL), 
	fEventHeader(NULL),
	fEventHeaderList(NULL)
{
	// constructor
	fChain = new TChain("jTree");
}

//______________________________________________________________________________
JTreeDataManager::~JTreeDataManager(){
	if( fChain ) delete fChain;
}

//______________________________________________________________________________

bool JTreeDataManager::IsGoodEvent(){    
	return true; 
}

//______________________________________________________________________________
void JTreeDataManager::RegisterList(TClonesArray* listToFill, TClonesArray* listFromToFill) { 

	int noIn    = fTrackList->GetEntriesFast();
	int counter = 0;
	for(int ii=0;ii<noIn;ii++){ // loop for all tracks 
		JBaseTrack *cgl = (JBaseTrack*)fTrackList->At(ii);
		if(1
		  ) {
			cgl->SetID(ii);
			new ((*listToFill)[counter++]) JBaseTrack(*cgl);
		}
	}
}

//______________________________________________________________________________
void JTreeDataManager::ChainInputStream(const char* infileList){

	// read root nano data files in a list  
	char inFile[200];
	ifstream infiles(infileList);
	while ( infiles >> inFile){ 
		fChain->Add(inFile);
	}
	//fChain->Print();

	if(fChain->GetEntriesFast()<=0){
		cout<<"Empty chain from "<<infileList<<endl;
		exit(0);
	}
	cout<<Form("there are %d events.\n", (int)fChain->GetEntries())<<endl;

	// Load Branch
	fChain->SetBranchAddress("JTrackList", &fTrackList);
	fChain->SetBranchAddress("JEventHeaderList", &fEventHeaderList);
}


//______________________________________________________________________________
int JTreeDataManager::LoadEvent(int ievt){

	//clear clones array and counters
	//load the new event
	int v =  ((TTree*)fChain)->GetEntry(ievt);
    //fEventHeader = dynamic_cast<AliJBaseEventHeader*>(fEventHeaderList->At(0));

	return v;
}


