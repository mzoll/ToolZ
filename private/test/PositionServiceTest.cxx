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
 * Unit test to test the robustness of the PositionService and storing the position and distances between all hashed DOMs
 */
#include <I3Test.h>

#include "ToolZ/PositionService.h"

#include "ToolZ/IC86Topology.h"

#include "TestHelpers.h"

#include <boost/make_shared.hpp>

TEST_GROUP(PositionService)

//create some objects which are used in all the hashings
static I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(IC86Topology::Build_IC86_Geometry());
static CompactOMKeyHashServiceConstPtr hasher = boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(geo));
static PositionServiceConstPtr posService = boost::make_shared<const PositionService>(geo->omgeo,hasher); 

TEST(Verify_against) {
  ENSURE(posService->VerifyAgainst(geo->omgeo));
}

TEST(get_position) {
  for (int i=0; i<hasher->HashSize(); i++) {
    const OMKey omkey = hasher->OMKeyFromHash(i); 
    
    ENSURE_EQUAL(posService->GetPosition(omkey), geo->omgeo.at(omkey).position);
  }
}
  
TEST(get_distance) {
  for (int i=0; i<hasher->HashSize(); i++) {
    const OMKey omkey_a = hasher->OMKeyFromHash(i); 
    I3Position pos_a = geo->omgeo.at(omkey_a).position;
    ENSURE_EQUAL(posService->GetPosition(omkey_a), pos_a);
  }
}

#if SERIALIZATION_ENABLED
TEST(Serialize_raw_ptr){
  PositionService* ps_save = new PositionService(geo->omgeo, hasher);
  PositionService* ps_load = nullptr;
  
  serialize_object(ps_save, ps_load);
  delete ps_save;
  delete ps_load;
};

TEST(Serialize_boost_shared_ptr){
  PositionServicePtr ps_save = boost::make_shared<PositionService>(geo->omgeo,hasher);
  PositionServicePtr ps_load;
  
  serialize_object(ps_save, ps_load);
};
#endif //SERIALIZATION_ENABLED
