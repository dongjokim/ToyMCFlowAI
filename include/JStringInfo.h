#ifndef JSTRINGINFO_H
#define JSTRINGINFO_H
#include <iostream>
#include <TString.h>
#include <TRegexp.h>
#include <TPRegexp.h>
#include <TObjArray.h>
#include <THashList.h>
#include <TObjString.h>


class JStringInfo {
public:
  JStringInfo( TString s, bool ignorecase=kTRUE):fValue(s),fIgnoreCase(ignorecase){
    ParseString();
  }
  void SetValue( TString s ){
    fValue = s;
    fHL.Clear();  // Clear the hash list in which the key values are stored
    ParseString();  // After the new value is set, parse it so that it can be used
  }
  TString GetValue(){ return fValue; }
  TString GetString(){ return fValue; }
  TObjArray * Find(TString key){
    if( fIgnoreCase ) key.ToLower();
    return (TObjArray*) fHL.FindObject(key);
  }
  
  //**** Getter
  bool IsDefined( TString key ){ return !GetS(key).Contains("ERROR:NoKey"); }
  bool IsFine( TString key, int index=0 ){ return !GetS(key,index).Contains("ERROR"); }
  
  //**** NOTE : IsIn, IsAround, IsOut, IsNotAround : if value is NOT FINE then just return "FALSE"
  bool IsIn( TString key, int i, float x0, float x1 ){float v=GetF(key,i);return IsFine(key,i) && v>=x0 && v<x1;}
  bool IsAround( TString key, int i, float x, float tol=1e-4 ){return IsIn( key, i, x-tol, x+tol );}
  bool IsOut( TString key, int i, float x0, float x1 ){ return IsFine(key,i) && GetF(key,i)<x0 && GetF(key,i)>=x1; }
  bool IsNotAround( TString key, int i,  float x, float tol=1e-4){return IsOut( key, i, x-tol, x+tol );}
  
  //**** BASIC Getter
  TString GetS(TString key, int index=0){
    TObjArray * oa = Find(key);
    if( !oa ) return "ERROR:NoKey";
    if( index >= oa->GetEntries() ) return "Error:ExcessIndex";
    TObjString *os = (TObjString*)oa->At(index);
    if( !os ) return "ERROR:NoItem";
    return os->String();
  }
  
  //**** TODO defalut value when value is not defined or out of array?
  //**** Get by Int
  int GetI(TString key, int i=0){return GetS(key,i).Atoi();}
  //**** Get by Float
  float GetF(TString key, int i=0){return GetS(key,i).Atof();}
  
  int GetN(TString key){
    TObjArray * oa = Find(key);
    if( !oa )  return -1;
    return oa->GetEntries();
  }
  
  void ParseString(){
    TPMERegexp token(":");
    TPMERegexp keyval("=");
    TPMERegexp item(",");
    
    int nToken = token.Split(fValue);
    for( int i=0;i<nToken;i++ ){
      if( token[i].BeginsWith("#") ) continue;
      int nKeyval = keyval.Split( token[i],2 );
      if( nKeyval !=2 ) continue;
      TString key = keyval[0];
      if( fIgnoreCase ) key.ToLower();
      key.ReplaceAll(" ","");
      key.ReplaceAll("\n","");
      key.ReplaceAll("\t","");
      TObjArray * os = (TObjArray*)fHL.FindObject(key);
      if(!os) {
        os = new TObjArray;
        os->SetName(key);
        os->SetOwner(kTRUE);
        fHL.AddLast(os);
      }
      os->Clear();
      int nItem = item.Split(keyval[1]);
      for( int ii=0;ii<nItem;ii++ ){
        os->Add( new TObjString(item[ii]) );
      }
    }
    
  }
  
  //**** TODO under development still
  void Print(){
    TIter nextKey(&fHL);
    TObjArray * os = NULL;
    while( (os = (TObjArray*) nextKey()) ){
      std::cout<<os->GetName()<<"\t=\t";
      TIter nextItem(os);
      TObjString * s = NULL;
      while( (s=(TObjString*)nextItem()) ){
        cout<<s->String()<<" ,  ";
      }
      cout<<endl;
    }
  }
  
private:
  TString fValue;
  bool    fIgnoreCase;
  THashList fHL;
  
};

#endif
