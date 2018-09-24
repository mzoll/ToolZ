/**
 * \file PositionService.cxx
 *
 * copyright (c) 2011
 * the IceCube Collaboration
 * $Id: IceHiveHelpers.cxx 144968 2016-04-20 22:52:24Z mzoll $
 * @date $Date: 2013-10-22$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include "ToolZ/PositionService.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>
#include <vector>

//=================== CLASS PositionService ===========

PositionService::PositionService(
  const I3OMGeoMap& omgeo,
  CompactOMKeyHashServiceConstPtr& hasher)
: hasher_(hasher ? hasher : boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(omgeo))),
  hashedPosition_(ConstructHashedPositions(omgeo))
{
  if (!hasher)
    hasher = hasher_;
};

PositionService::PositionService(
  const I3OMGeoMap& omgeo,
  const CompactOMKeyHashServiceConstPtr& hasher)
: hasher_(hasher),
  hashedPosition_(ConstructHashedPositions(omgeo))
{};

#if SERIALIZATION_ENABLED
  PositionService::PositionService(
    const CompactOMKeyHashServiceConstPtr& hasher,
    const std::vector<I3Position>& hashedPosition)
  : hasher_(hasher),
    hashedPosition_(hashedPosition)
  {};
#endif //SERIALIZATION_ENABLED
    
std::vector<I3Position> PositionService::ConstructHashedPositions (
  const I3OMGeoMap& omgeo) const 
{
  std::vector<I3Position> hashedPosition;
  BOOST_FOREACH(const OMKey& omkey, hasher_->GetOMKeys()) {
    hashedPosition.push_back(omgeo.at(omkey).position);
  }
  return hashedPosition;
};

bool PositionService::VerifyAgainst(
  const I3OMGeoMap& omgeo) const 
{
  for (uint64_t i=0; i<hashedPosition_.size(); i++) {
    const OMKey omkey = hasher_->OMKeyFromHash(i);
    const I3OMGeoMap::const_iterator geo_entry = omgeo.find(omkey);
    if (geo_entry == omgeo.end()) {
      log_error("Do not verify against each other");
      return false;
    }
    if (hashedPosition_.at(i) != omgeo.at(omkey).position) {
      log_error("Do not verify against each other");
      return false;
    }
  }
  return true;
}

#if SERIALIZATION_ENABLED
  I3_SERIALIZABLE(PositionService);
#endif
