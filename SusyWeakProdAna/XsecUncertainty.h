// emacs -*- C++ -*-
#ifndef SUSY_WH_XSECUNCERTAINTY_H
#define SUSY_WH_XSECUNCERTAINTY_H

#include <string>
#include "SusyWeakProdAna/DsidGroups.h"

/*!
  A class retrieve the xsec uncertainty described in sec.7.2 of the SS WH note.
  
  The uncertainties are given by macro-groups, but when we run we only know the dsid.
  The dsid-group association is produced by the script ....
  A 100% uncertainty is used for dsid for which we cannod determine the group.

  davide.gerbaudo@gmail.com
  Mar 2014
*/
class XsecUncertainty {
public:
  //enum McGroup {
  typedef enum{
        kUnknown,
        kTtbar,
        kSingleTop,
        kTtbarW,
        kTtbarZ,
        kWw,
        kWz,
        kZz,
        kWwjj,
        kTriboson,
        kZjets,
        kHiggs
    }McGroup;
public:
    XsecUncertainty() : m_keepQuiet(false), m_group(kUnknown), m_uncertainty(1.0) {}
    bool determineGroup(const int dsid);
    float fractionalUncertainty() const;
    McGroup groupFromDsid(int dsid);
    std::string str() const;
    bool m_keepQuiet;
public:
    static std::string McGroup2str(const McGroup g);
private:
    McGroup m_group;
    float m_uncertainty;
}; // end XsecUncertainty


#endif // end include guard
