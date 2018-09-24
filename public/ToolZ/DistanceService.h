/**
 * \file DistanceService.h
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

#ifndef DISTANCESERVICE_H
#define DISTANCESERVICE_H

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/PositionService.h"
#include "ToolZ/IndexMatrix.h"

#include <boost/foreach.hpp>

#include "ToolZ/__SERIALIZATION.h"
static const unsigned distanceservice_version_ = 0;

//============ CLASS DistanceService ==========

//forward declarations for Serialization
#if SERIALIZATION_ENABLED
class DistanceService;

namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  void save_construct_data(
    Archive & ar, const DistanceService * t, const unsigned int file_version);
  
  template<class Archive>
  void load_construct_data(
    Archive & ar, DistanceService * t, const unsigned int file_version);
}};
#endif //SERIALIZATION_ENABLED


/**
  * Hashes the position and the distances between DOMs in a dynamic way.
  * iternal storing is as a uint in meters, so there will be an error of about 0.1m
  */
class DistanceService {
#if SERIALIZATION_ENABLED
  friend class SERIALIZATION_NS::access;
  
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const DistanceService * t, const unsigned int file_version);
  
  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, DistanceService * t, const unsigned int file_version);

  template<class Archive>
  void serialize(Archive & ar, const unsigned int file_version);
#endif //SERIALIZATION_ENABLED
private: //parameters
  /// a hasher for translation of OMKeys to indeces (hashes)
  const CompactOMKeyHashServiceConstPtr hasher_;
  /// a position service 
  const PositionServiceConstPtr posService_;
  
private: //property  
  /// holds the precashed distances
  mutable indexmatrix::SymmetricIndexMatrix<uint16_t, std::vector<uint16_t> > hashedDist_;
  
private: 
  ///for the construction of the hashedPositions
  std::vector<I3Position> ConstructHashedPositions (  
    const I3GeometryConstPtr& geo) const ;
  
public:
  /// constructor
  DistanceService(
    const PositionServiceConstPtr& posService);
  
  /// Hash all the Distances at once
  void HashAllDistances() const;
  
  /// Get the distance between these two DOMs by hashed index, compute if neccessary
  /// \param a hashed DOMindex
  /// \param b hashed DOMindex
  double GetDistance(const CompactHash a,
                     const CompactHash b) const;
  /// Get the distance between these two DOMs by omkey, compute if neccessary
  /// \param a OMKey
  /// \param b OMKey
  double GetDistance(const OMKey& a,
                     const OMKey& b) const;
  
  /// Get the internal Hasher
  CompactOMKeyHashServiceConstPtr GetHashService() const;
  /// Get the internal PositionService
  PositionServiceConstPtr GetPosService() const;
  
  ///Verify the DistService against this geometry
  /// checks if all hashed Positions are the same
  bool VerifyAgainst(const I3OMGeoMap& omgeo) const;
};

typedef boost::shared_ptr<DistanceService> DistanceServicePtr;
typedef boost::shared_ptr<const DistanceService> DistanceServiceConstPtr;


//========================================================
//==================== IMPLERMENTATIONS ==================
//========================================================

#if SERIALIZATION_ENABLED
template<class Archive>
void DistanceService::serialize(Archive & ar, const unsigned int version)
{};

// //NOTE (de)serialization overrides
namespace SERIALIZATION_NS_BASE { namespace serialization {
template<class Archive>
inline void save_construct_data(
  Archive & ar, const DistanceService * t, const unsigned int file_version)
{
  ar << SERIALIZATION_NS::make_nvp("posService", t->posService_);
};

template<class Archive>
inline void load_construct_data(
  Archive & ar, DistanceService * t, const unsigned int file_version)
{
  // retrieve data from archive required to construct new instance
  PositionServiceConstPtr posService;
  ar >> SERIALIZATION_NS::make_nvp("posService", posService);
  // invoke inplace constructor to initialize instance of my_class
  ::new(t)DistanceService(posService);
};
}} // namespace ...
#endif //SERIALIZATION_ENABLED

inline
double DistanceService::GetDistance(
  const OMKey& a,
  const OMKey& b) const
{
  return GetDistance( hasher_->HashFromOMKey(a), hasher_->HashFromOMKey(b));
};

inline
CompactOMKeyHashServiceConstPtr 
DistanceService::GetHashService() const
  { return hasher_; };
  
inline
PositionServiceConstPtr
DistanceService::GetPosService() const
  { return posService_; };

#endif // DISTANCESERVICE_H
