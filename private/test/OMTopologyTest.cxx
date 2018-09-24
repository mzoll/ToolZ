/**
 * \file OMTopologyTest.cxx
 *
 * (c) 2012 the IceCube Collaboration
 *
 * \version $Revision: 129966 $
 * \date $Date: 2015-03-07 21:57:35 +0100 (lör, 07 mar 2015) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 * 
 * Describes the different topological settings for a DOM
 */

#include "ToolZ/OMTopology.h"

#include "I3Test.h"

#include "ToolZ/IC86Topology.h"

#include "TestHelpers.h"

#include <boost/make_shared.hpp>

TEST_GROUP(OMTopology)

TEST (OMTopology) {
  OMTopology omt;
  
  ENSURE_EQUAL(omt.GetFlags().size(),0);
  
  omt.SetFlag("InIce"); //== omt.SetFlag("InIce", true);
  omt.SetFlag("IceCube", true);
  
  ENSURE_EQUAL(omt.GetFlags().size(), 2);
  
  ENSURE(omt.GetFlag("InIce"));
  ENSURE(omt.GetFlag("IceCube"));
  
  omt.SetFlag("IceCube", false); //disable IceCube flag
  
  ENSURE_EQUAL(omt.GetFlags().size(), 1);
  
  ENSURE(omt.GetFlag("InIce"));
  ENSURE(! omt.GetFlag("IceCube"));
};

TEST (OMTopology_compare) {
  OMTopology omt;
  omt.SetFlag("InIce"); //== omt.SetFlag("InIce", true);
  omt.SetFlag("IceCube", true);

  std::vector<std::string> flags_c;
  flags_c.push_back("InIce");
  flags_c.push_back("DeepCore");
  OMTopology omt_c(flags_c);
  
  ENSURE(omt.Common(omt_c));
};


TEST (OMTopologyMap_Comparator) {
  OMTopologyMap omtmap;
  
  OMTopology omt_ic;
  omt_ic.SetFlag("InIce");
  omt_ic.SetFlag("IceCube");
  
  omtmap[OMKey(1,1)] = omt_ic;
  omtmap[OMKey(1,2)] = omt_ic;
  
  OMTopology omt_dc;
  omt_dc.SetFlag("InIce");
  omt_dc.SetFlag("DeepCore");
  
  omtmap[OMKey(1,10)] = omt_dc;
  
  OMTopology omt_it;
  omt_it.SetFlag("IceTop");
  
  omtmap[OMKey(1,61)] = omt_it;
  
  
  OMTopology omt_in;
  omt_in.SetFlag("InIce");
  
  
  OMTopologyMap_Comparator comp(omt_in, omtmap);
  
  ENSURE(comp(OMKey(1,1)));
  ENSURE(comp(OMKey(1,2)));
  ENSURE(comp(OMKey(1,10)));
  ENSURE(!comp(OMKey(1,61)));
  
  std::list<OMKey> all_in = comp.AllCommon();
  
  ENSURE_EQUAL(all_in.size(), 3);
}

#if SERIALIZATION_ENABLED
TEST(OMTopology_Serialize_raw_ptr){
  OMTopology* omt_save = new OMTopology();
  OMTopology* omt_load = nullptr;
  serialize_object(omt_save, omt_load);
  delete omt_save;
  delete omt_load;
};

TEST(OMTopology_Serialize_shared_ptr){
  OMTopologyPtr omt_save = boost::make_shared<OMTopology>();
  OMTopologyPtr omt_load;
  
  serialize_object(omt_save, omt_load);
};

TEST(OMTopologyMap_Serialize_raw_ptr){
  OMTopologyMap* omt_save = new OMTopologyMap();
  OMTopologyMap* omt_load = nullptr;
  
  serialize_object(omt_save, omt_load);
  delete omt_save;
  delete omt_load;
};

TEST(OMTopologyMap_Serialize_shared_ptr){
  OMTopologyMapPtr omt_save = boost::make_shared<OMTopologyMap>();
  OMTopologyMapPtr omt_load;
  
  serialize_object(omt_save, omt_load);
};
#endif

