/**************************************************************************
* provided "as is" without express or implied warranty.                  *
**************************************************************************/

// $Id: JBaseEventHeader.cxx,v v 1.0 2022/12/22  djkim Exp $

////////////////////////////////////////////////////
//
//  \file JBaseEventHeader.cc
//  \brief
//  \author D.J.Kim(University of Jyvaskyla) email: djkim@jyu.fi
//  \version $Revision: 1.0 $
//  \date $Date: v 1.0 2022/12/22  $
//
// Base class for event headers
////////////////////////////////////////////////////

#include <TNamed.h>
#include "JBaseEventHeader.h"

ClassImp(JBaseEventHeader)

//______________________________________________________________________________
JBaseEventHeader::JBaseEventHeader(): 
  TNamed("JBaseEventHeader", ""), 
  fEventID(-999),              
  fCentrality(-999),
  fVtxX(9999), 
  fVtxY(9999), 
  fVtxZ(9999), 
  fVtxZErr(-999)
{
  // default constructor
}

//______________________________________________________________________________
JBaseEventHeader::JBaseEventHeader(int eventid, float cent, float vtxz): 
  TNamed("JBaseEventHeader", ""), 
  fEventID(eventid),              
  fCentrality(cent),
  fVtxX(-999),
  fVtxY(-999),
  fVtxZ(vtxz),
  fVtxZErr(-999)
{
  //constructor
}
//______________________________________________________________________________
JBaseEventHeader::JBaseEventHeader(const JBaseEventHeader& a):
  TNamed(a),
  fEventID(a.fEventID),
  fCentrality(a.fCentrality),
  fVtxX(a.fVtxX),
  fVtxY(a.fVtxY),
  fVtxZ(a.fVtxZ),
  fVtxZErr(a.fVtxZErr)
{
  //copy constructor
}

//______________________________________________________________________________
JBaseEventHeader&  JBaseEventHeader::operator=(const JBaseEventHeader& header){
  //operator=  
  if(this != &header){
    TNamed::operator=(header);
    fEventID    = header.fEventID;
    fCentrality = header.fCentrality;
    fVtxX       = header.fVtxX;
    fVtxY       = header.fVtxY;
    fVtxZ       = header.fVtxZ;
    fVtxZErr   = header.fVtxZErr;
  }

  return *this;
}

