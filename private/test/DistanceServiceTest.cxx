/**
 * \file DistanceServiceTest.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of the DistanceService and storing the position and distances between all hashed DOMs
 */
#include <I3Test.h>

#include "ToolZ/DistanceService.h"

#include "ToolZ/IC86Topology.h"

#include "TestHelpers.h"

#include <boost/make_shared.hpp>

TEST_GROUP(DistanceService)

//create some objects which are used in all the hashings
static I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(IC86Topology::Build_IC86_Geometry());
static CompactOMKeyHashServiceConstPtr hasher = boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(geo));
static PositionServiceConstPtr posService = boost::make_shared<const PositionService>(geo->omgeo,hasher); 

static DistanceServiceConstPtr distService = boost::make_shared<const DistanceService>(posService); 


TEST(Verify_against) {
  ENSURE(distService->VerifyAgainst(geo->omgeo));
}
  
TEST(get_distance) {
  for (int i=0; i<hasher->HashSize(); i++) {
    const OMKey omkey_a = hasher->OMKeyFromHash(i); 
    I3Position pos_a = geo->omgeo.at(omkey_a).position;
    for (int j=0; j<hasher->HashSize(); j++) {
      const OMKey omkey_b = hasher->OMKeyFromHash(j); 
      I3Position pos_b = geo->omgeo.at(omkey_b).position;
      ENSURE_DISTANCE(distService->GetDistance(omkey_a, omkey_b), (pos_a-pos_b).Magnitude(), 0.5); //exact within 0.5meters
    }
  }
}

#if SERIALIZATION_ENABLED
TEST(Serialize_raw_ptr){
  DistanceService* ds_save = new DistanceService(posService);
  DistanceService* ds_load = nullptr;
  
  serialize_object(ds_save, ds_load);
  delete ds_save;
  delete ds_load;
};

TEST(Serialize_boost_shared_ptr){
  DistanceServicePtr ds_save = boost::make_shared<DistanceService>(posService);
  DistanceServicePtr ds_load;
  
  serialize_object(ds_save, ds_load);
};
#endif //SERIALIZATION_ENABLED
