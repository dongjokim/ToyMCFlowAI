// $Id: JBaseTrack.h,v 1.0 2022/12/22 djkim Exp $

///////////////////////////////////////////////////
/*
   \file JBaseTrack.h
   \brief
   \author D.J.Kim(University of Jyvaskyla)
   \email: djkim@jyu.fi
   \version $Revision: 1.0 $
   \date $Date: 2022/12/22 $
   */
///////////////////////////////////////////////////

#ifndef JBASETRACK_H
#define JBASETRACK_H

#ifndef ROOT_TObject
#include <TObject.h>
#endif

#include <iostream>
#include <TLorentzVector.h>

using namespace std;

class JBaseTrack : public TLorentzVector {
    public:
        JBaseTrack();
        JBaseTrack(float px,float py, float pz, float e, Int_t id, Short_t ptype, Char_t charge); // constructor
        JBaseTrack(const JBaseTrack& a);
        JBaseTrack(const TLorentzVector & a);
        virtual ~JBaseTrack(){;}    //destructor

        TLorentzVector GetLorentzVector(){ return TLorentzVector(Px(), Py(), Pz(), E());}

        Int_t         GetID()           const { return fID;}
        Int_t         GetLabel()        const { return fLabel; }
        Short_t       GetParticleType() const { return fParticleType;}
        ULong_t       GetStatus()       const { return fStatus; }
        Short_t       GetCharge()       const { return fCharge; } 

        void SetID      (const int id){fID=id;}
        void SetLabel   (const Int_t label ){ fLabel=label; }
        void SetParticleType(const Short_t ptype){ fParticleType=ptype; }
        void SetStatus  (const ULong_t status){ fStatus=status; }
        void SetCharge  (const Char_t charge){ fCharge=charge; }

        virtual void Print(Option_t *option="") const;

        // Operators
        JBaseTrack& operator=(const JBaseTrack& trk);

    protected:
        Int_t         fID;            // Unique track ID
        Int_t         fLabel;         // Unique track label for MC-Data relation
        Short_t       fParticleType;  // ParticleType 
        Char_t        fCharge;        // track charge for real data
        ULong_t       fStatus;        // reconstruction status flags or MC status 

        ClassDef(JBaseTrack,1)
};

#endif

