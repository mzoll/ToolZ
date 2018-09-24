/**
 * \file PositionServiceTest.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of the HashedGeometry storing the subservices
 */
#include <I3Test.h>

#include "ToolZ/HashedGeometry.h"

#include "ToolZ/IC86Topology.h"

#include "TestHelpers.h"

#include <boost/make_shared.hpp>

TEST_GROUP(HashedGeometry)

//create some objects which are used in all the hashings
const I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(IC86Topology::Build_IC86_Geometry());

TEST(HashedGeo) {
  /// constructor: hash this omgeo
  {
  HashedGeometryConstPtr hashedGeo= boost::make_shared<HashedGeometry>(geo->omgeo);
  ENSURE_EQUAL(hashedGeo->GetHashService()->HashSize(), geo->omgeo.size());
  } 

  std::set<OMKey> omkeys = ExtractOMKeys(geo->omgeo);
  /// constructor: hash this omgeo, but only the specified OMKeys
  HashedGeometryConstPtr hashedGeo= boost::make_shared<HashedGeometry>(geo->omgeo, omkeys);
  ENSURE_EQUAL(hashedGeo->GetHashService()->HashSize(), geo->omgeo.size());
  
  //test getting subservices
  
  ENSURE(hashedGeo->GetHashService() != 0);
  ENSURE(hashedGeo->GetPosService() != 0);
  ENSURE(hashedGeo->GetDistService() != 0);
};

#if SERIALIZATION_ENABLED
TEST(Serialize_raw_ptr){
  HashedGeometry* hg_save = new HashedGeometry(geo->omgeo);
  HashedGeometry* hg_load = nullptr;
  
  serialize_object(hg_save, hg_load);
  delete hg_save;
  delete hg_load;
};

TEST(Serialize_boost_shared_ptr){
  HashedGeometryPtr hg_save = boost::make_shared<HashedGeometry>(geo->omgeo);
  HashedGeometryPtr hg_load;
  
  serialize_object(hg_save, hg_load);
};
#endif //SERIALIZATION_ENABLED
