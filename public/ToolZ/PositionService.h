/**
 * \file PositionService.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Stores the position and distances between all hashed DOMs
 */

#ifndef POSITIONSERVICE_H
#define POSITIONSERVICE_H

#include "ToolZ/OMKeyHash.h"

#include "dataclasses/geometry/I3Geometry.h"

#include "ToolZ/__SERIALIZATION.h"
static const unsigned positionservice_version_ = 0;

//============ CLASS PositionService ==========

//forward defs
class PositionService;

namespace SERIALIZATION_NS_BASE {
  namespace serialization {
    template<class Archive>
    void save_construct_data(
      Archive & ar, const PositionService * t, const unsigned int file_version);
    
    template<class Archive>
    void load_construct_data(
      Archive & ar, PositionService * t, const unsigned int file_version);
}};

/**
 * Hashes the position of DOMs from an I3Geometry
 */
class PositionService {
#if SERIALIZATION_ENABLED
private:
  friend class SERIALIZATION_NS::access;
  
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const PositionService * t, const unsigned int file_version);
  
  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, PositionService * t, const unsigned int file_version);

  template<class Archive>
  void serialize(Archive & ar, const unsigned int file_version);
  
  //construct from sparse information only in the serialization
  PositionService(
    const CompactOMKeyHashServiceConstPtr& hasher,
    const std::vector<I3Position>& hashedPosition);
#endif //SERIALIZATION_ENABLED
  
private: //parameters
  /// a hasher for translation of OMKeys to indeces (hashes)
  const CompactOMKeyHashServiceConstPtr hasher_;
  
private: //property  
  /// holds precashed positions
  const std::vector<I3Position> hashedPosition_;
  
private: 
  ///for the construction of the hashedPositions
  std::vector<I3Position> ConstructHashedPositions (  
    const I3OMGeoMap& omgeo) const;
  
public:
  /// constructor
  /// \param geo pointer to an I3Geometry used for Position retrieval
  /// \param hasher pointer to the hashing service; will be created if not exists
  PositionService(
    const I3OMGeoMap& omgeo,
    CompactOMKeyHashServiceConstPtr& hasher);
  
  ///constructor: NOTE compiler is too stupid to accept the above
  PositionService(
    const I3OMGeoMap& omgeo,
    const CompactOMKeyHashServiceConstPtr& hasher);
  
public:
  /// Get the Position of the DOM hased under index a; hash if neccessary
  /// \param a the simple index of that DOM
  const I3Position& GetPosition(const CompactHash a) const;
  /// Get the Position of the DOM at this omkey
  /// \param a the simple index of that DOM
  const I3Position& GetPosition(const OMKey& a) const;
  
  /// Get the internal Hasher
  CompactOMKeyHashServiceConstPtr GetHashService() const;
  
  ///Verify the DistService against this geometry
  /// checks if all hashed Positions are the same
  bool VerifyAgainst(const I3OMGeoMap& omgeo) const;
};

typedef boost::shared_ptr<PositionService> PositionServicePtr;
typedef boost::shared_ptr<const PositionService> PositionServiceConstPtr;


//========================================================
//==================== IMPLERMENTATIONS ==================
//========================================================

#if SERIALIZATION_ENABLED
template<class Archive>
void PositionService::serialize(Archive & ar, const unsigned int version)
{ };

// //NOTE (de)serialization overrides
//   namespace SERIALIZATION_NS_BASE { namespace serialization {
template<class Archive>
void SERIALIZATION_NS::save_construct_data(
  Archive & ar, const PositionService * t, const unsigned int file_version)
{
  ar << SERIALIZATION_NS::make_nvp("hasher", t->hasher_);
  ar << SERIALIZATION_NS::make_nvp("hashedPosition", t->hashedPosition_); 
};

template<class Archive>
void SERIALIZATION_NS::load_construct_data(
  Archive & ar, PositionService * t, const unsigned int file_version)
{
    // retrieve data from archive required to construct new instance
  CompactOMKeyHashServiceConstPtr hasher;
  ar >> SERIALIZATION_NS::make_nvp("hasher", hasher);
  std::vector<I3Position> hashedPosition;
  ar >> SERIALIZATION_NS::make_nvp("hashedPosition", hashedPosition); 
  // invoke inplace constructor to initialize instance of my_class
  ::new(t)PositionService(hasher, hashedPosition);
};
// }} // namespace 
#endif //SERIALIZATION_ENABLED

inline
const I3Position& PositionService::GetPosition(const CompactHash a) const {
  return hashedPosition_[a];
};

inline
const I3Position& PositionService::GetPosition(const OMKey& a) const
   {return GetPosition(hasher_->HashFromOMKey(a));};
 
inline
CompactOMKeyHashServiceConstPtr 
PositionService::GetHashService() const
  { return hasher_; };

#endif // POSITIONSERVICE_H
