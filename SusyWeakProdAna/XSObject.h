#ifndef XSObject_h
#define XSObject_h

#include "TObject.h"

//-------------------------------------------//
// Object to store relevant info for sample
//-------------------------------------------//
class XSObject : public TObject
{

 public:

  // ---- Constructor ---- // 
  XSObject(float _mc1n2, float _mn1, float _xs, float _xssys){
    mc1n2 = _mc1n2;
    mn1   = _mn1;
    xs    = _xs;
    xssys = _xssys;
  };

  XSObject(){
    mc1n2 = 0;
    mn1   = 0;
    xs    = 0.;
    xssys = 0.;
  };

  // ---- Destructor ---- // 
  ~XSObject(){};

  // ---- Reset ---- //
  void reset(){
    mc1n2 = mn1 = 0;
    xs = xssys = 0;
  };

  // ---- Get Values ----//
  float getMC1N2() const { return mc1n2; };
  float getMN1()   const { return mn1; };
  float getXS()    const { return xs; };
  float getXSSys() const { return xssys; };

  XSObject& setMc1mn2(float val) { mc1n2=val; return *this; }
  XSObject& setMn1   (float val) { mn1=val;   return *this; }
  XSObject& setXs    (float val) { xs=val;    return *this; }
  XSObject& setXssys (float val) { xssys=val; return *this; }
  
  ClassDef(XSObject,2);

 private:

  float mc1n2;   // Mass of Chargino 1 Neutralino 2
  float mn1;     // Mass of Neutralino 1
  float xs;    // Cross section
  float xssys; // Cross section uncertainty


};



#endif
