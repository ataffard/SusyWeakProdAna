#include "SusyWeakProdAna/SUSYCrossSection.h"

SUSY::CrossSectionDB::CrossSectionDB(const std::string& txtfilename,bool extended)
{
  using namespace std;
  m_extended=extended;
  string line;
  ifstream in(txtfilename.c_str());
  bool first = true;
  while ( getline(in,line) ) 
  {
    // skip first line
    if (first) {
      first = false;
      continue;
    }
    // skip leading blanks (in case there are some in front of a #
    if ( !line.empty() )
    {
      while ( line[0] == ' ' ) line.erase(0,1);
    }
    // accept lines starting with # as a comment line
    if ( !line.empty() && line[0] != '#' )
    {
      stringstream is(line);
      int id;
      string name;
      float xsect, kfactor, efficiency, relunc;
      float sumweight=-1,stat=-1;
      is >> id >> name >> xsect >> kfactor >> efficiency >> relunc;
      if(m_extended==true)
	{
	  is >>sumweight>>stat;
	}
      //cout << " Process: " << name << "  " << id << " " << xsect << endl; 
      m_xsectDB[id] = Process(id,name,xsect,kfactor,efficiency,relunc,sumweight,stat);
    }
  }

}

SUSY::CrossSectionDB::Process SUSY::CrossSectionDB::process(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return Process();
  else return pos->second;
}

float SUSY::CrossSectionDB::rawxsect(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return -1.f;
  else return pos->second.xsect();
}

float SUSY::CrossSectionDB::kfactor(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return -1.f;
  else return pos->second.kfactor();
}

float SUSY::CrossSectionDB::efficiency(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return -1.f;
  else return pos->second.efficiency();
}

float SUSY::CrossSectionDB::sumweight(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return -1.f;
  else return pos->second.sumweight();
}
float SUSY::CrossSectionDB::rel_uncertainty(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return -1.f;
  else return pos->second.relunc();
}

std::string SUSY::CrossSectionDB::name(int id) const
{
  xsDB_t::const_iterator pos = m_xsectDB.find(id);
  if ( pos == m_xsectDB.end() ) return "";
  else return pos->second.name();
}

