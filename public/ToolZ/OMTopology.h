/**
 * \file OMTopology.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: OMTopology.h 129966 2015-03-07 20:57:35Z mzoll $
 * \version $Revision: 129966 $
 * \date $Date: 2015-03-07 21:57:35 +0100 (lör, 07 mar 2015) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Describes the different topological settings for a DOM in which it can possibly end up in
 */

#ifndef OMTOPOLOGY_H
#define OMTOPOLOGY_H

#include <cstdlib>
#include <set>
#include <vector>
#include <map>

#include "icetray/I3FrameObject.h"
#include "icetray/OMKey.h"
#include "dataclasses/I3Map.h"

//serialization
#include "icetray/serialization.h" //NOTE I3FrameObjects and derivatives will always have to use icetray/serialization

#include "ToolZ/__SERIALIZATION.h"
static const unsigned omtopology_version_ = 0;

///List of all available flags
/// NOTE might be expanded, but NEVER shortened or reshuffled without breaking old code
static const char* _flags_init[] = {
  //general positions
  "IceTop",
  "InIce",
  //DOM topologies
  "IceCube",
  "DeepCore",
  "Gen2",
  "Pingu",
  //analysis defined
  "DeepCoreFidutial", //bottom part of DeepCore + DOMs on surrounding IC strings
  "DeepCoreCap" //top part of DeepCore (VetoCap) + DOMs on surrounding IC strings
};


/** Define OMTopologies, which can be found in the detector
 * NOTE if you consider adding more flags, append them to the list
 */
class OMTopology : public I3FrameObject {
#if SERIALIZATION_ENABLED
  friend class SERIALIZATION_NS::access;  
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
#endif //SERIALIZATION_ENABLED
private:
  ///holds the flag string definitions
  static const std::vector<std::string> flags_;
  ///is the bitmap to the set flags
  int flag_holder_;
  
public: //constructor
  ///constructor
  OMTopology();
  ///constructor full initialized
  template <class InputIterator>
  OMTopology(InputIterator first, InputIterator last);
  OMTopology(const std::vector<std::string>& flags);
  OMTopology(const std::list<std::string>& flags);
  OMTopology(const std::set<std::string>& flags);
  
public: //methods  
  ///in the comparision of two OMTopologies, do they have common flags?
  bool Common(const OMTopology& o) const;
  //is the Flag set?
  bool GetFlag(const std::string& flag) const;
  // set the Flag to a value (default=true)
  void SetFlag(const std::string& flag, const bool val=true);
  // get all flags that are set by name
  std::list<std::string> GetFlags() const;
};

I3_POINTER_TYPEDEFS(OMTopology);

#if SERIALIZATION_ENABLED
  SERIALIZATION_CLASS_VERSION(OMTopology, omtopology_version_);
#endif //SERIALIZATION_ENABLED

std::ostream& operator<< (std::ostream& oss, const OMTopology& omt);

typedef I3Map<OMKey, OMTopology> OMTopologyMap;

I3_POINTER_TYPEDEFS(OMTopologyMap);

#if SERIALIZATION_ENABLED
  SERIALIZATION_CLASS_VERSION(OMTopologyMap, omtopology_version_);
#endif //SERIALIZATION_ENABLED

/** 
 * Facilitates comparision of OMTopology types on a OMTopologyMap
 * aka. tells if a DOM in a detector configuration is of ANY of the specified types
 */
class OMTopologyMap_Comparator{
private: //properties
  ///holds the Flags that should be compared
  const OMTopology omtopoflags_;
  ///holds the OMTopologyMap 
  const OMTopologyMap omtopomap_;
public: //methods
  ///constructor
  OMTopologyMap_Comparator(
    const OMTopology& omt, 
    const OMTopologyMap& omtmap);
  ///comparision of this OMKey if it has the specified flags
  bool operator() (const OMKey& omkey) const;
  ///returns a list of all OMKeys that have ANY of the specified flags
  std::list<OMKey> AllCommon() const;
};

typedef boost::shared_ptr<OMTopologyMap_Comparator> OMTopologyMap_ComparatorPtr;
typedef boost::shared_ptr<const OMTopologyMap_Comparator> OMTopologyMap_ComparatorConstPtr;


///==============================================================
///=================== IMPLEMENTATION ===========================
///==============================================================

//============== CLASS OMTopology =================
#if SERIALIZATION_ENABLED
template<class Archive>
void OMTopology::serialize(Archive & ar, const unsigned int version){
  ar & SERIALIZATION_BASE_OBJECT_NVP(I3FrameObject);
  ar & SERIALIZATION_NS::make_nvp("flag_holder", flag_holder_);
};
#endif //SERIALIZATION_ENABLED

template <class InputIterator>
OMTopology::OMTopology(InputIterator first, InputIterator last)
{
  while (first!=last) {
    const std::string& flag_name = *first;
    size_t pos=0;
    while(pos< flags_.size()) {
      if (flags_.at(pos)==flag_name) {
        const int flag_pos = 1<<pos;
        flag_holder_ |= flag_pos;
        break;
      }
      pos++;
    }
    if (pos== flags_.size())
      log_error_stream("Unknown flag name : "<<flag_name);
    first++;
  }
};

inline
bool OMTopology::Common(const OMTopology& o) const {
  return (bool)(flag_holder_ & o.flag_holder_);
};

#endif //OMTOPOLOGY_H
    
