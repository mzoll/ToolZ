/**
 * \file OMKeyHash.h
 *
 * (c) 2016 the IceCube Collaboration
 *
 * $Id: OMKeyHash.h 153711 2017-03-03 11:19:05Z mzoll $
 * \version $Revision: 153711 $
 * \date $Date: 2017-03-03 12:19:05 +0100 (Fri, 03 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Provide hashing of OMKeys on subsequent hash-values of natural numbers (N) aka. unsigned integers
 */

#ifndef OMKEYHASH_H
#define OMKEYHASH_H

#include "icetray/OMKey.h"
#include "dataclasses/geometry/I3Geometry.h"

//serialization
#include "ToolZ/__SERIALIZATION.h"
static const unsigned omkeyhash_version_ = 0;

/// the new Hash object for OMKeys
typedef unsigned int CompactHash;

/// failitate extraction from a general I3Map(OMkey, value)
template <class mapped>
std::set<OMKey> 
ExtractOMKeys (
  const I3Map<OMKey, mapped>& map);

/// facilitate extraction of an vector of OMKeys
//OMKeyVec_From_I3Geo
std::set<OMKey> ExtractOMKeys(
  const I3GeometryConstPtr& geo);


//forward declarations for serialization
#if SERIALIZATION_ENABLED
class CompactOMKeyHashService;

namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  void save_construct_data(
  Archive & ar,
  const CompactOMKeyHashService * t,
  const unsigned int version);

  template<class Archive>
  void load_construct_data(
    Archive & ar,
    CompactOMKeyHashService * t,
    const unsigned int version);
}};
#endif //SERIALIZATION_ENABLED

    
/**
  * A Hashing service for OMKeys, which are consecutive; resolves bidirectional
  */
class CompactOMKeyHashService {
#if SERIALIZATION_ENABLED
private:
  //serialization
  friend class SERIALIZATION_NS::access;
  
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar,
    const CompactOMKeyHashService * t,
    const unsigned int version);

  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar,
    CompactOMKeyHashService * t,
    const unsigned int version);

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
#endif //SERIALIZATION_ENABLED

private:
  ///each hash value can be translated to an OMKey; this vector is ordered and fixed on construction
  const std::vector<OMKey> omkeyvec_; //should be const but cannot be for practible reasons
  ///each OMKey has a consecutive hash value
  const std::map<OMKey, CompactHash> hashmap_; //NOTE should be const, but the overhead is just too much (serialization::load_constuct_data)

private: //internal methods (for constructors)
///create the hashmap from an omkey-vec
static
std::map<OMKey, CompactHash> HashMap_From_OMKeyVec(const std::vector<OMKey>& omkeyvec);

// private:
public:
  /// blank Constructor, NOTE avoid if possible
  CompactOMKeyHashService();
  
private: //hide this constructor
  /// initialize from list-like objects
  template <class InputIterator>
  CompactOMKeyHashService (InputIterator first, InputIterator last);
public:
  CompactOMKeyHashService(const std::set<OMKey> omkeys);
  
public: //methods  
  /// verify that all these omkeys are hashable
  bool VerifyAgainst(const std::set<OMKey>& omkeys) const;
  
  ///Get the OMKey to that hash
  OMKey OMKeyFromHash(const CompactHash hash) const;
  
  ///Get The Hash for this OMKey
  CompactHash HashFromOMKey(const OMKey omkey) const;
  
  ///hasher holds this OMKey
  bool HoldsOMKey(const OMKey omkey) const;
  
  ///hasher holds this hash
  bool HoldsHash(const CompactHash hash) const;
  
  ///size of the table
  unsigned int HashSize() const;
  
  ///returns all cached OMKeys
  std::set<OMKey> GetOMKeys() const;
};

typedef boost::shared_ptr<CompactOMKeyHashService> CompactOMKeyHashServicePtr;
typedef boost::shared_ptr<const CompactOMKeyHashService> CompactOMKeyHashServiceConstPtr;

