/**
 * \file ResponseHelpersTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of IceHiveHelpers
 */

#include "ToolZ/ResponseMapHelpers.h"

#include <I3Test.h>

#include "TestHelpers.h"

TEST_GROUP(ResponseMapHelpers);

/// sensible value test for hardcoded values
TEST(GetResponsesInTimeRange) {
  /* @brief Give me all pulses from this ResponseMap which fall within this time-window
   *  A simple selection function
   *  @param pulses from this pulses
   *  @param tw_start starting at this time
   *  @param tw_stop ending at this time
   *  @return the subset of pulses
   */
  I3RecoPulseSeriesMap reco_map;
  reco_map[OMKey(1,1)].push_back(MakeRecoPulse(0.,1.));
  reco_map[OMKey(1,1)].push_back(MakeRecoPulse(1.,1.));
  reco_map[OMKey(2,2)].push_back(MakeRecoPulse(1.,1.));
  reco_map[OMKey(2,2)].push_back(MakeRecoPulse(2.,1.));
  //testcase done
  
  I3RecoPulseSeriesMap clip_map;
  I3RecoPulseSeriesMap::const_iterator c_iter;
  
  //just take the first part 
  clip_map = GetResponsesInTimeRange<I3RecoPulse>(reco_map, 0., 0.);
  ENSURE_EQUAL(clip_map.size(), (size_t)1, "Size is of output is as expected");
  c_iter= clip_map.begin();
  ENSURE(c_iter->first==OMKey(1,1) && c_iter->second.size()==(size_t)1, "Right OMKey(1,1) has right amount of pulses");
  
  //just take the first part 
  clip_map = GetResponsesInTimeRange<I3RecoPulse>(reco_map, 0., 1.);
  ENSURE_EQUAL(clip_map.size(), (size_t)2, "Size is of output is as expected");
  c_iter= clip_map.begin();
  ENSURE(c_iter->first==OMKey(1,1) && c_iter->second.size()==(size_t)2, "Right OMKey(1,1) has right amount of pulses");
  c_iter++;
  ENSURE(c_iter->first==OMKey(2,2) && c_iter->second.size()==(size_t)1, "Right OMKey(2,2) has right amount of pulses");
  
  //just take the middle part wher upper==lower
  clip_map = GetResponsesInTimeRange<I3RecoPulse>(reco_map, 1., 1.);
  ENSURE_EQUAL(clip_map.size(), (size_t)2, "Size is of output is as expected");
  c_iter= clip_map.begin();
  ENSURE(c_iter->first==OMKey(1,1) && c_iter->second.size()==(size_t)1, "Right OMKey(1,1) has right amount of pulses");
  c_iter++;
  ENSURE(c_iter->first==OMKey(2,2) && c_iter->second.size()==(size_t)1, "Right OMKey(2,2) has right amount of pulses");
};



TEST (GetMaskAncestry) {
  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  
  I3RecoPulseSeriesMap map;
  I3RecoPulseSeries series;
  I3RecoPulse reco;
  OMKey key;
  
  series.push_back(reco);
  map[key] = series;
  
  frame->Put("Pulses0", boost::make_shared<I3RecoPulseSeriesMap>(map)); //I3RecoMap containing 1 pulse
  frame->Put("Pulses1", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses0", map)); //Mask containingh one pulse
  frame->Put("Pulses2", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses1", I3RecoPulseSeriesMap())); //Mask containing no pulses
  frame->Put("Pulses3", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses2", I3RecoPulseSeriesMap())); //Mask containing no pulses
  
  /* Get a list of all deriations of this mask; children up front, grandparents in the back
   * @param frame The frame which is searched for the the mask and its ancestors
   * @param key The name of a I3RecoPulseSeriesMap or I3RecoPulseSeriesMapMask
   * @return the ancestry of this map/mask, parents in the back
   */
  
  std::vector<std::string> ancestry = GetMaskAncestry (frame, "Pulses3");
  
  ENSURE( ancestry.size()==4);
  ENSURE( ancestry[0] == "Pulses3");
  ENSURE( ancestry[1] == "Pulses2");
  ENSURE( ancestry[2] == "Pulses1");
  ENSURE( ancestry[3] == "Pulses0");
}

TEST (GetCommonMaskAncestry) {
  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  
  I3RecoPulseSeriesMap map;
  I3RecoPulseSeries series;
  I3RecoPulse reco;
  OMKey key;
  
  series.push_back(reco);
  map[key] = series;
  
  //the common hierarchy
  frame->Put("Pulses0", boost::make_shared<I3RecoPulseSeriesMap>(map)); //I3RecoMap containing 1 pulse
  frame->Put("Pulses1", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses0", map)); //Mask containingh one pulse
  //the one mask
  frame->Put("Fork1", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses1", map)); //Mask containing one pulse
  //the other mask
  frame->Put("Fork2", boost::make_shared<I3RecoPulseSeriesMapMask>(*frame, "Pulses1", I3RecoPulseSeriesMap())); //Mask containing no pulses
  
  /*Get me the oldest common ancestors for masks
   * @param frame The frame which is searched for the the mask and its ancestors
   * @param key1 the one I3RecoPulseSeriesMapMask
   * @param key2 the other I3RecoPulseSeriesMapMask
   * @return key of the first common ancestor
   */
  std::vector<std::string> common_ancestry = GetCommonMaskAncestry (frame, "Fork1", "Fork2");
  
  ENSURE( common_ancestry.size()==2);
  ENSURE( common_ancestry[0] == "Pulses1");
  ENSURE( common_ancestry[1] == "Pulses0");
}  

TEST(UniteRecoMaps) {
  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  
  I3RecoPulseSeriesMap map1, map2, map3;
  I3RecoPulseSeries series1, series2;
  I3RecoPulse reco1; reco1.SetTime(1.); reco1.SetCharge(1.);
  I3RecoPulse reco2; reco2.SetTime(2.); reco2.SetCharge(2.);
  
  OMKey key1(1,1);
  OMKey key2(2,2);
  
  series1.push_back(reco1);
  series2.push_back(reco2);
  
  map1[key1] = series1; //reco1 at omkey1
  map2[key1] = series2; //reco2 at omkey1
  map3[key2] = series2; //reco2 at omkey2

  /** @brief A convenience function to unite two RecoMaps into a single one
   * @param mapA the one map
   * @param mapB the other map
   * @return the united map
   */
  I3RecoPulseSeriesMap unite12 = UniteRecoMaps (map1, map2);
  
  ENSURE(unite12.size()==1);
  I3RecoPulseSeriesMap::const_iterator u12_iter = unite12.begin();
  ENSURE(u12_iter->first == OMKey(1,1));
  ENSURE(u12_iter->second.size() == 2);
  ENSURE(u12_iter->second[0] == reco1);
  ENSURE(u12_iter->second[1] == reco2);
  
  I3RecoPulseSeriesMap unite13 = UniteRecoMaps (map1, map3);
  ENSURE(unite13.size()==2);
  I3RecoPulseSeriesMap::const_iterator u13_iter = unite13.begin();
  ENSURE(u13_iter->first == OMKey(1,1));
  ENSURE(u13_iter->second.size() == 1);
  ENSURE(u13_iter->second[0] == reco1);
  u13_iter++;
  ENSURE(u13_iter->first == OMKey(2,2));
  ENSURE(u13_iter->second.size() == 1);
  ENSURE(u13_iter->second[0] == reco2);
}