/**
 * \file HashedGeometry.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Concerts an OMGeomtry into a minimal table of typically requested info
 */

#ifndef HASHEDGEOMETRY_H
#define HASHEDGEOMETRY_H

#include "ToolZ/__SERIALIZATION.h"

#include "dataclasses/geometry/I3Geometry.h"

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/PositionService.h"
#include "ToolZ/DistanceService.h"

static const unsigned hashedgeometry_version_ = 0;

class HashedGeometry {
#if SERIALIZATION_ENABLED
private:
  friend class SERIALIZATION_NS::access;
  
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar,
    const HashedGeometry * t,
    const unsigned int version);

  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar,
    HashedGeometry * t,
    const unsigned int version);

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  
  ///reconstruct from sparse information
  HashedGeometry (const PositionServiceConstPtr& posService);
#endif //SERIALIZATION_ENABLED
private: //properties
  const CompactOMKeyHashServiceConstPtr hashService_;
  const PositionServiceConstPtr posService_;
  const DistanceServiceConstPtr distService_;
public: //constructor
  /// constructor: hash this omgeo
  HashedGeometry(const I3OMGeoMap& omgeo);
  
  /// constructor: hash this omgeo, but only the specified OMKeys
  HashedGeometry(
    const I3OMGeoMap& omgeo,
    const std::set<OMKey> omkeys);
public: //getters
  CompactOMKeyHashServiceConstPtr GetHashService() const;
  PositionServiceConstPtr GetPosService() const;
  DistanceServiceConstPtr GetDistService() const;
};

typedef boost::shared_ptr<HashedGeometry> HashedGeometryPtr;
typedef boost::shared_ptr<const HashedGeometry> HashedGeometryConstPtr;

//=================== IMPLEMENTATION ===================

#if SERIALIZATION_ENABLED
template<class Archive>
void HashedGeometry::serialize(Archive & ar, const unsigned int version)
{};

template<class Archive>
inline void SERIALIZATION_NS::save_construct_data(
  Archive & ar,
  const HashedGeometry * t,
  const unsigned int version)
{
  const PositionService* p = (t->posService_).get();
  ar << SERIALIZATION_NS::make_nvp("posService", p);
};

template<class Archive>
inline void SERIALIZATION_NS::load_construct_data(
  Archive & ar,
  HashedGeometry * t,
  const unsigned int version)
{
  PositionService* posService;
  ar >> SERIALIZATION_NS::make_nvp("posService", posService);
  ::new(t)HashedGeometry(boost::make_shared<const PositionService>(*posService));
};
#endif //SERIALIZATION_ENABLED

inline
CompactOMKeyHashServiceConstPtr HashedGeometry::GetHashService() const
{ return hashService_; };

inline
PositionServiceConstPtr HashedGeometry::GetPosService() const
{ return posService_; };

inline
DistanceServiceConstPtr HashedGeometry::GetDistService() const
{ return distService_; };

#endif //HASHEDGEOMETRY_H


