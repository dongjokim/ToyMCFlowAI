// $Id: JBaseEventHeader.h,v v 1.0 2022/12/22  djkim Exp $

////////////////////////////////////////////////////
/*!
  \file JBaseEventHeader.h
  \brief
  \author D.J.Kim(University of Jyvaskyla)
  \email: djkim@jyu.fi
  \version $Revision: 1.5 $
  \date $Date: v 1.0 2022/12/22  $
  */
////////////////////////////////////////////////////

#ifndef JBASEEVENTHEADER_H
#define JBASEEVENTHEADER_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif
#include <TNamed.h>

class JBaseEventHeader : public TNamed {

 public:
  JBaseEventHeader();                                    // default constructor
  JBaseEventHeader(int eventid, float cent, float vtxz); // constructor
  JBaseEventHeader(const JBaseEventHeader& a);                    // copy constructor
  virtual ~JBaseEventHeader(){;}                         // destructor

  //getter
  int    GetEventID() const {return fEventID;} 
  float  GetCentrality() const {return fCentrality;}
  float  GetEventPlane() const {return fEP;}
  float  GetXVertex() const {return fVtxX;}
  float  GetYVertex() const {return fVtxY;}
  float  GetZVertex() const {return fVtxZ;}
  float  GetZVertexErr() const {return fVtxZErr;}

  //setter
  void SetEventID(int evid) {fEventID=evid;}
  void SetEventPlane(float  ep) {fEP=ep;}
  void SetCentrality(float  cent) {fCentrality=cent;}
  void SetXVertex(float vt) {fVtxX=vt;}
  void SetYVertex(float vt) {fVtxY=vt;}
  void SetZVertex(float vt) {fVtxZ=vt;}
  void SetZVertexErr(float vt) {fVtxZErr=vt;}
  void SetVertex(float x, float y, float z, float err){ fVtxX=x;fVtxY=y;fVtxZ=z;fVtxZErr=err; }

  JBaseEventHeader& operator=(const JBaseEventHeader& header);

 private:

  Int_t   fEventID;         //event id
  Double32_t fCentrality;   //centrality
  Double32_t fEP;   //centrality
  Double32_t fVtxX;         //vertex X
  Double32_t fVtxY;         //vertex Y
  Double32_t fVtxZ;         //vertex Z
  Double32_t fVtxZErr;      //vertex error

  ClassDef(JBaseEventHeader,1)

};

#endif