#if SERIALIZATION_ENABLED
  SERIALIZATION_CLASS_VERSION(CompactOMKeyHashService,omkeyhash_version_);
#endif //SERIALIZATION_ENABLED


//=====================================================
//=================== IMPLEMENTATIONS =================
//=====================================================
 
#include <boost/foreach.hpp>

template <class mapped>
std::set<OMKey> 
ExtractOMKeys (
  const I3Map<OMKey, mapped>& responsemap) 
{
  std::set<OMKey> omkey_set;
  typedef I3Map<OMKey, mapped > I3MapKeyValue;
  BOOST_FOREACH(const typename I3MapKeyValue::value_type omkey_mapped, responsemap)
    omkey_set.insert(omkey_set.end(),omkey_mapped.first);
  return omkey_set;
};

#if SERIALIZATION_ENABLED
template<class Archive>
void CompactOMKeyHashService::serialize(Archive & ar, const unsigned int version)
{};

// template<class Archive>
// void CompactOMKeyHashService::save(Archive & ar, const unsigned int version) const {
//   ar << icecube::serialization::make_nvp("omkeyVector", const_cast<std::vector<OMKey> &>(omkeyvec_));
// };
// 
// template<class Archive>
// void CompactOMKeyHashService::load(Archive & ar, const unsigned int version) {
//   ar >> icecube::serialization::make_nvp("omkeyVector", const_cast<std::vector<OMKey> &>(omkeyvec_));
//   hashmap_ = HashMap_From_OMKeyVec(omkeyvec_);
// };

// //NOTE (de)serialization overrides
template<class Archive>
inline void  SERIALIZATION_NS::save_construct_data(
  Archive & ar,
  const CompactOMKeyHashService * t,
  const unsigned int version)
{
  ar << SERIALIZATION_NS::make_nvp("omkeyVector", t->omkeyvec_);
};

template<class Archive>
inline void SERIALIZATION_NS::load_construct_data(
  Archive & ar,
  CompactOMKeyHashService * t,
  const unsigned int version)
{
  std::vector<OMKey> omkeyvec;
  ar >> SERIALIZATION_NS::make_nvp("omkeyVector", omkeyvec);
#if __cplusplus >= 201103L 
  ::new(t)CompactOMKeyHashService(omkeyvec.begin(), omkeyvec.end());
#else
  ::new(t)CompactOMKeyHashService(std::set<OMKey>(omkeyvec.begin(), omkeyvec.end()));  
#endif    
};
#endif //SERIALIZATION_ENABLED

template <class InputIterator>
CompactOMKeyHashService::CompactOMKeyHashService (
  InputIterator first,
  InputIterator last) :
  omkeyvec_(first, last),
  hashmap_(HashMap_From_OMKeyVec(omkeyvec_))
{};

inline
OMKey CompactOMKeyHashService::OMKeyFromHash(const CompactHash hash) const {
  return omkeyvec_.at(hash); //NOTE this throws exception if hash is greater than the number of hashed DOMs
};
  
inline
CompactHash CompactOMKeyHashService::HashFromOMKey(const OMKey omkey) const {
  return hashmap_.at(omkey); //NOTE this throws exception if omkey does not exist in map
};

inline
bool CompactOMKeyHashService::HoldsOMKey(const OMKey omkey) const {
  return (hashmap_.find(omkey) != hashmap_.end());
};
  
inline
bool CompactOMKeyHashService::HoldsHash(const CompactHash hash) const {
  return hash<omkeyvec_.size();
};

inline 
unsigned int CompactOMKeyHashService::HashSize() const {
  return omkeyvec_.size();
};

inline
std::set<OMKey> CompactOMKeyHashService::GetOMKeys() const {
  return std::set<OMKey>(omkeyvec_.begin(),omkeyvec_.end());
};

#endif //OMKEYHASH_H
