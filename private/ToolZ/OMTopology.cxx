/**
 * \file OMTopology.cxx
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: Hive-lib.h 129966 2015-03-07 20:57:35Z mzoll $
 * \version $Revision: 129966 $
 * \date $Date: 2015-03-07 21:57:35 +0100 (lör, 07 mar 2015) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 */

#include "ToolZ/OMTopology.h"

#include <boost/foreach.hpp>

using namespace std;

//=========== CLASS OMTopology =====================

const std::vector<std::string> OMTopology::flags_(_flags_init, _flags_init+sizeof(_flags_init) / sizeof(_flags_init[0]));

OMTopology::OMTopology(): 
  flag_holder_()
{};

#if __cplusplus >= 201103L
OMTopology::OMTopology (const std::vector<std::string>& flag_names) :
  OMTopology(flag_names.begin(), flag_names.end())
{};

OMTopology::OMTopology (const std::list<std::string>& flag_names) :
  OMTopology(flag_names.begin(), flag_names.end())
{};

OMTopology::OMTopology (const std::set<std::string>& flag_names) :
  OMTopology(flag_names.begin(), flag_names.end())
{};

#else
template <class InputIterator>
int Construct_flag_holder(
  const std::vector<std::string>& flags,
  InputIterator first,
  InputIterator last)
{
  int flag_holder=0;
  while (first!=last) {
    const std::string& flag_name = *first;
    size_t pos=0;
    while(pos< flags.size()) {
      if (flags.at(pos)==flag_name) {
        const int flag_pos = 1<<pos;
        flag_holder |= flag_pos;
        break;
      }
      pos++;
    }
    if (pos== flags.size())
      log_error_stream("Unknown flag name : "<<flag_name);
    first++;
  }
  return flag_holder;
};

OMTopology::OMTopology (const std::vector<std::string>& flag_names) :
  flag_holder_(Construct_flag_holder(flags_, flag_names.begin(), flag_names.end()))
{};

OMTopology::OMTopology (const std::list<std::string>& flag_names) :
  flag_holder_(Construct_flag_holder(flags_, flag_names.begin(), flag_names.end()))
{};

OMTopology::OMTopology (const std::set<std::string>& flag_names) :
  flag_holder_(Construct_flag_holder(flags_, flag_names.begin(), flag_names.end()))
{};  
#endif


bool OMTopology::GetFlag(const std::string& flag_name) const {
  size_t pos=0;
  while(pos< flags_.size()) {
    if (flags_.at(pos)==flag_name) {
      const int flag_pos = 1<<pos;
      return (bool)(flag_holder_ & flag_pos);
    }
    pos++;
  }
  //if (pos == flags_.size()) //NOTE guarantied because of early return
  log_error_stream("Unknown flag name : "<<flag_name);
  return false;
};

void OMTopology::SetFlag(const std::string& flag_name, const bool val) {
  size_t pos=0;
  while(pos< flags_.size()) {
    if (flags_.at(pos)==flag_name) {
      const int flag_pos = 1<<pos;
      if (val)
        flag_holder_ |= flag_pos; //bitwise or
      else
        flag_holder_ ^= flag_pos; //bitwise exclusive or
      return;
    }
    pos++;
  }
  log_error_stream("Unknown flag name : "<<flag_name);
};

std::list<std::string> OMTopology::GetFlags() const {
  std::list<std::string> flags;
  size_t pos=0;
  int flag_pos=1;
  while(pos< flags_.size()) {
    if ( flag_holder_ & flag_pos )
      flags.push_back(flags_.at(pos));
    pos++;
    flag_pos= flag_pos<<1;
  }
  return flags;
}

std::ostream& operator<< (std::ostream& oss, const OMTopology& omt) {
  std::list<std::string> flags = omt.GetFlags();
  oss << "OMTopology -- Flags : ";
  BOOST_FOREACH(const string& str, flags)
    oss << str << " ";
  return oss;
}

#if SERIALIZATION_ENABLED
  I3_SERIALIZABLE(OMTopology);
  I3_SERIALIZABLE(OMTopologyMap);
#endif //SERIALIZATION_ENABLED


//=============== CLASS OMTopologyMap_Comparator ======
OMTopologyMap_Comparator::OMTopologyMap_Comparator(
  const OMTopology& omt, 
  const OMTopologyMap& omtmap):
  omtopoflags_(omt),
  omtopomap_(omtmap) 
  {};
  
bool OMTopologyMap_Comparator::operator() (const OMKey& omkey) const {
  return omtopoflags_.Common(omtopomap_.at(omkey));
};

std::list<OMKey> OMTopologyMap_Comparator::AllCommon() const {
  std::list<OMKey> l;
  BOOST_FOREACH(const OMTopologyMap::value_type& omkey_omt, omtopomap_) {
    if (omtopoflags_.Common(omkey_omt.second))
      l.push_back(omkey_omt.first);
  }
  return l;
};



