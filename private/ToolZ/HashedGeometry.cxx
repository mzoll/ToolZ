/**
 * \file HashedGeometry.cxx
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

#include "ToolZ/HashedGeometry.h"

HashedGeometry::HashedGeometry(const I3OMGeoMap& omgeo) 
: hashService_(boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(omgeo))),
  posService_(boost::make_shared<const PositionService>(omgeo, hashService_)),
  distService_(boost::make_shared<const DistanceService>(posService_))
{};

HashedGeometry::HashedGeometry(
  const I3OMGeoMap& omgeo,
  const std::set<OMKey> omkeys)
: hashService_(boost::make_shared<const CompactOMKeyHashService>(omkeys)),
  posService_(boost::make_shared<const PositionService>(omgeo, hashService_)),
  distService_(boost::make_shared<const DistanceService>(posService_))
{};


#if SERIALIZATION_ENABLED
HashedGeometry::HashedGeometry (const PositionServiceConstPtr& posService) 
: hashService_(posService->GetHashService()),
  posService_(posService),
  distService_(boost::make_shared<const DistanceService>(posService_))
{};
#endif //SERIALIZATION_ENABLED

#if SERIALIZATION_ENABLED
  #ifdef SERIALIZATON_SUPPORT_ICECUBE
I3_SERIALIZABLE(HashedGeometry);
  #endif // SERIALIZATON_SUPPORT_ICECUBE
  #ifdef SERIALIZATON_SUPPORT_BOOST
BOOST_CLASS_EXPORT_IMPLEMENT(HashedGeometry);
  #endif // SERIALIZATON_SUPPORT_BOOST
#endif //SERIALIZATON_ENABLED
