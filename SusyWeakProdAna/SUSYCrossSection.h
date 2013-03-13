#ifndef __SUSYCROSSSECTION__
#define __SUSYCROSSSECTION__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

//#include <ext/hash_map>
//namespace std { using namespace __gnu_cxx; }


namespace SUSY
{

class CrossSectionDB
{
public:
  CrossSectionDB(const std::string& txtfilename, bool extended=false);

  // class to defined a MC process with id (or run #), name and cross-section
  class Process
  {
  public:
    Process() :
      m_id(-1), m_name(""), m_cross_section(-1.f), m_kfactor(-1.f), m_efficiency(-1.f), m_relunc(-1.f),  m_sumweight(-1.f),m_stat(-1.f) {}

      Process(int id, const std::string& name, float cross_section, float kfactor, float efficiency, float relunc,float sumweight,float stat):
      m_id(id), m_name(name), m_cross_section(cross_section), m_kfactor(kfactor), m_efficiency(efficiency), m_relunc(relunc), m_sumweight(sumweight), m_stat(stat) {}

    int ID() const { return m_id;}
    const std::string& name() const { return m_name;}
    float xsect() const { return m_cross_section;}
    float kfactor() const { return m_kfactor;}
    float efficiency() const { return m_efficiency;}
    float relunc() const { return m_relunc;}
    float sumweight() const { return m_sumweight;}
    float stat() const { return m_stat;}

  private:
    int m_id;
    std::string m_name;
    float m_cross_section;
    float m_kfactor;
    float m_efficiency;
    float m_relunc;
    float m_sumweight;
    float m_stat;
  };

  // invalid ID returns a process with ID < 0
  Process process(int id) const;

  // invalid process give < 0 result 
  float xsectTimesEff(int id) const { return rawxsect(id)*kfactor(id)*efficiency(id); }
  float rawxsect(int id) const;
  float kfactor(int id) const;
  float efficiency(int id) const;
  float rel_uncertainty(int id) const;
  float sumweight(int id) const;

  // invalid ID return "" string
  std::string name(int id) const;

  // Allow iteration over all samples, denying modification of private data
private:
  typedef std::map<int,Process> xsDB_t; // Internal data format
  bool m_extended;
public:
  typedef xsDB_t::const_iterator iterator; // External iterator format
  iterator begin() const { return m_xsectDB.begin(); }
  iterator end() const {return m_xsectDB.end(); }

private:

   xsDB_t m_xsectDB;
};

}

#endif
