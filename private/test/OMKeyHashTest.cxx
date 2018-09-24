/**
 * \file OMKeyHashTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: OMKeyHashTest.cxx 153711 2017-03-03 11:19:05Z mzoll $
 * \version $Revision: 153711 $
 * \date $Date: 2017-03-03 12:19:05 +0100 (Fri, 03 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of OMKey Hashing as it is implemented in IceHive/OMKeyHash.h
 */

#include "ToolZ/OMKeyHash.h"

#include "ToolZ/IC86Topology.h"

#include <I3Test.h>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "TestHelpers.h"


TEST_GROUP(OMKeyHash);

static I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(IC86Topology::Build_IC86_Geometry());

/// Use a geometry to create a hasher and perform some simple tests
TEST(UsingGeometry){
  
  CompactOMKeyHashService hasher(ExtractOMKeys(geo));
  
  ///of course the geo should be verifyable against itself
  ENSURE(hasher.VerifyAgainst(ExtractOMKeys(geo)));
  
  /// test the size of the hashes is identical to the number of OMKeys in the geo
  ENSURE(geo->omgeo.size()==hasher.HashSize());
  
  /// the expected operations of forward and back hashing works
  CompactHash currentHash= 0;
  BOOST_FOREACH(const I3OMGeoMap::value_type& omkey_omgeo, geo->omgeo) {
    const OMKey& omkey = omkey_omgeo.first;
    const CompactHash hash = hasher.HashFromOMKey(omkey);
    ENSURE_EQUAL(currentHash, hash);
    OMKey omkey_re = hasher.OMKeyFromHash(hash);
    ENSURE_EQUAL(omkey_re, omkey);
    currentHash++;
  }
};

TEST(UsingVector) {
  std::set<OMKey> omkey_set;
  for(unsigned i=0; i<100; i++) {
    for (unsigned j=0; j<100; j++) {
      omkey_set.insert(omkey_set.end(),OMKey(i,j));
    }
  }
//   CompactOMKeyHashService hasher(omkey_set.begin(), omkey_set.end());
  CompactOMKeyHashService hasher(omkey_set);
  
  /// test the size of the hashes is identical to the number of OMKeys in the geo
  ENSURE(omkey_set.size()==hasher.HashSize());
  
  std::set<OMKey> omkey_set_ex = hasher.GetOMKeys();
  
  hasher.OMKeyFromHash(100);
  hasher.HashFromOMKey(OMKey(55,55));
};


#if SERIALIZATION_ENABLED
TEST(Serialize_raw_ptr){
  CompactOMKeyHashService* hashService_save = new CompactOMKeyHashService(ExtractOMKeys(geo));
  CompactOMKeyHashService* hashService_load = nullptr; //(CompactOMKeyHashService*)malloc(sizeof(CompactOMKeyHashService));
  
  serialize_object(hashService_save, hashService_load);
  //   hashService_load->GetOMKeys();
  delete hashService_save;
  delete hashService_load;
};

TEST(Serialize_boost_shared_ptr){
  CompactOMKeyHashServicePtr hashService_save = boost::make_shared<CompactOMKeyHashService>(ExtractOMKeys(geo));
  CompactOMKeyHashServicePtr hashService_load = nullptr; //((CompactOMKeyHashService*)malloc(sizeof(CompactOMKeyHashService)));
  
  serialize_object(hashService_save, hashService_load);
//   hashService_load->GetOMKeys();
};
#endif
