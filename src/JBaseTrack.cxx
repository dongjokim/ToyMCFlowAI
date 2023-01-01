// $Id: JBaseTrack.cxx,v 2022/12/22 djkim Exp $
////////////////////////////////////////////////////
/*!
  \file JBaseTrack.cxx
  \brief
  \author D.J.Kim  (University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.0 $
  \date $Date: 2022/12/22 $
  */
////////////////////////////////////////////////////

#include "JBaseTrack.h"
#include <TString.h>

//______________________________________________________________________________
JBaseTrack::JBaseTrack():
    fID(-1),
    fLabel(-9999), 
    fParticleType(-1), 
    fCharge(0), 
    fStatus(0)
{
  // constructor
}

//_____________________________________________________________
JBaseTrack::JBaseTrack(float px,float py, float pz, float e, Int_t id, Short_t ptype, Char_t charge):
    TLorentzVector( px, py, pz, e ), 
    fID(id),
    fLabel(-9999),
    fParticleType(ptype), 
    fCharge(charge), 
    fStatus(0)
{
  // constructor
}

//_____________________________________________________________
JBaseTrack::JBaseTrack(const JBaseTrack& a):
    TLorentzVector  (a), 
    fID             (a.fID),
    fLabel          (a.fLabel),
    fParticleType   ( a.fParticleType ), 
    fCharge         ( a.fCharge ), 
    fStatus         ( a.fStatus ) 
{
  //copy constructor
}

//_____________________________________________________________
JBaseTrack::JBaseTrack(const TLorentzVector& a):
    TLorentzVector  (a),
    fID             ( -1 ),
    fLabel          ( -9999 ),
    fParticleType   ( -1 ),
    fCharge         ( 0 ),
    fStatus         ( 0 )
{
  //copy constructor
}
//_____________________________________________________________
JBaseTrack& JBaseTrack::operator=(const JBaseTrack& trk){
  //operator =  
  if(this != &trk){
    TLorentzVector::operator=(trk);
    fID           = trk.fID;
    fLabel        = trk.fLabel;
    fParticleType = trk.fParticleType;
    fCharge       = trk.fCharge;
    fStatus       = trk.fStatus;
  }
  return *this;
}

//_____________________________________________________________
void JBaseTrack::Print(Option_t *option) const{
  //object print out
  std::cout<<Form("(ID,Type,Charge)=(%d, %d, %d)" , 
                  fID, fParticleType, fCharge);
  TLorentzVector::Print();
  cout<<"ID ="<<fID <<endl; 
  cout<<"fLabel="<<fLabel <<endl; 
  cout<<"fParticleType="<<fParticleType <<endl; 
  cout<<"fCharge="<<fCharge <<endl; 
  cout<<"fStatus="<<fStatus <<endl; 
}

ClassImp(JBaseTrack)

