/**
 * \file DistanceService.cxx
 *
 * copyright (c) 2011
 * the IceCube Collaboration
 * $Id: IceHiveHelpers.cxx 144968 2016-04-20 22:52:24Z mzoll $
 * @date $Date: 2013-10-22$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include "ToolZ/DistanceService.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>
#include <vector>

using namespace indexmatrix;

//=================== CLASS DistanceService ===========

DistanceService::DistanceService(
  const PositionServiceConstPtr& posService) 
: hasher_(posService->GetHashService()),
  posService_(posService),
  hashedDist_(hasher_->HashSize())
{};

void DistanceService::HashAllDistances() const {
  for (unsigned int i=0; i<hasher_->HashSize(); i++) {
    for (unsigned int j=0; j<hasher_->HashSize() && j<=i; j++) {
      const I3Position pos_i = posService_->GetPosition(i);
      const I3Position pos_j = posService_->GetPosition(j);
      const double dist = (pos_i-pos_j).Magnitude() +0.5; //+0.5 for rounding
      hashedDist_.Set(i,j,dist);
    }
  }
};

double DistanceService::GetDistance(
  const CompactHash a,
  const CompactHash b) const 
{
  if (a==b) //same DOM has always distance 0.
    return 0.;
  uint16_t dist = hashedDist_.Get(a,b);
  if (dist) //if this entry is different from 0. thus has been set
    return double(dist);
  const I3Position pos_a = posService_->GetPosition(a);
  const I3Position pos_b = posService_->GetPosition(b);
  dist = (pos_a-pos_b).Magnitude() + 0.5; //+0.5 for rounding
  hashedDist_.Set(a,b,dist);
  return dist;
};  

bool DistanceService::VerifyAgainst(
  const I3OMGeoMap& omgeo) const 
{
  return posService_->VerifyAgainst(omgeo);
}

#if SERIALIZATION_ENABLED
  I3_SERIALIZABLE(DistanceService);
#endif
