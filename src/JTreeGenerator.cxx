#include "JTreeGenerator.h"
#include "JBaseEventHeader.h"
#include "JBaseTrack.h"

// Define some simple structures
typedef struct {Double_t phi,eta,pt,mass,E,correction;} JTRACKS;
typedef struct {
    Int_t runnumber,event,ntrack,icent;
    Double_t eCM,psi_2,psi_3,v_2,v_3;
} JEVENT;
JTRACKS jtracks;
JEVENT jevent;


//______________________________________________________________________________
JTreeGenerator::JTreeGenerator(TFile *fin){   // constructor
    fOutRoot = fin;
}

void JTreeGenerator::GenerateTree()
{
    if (!fOutRoot) {
        cout << "Error: Cannot open input file " << fOutRoot << endl;
        return;
    }
    fOutRoot->cd();
	event = new TClonesArray("JBaseEventHeader",10000);
	tracks = new TClonesArray("JBaseTrack",10000);
    jTree = new TTree("jTree","Tree from ToyMC");
    jTree->Branch("JTrackList",&tracks);
    jTree->Branch("JEventHeaderList",&event);


	vTree = new TTree("vTree","vTree from ToyMC");
    vTree->Branch("phi",&jtracks.phi,"phi/D");
    vTree->Branch("eta",&jtracks.eta,"eta/D");
    vTree->Branch("pt",&jtracks.pt,"pt/D");
    vTree->Branch("mass",&jtracks.mass,"mass/D");
    vTree->Branch("E",&jtracks.E,"E/D");
    vTree->Branch("correction",&jtracks.correction,"correction/D");
    vTree->Branch("runnumber",&jevent.event,"runnumber/I");
    vTree->Branch("event",&jevent.event,"event/I");
    vTree->Branch("ntrack",&jevent.ntrack,"ntrack/I");
    vTree->Branch("icent",&jevent.icent,"icent/I");
    vTree->Branch("eCM",&jevent.eCM,"eCM/D");
    vTree->Branch("psi_2",&jevent.psi_2,"psi_2/D");
    vTree->Branch("psi_3",&jevent.psi_3,"psi_3/D");
    vTree->Branch("v_2",&jevent.v_2,"v_2/D");
    vTree->Branch("v_3",&jevent.v_3,"v_3/D");
}

void JTreeGenerator::AddEvent(Int_t random_seed, int iEvent, double cent, int ic, int Ntrk, double Psi_n[3], double v2, double v3)
{
    //new ( (*event)[iEvent] )JBaseEventHeader(iEvent,cent,0.1);
    JBaseEventHeader *hdr = new( (*event)[event->GetEntriesFast()] ) JBaseEventHeader;
    hdr->SetEventID(iEvent);
    hdr->SetCentrality(cent);

	jevent.runnumber = random_seed; // just for MC
	jevent.event = iEvent;
	jevent.ntrack = Ntrk;
	jevent.icent = ic;
	jevent.eCM = TMath::Log(5020.);
	jevent.psi_2 = Psi_n[1];
	jevent.psi_3 = Psi_n[2];
	jevent.v_2 = v2;
	jevent.v_3 = v3;
}

void JTreeGenerator::AddTrack(int i, double px, double py, double pz, double E,double phi, double eta, double pt, double mass)
{
    new ( (*tracks)[i] )TLorentzVector(px, py, pz, E); // No fill needed every track
	jtracks.phi = phi;
	jtracks.eta = eta;
	jtracks.pt  = pt;
	jtracks.E   = E;
	jtracks.mass = mass;
	jtracks.correction = 1.0;
    vTree->Fill(); // Needed to fill vTree 
}

void JTreeGenerator::FillTree()
{    
    if (!fOutRoot) {
        cout << "Error: Cannot open input file " << fOutRoot << endl;
        return;
    }
    fOutRoot->cd();
    jTree->Fill();
    // vTree->Fill();
}   
void JTreeGenerator::WriteTree()
{
    if (!fOutRoot) {
        cout << "Error: Cannot open input file " << fOutRoot << endl;
        return;
    }
    fOutRoot->cd();
    jTree->Write();
    vTree->Write();
}