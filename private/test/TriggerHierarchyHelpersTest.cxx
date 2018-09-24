/**
 * \file TriggerHierarchyHelpersTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of IceHiveHelpers
 */

#include "ToolZ/TriggerHierarchyHelpers.h"

#include <I3Test.h>

TEST_GROUP(TriggerHierarchyHelpers);

//pose a simple scenario to clip a trigger Hierarchy
TEST(ClipTriggerHierarchy) {
  /* @brief Clip out the triggers with the given configIDs and within the specified TimeWindow
   * @param trigHier find triggers in there
   * @param configIDs the configured Trigger-IDs of thoese we gonna look for
   * @param tw_start the start time from which clipping commenses
   * @param tw_stop the stop time until which clipping commenses
   * @return a clipped trigger Hierarchy
   */
  
  //build a trigger Hierarchy which looks like this
  //   Time         : 0         |10
  // GlobalTroughput: GGGGGGGGGG
  // Throughput     :  TTTTTTTT
  // TriggerID-1    :   111111
  // Throughput     :  TTT  TTT
  // TriggerID-2    :   2    2
  
  I3TriggerHierarchy th;
  I3TriggerHierarchy::iterator subIter;
  I3TriggerHierarchy::iterator eachTrig;
  
  //a global trigger
  I3Trigger globTrigger;
  //local throughput
  I3Trigger tpTrigger_1_0, tpTrigger_2_0, tpTrigger_2_1;
  //inice-triggers
  I3Trigger aTrigger_1_0, aTrigger_2_0, aTrigger_2_1;
  
  //now fill and insert in trigger hierarchy
  globTrigger.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::MERGED);
  globTrigger.SetTriggerFired(true);
  globTrigger.SetTriggerTime(0.);
  globTrigger.SetTriggerLength(10.);
  
  //fill in the triggers, strat with the global trigger
  subIter = th.insert(th.end(), globTrigger);
   
  //fill a trigger, strat with its troughput
  tpTrigger_1_0.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
  tpTrigger_1_0.SetTriggerFired(true);
  tpTrigger_1_0.SetTriggerTime(0.);
  tpTrigger_1_0.SetTriggerLength(10.);
  aTrigger_1_0.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::SIMPLE_MULTIPLICITY);
  aTrigger_1_0.SetTriggerFired(true);
  aTrigger_1_0.GetTriggerKey().SetConfigID(1);
  aTrigger_1_0.SetTriggerTime(1.);
  aTrigger_1_0.SetTriggerLength(8.);
  
  eachTrig = th.append_child(subIter, tpTrigger_1_0);
  th.append_child(eachTrig, aTrigger_1_0);
  
  // fill next trigger
  tpTrigger_2_0.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
  tpTrigger_2_0.SetTriggerFired(true);
  tpTrigger_2_0.SetTriggerTime(1.);
  tpTrigger_2_0.SetTriggerLength(3.);
  aTrigger_2_0.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::SIMPLE_MULTIPLICITY);
  aTrigger_2_0.SetTriggerFired(true);
  aTrigger_2_0.GetTriggerKey().SetConfigID(2);
  aTrigger_2_0.SetTriggerTime(2.);
  aTrigger_2_0.SetTriggerLength(1.);

  eachTrig = th.append_child(subIter, tpTrigger_2_0);
  th.append_child(eachTrig, aTrigger_2_0);
  
  // fill next trigger
  tpTrigger_2_1.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
  tpTrigger_2_1.SetTriggerFired(true);
  tpTrigger_2_1.SetTriggerTime(6.);
  tpTrigger_2_1.SetTriggerLength(3.);
  aTrigger_2_1.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::SIMPLE_MULTIPLICITY);
  aTrigger_2_1.SetTriggerFired(true);
  aTrigger_2_1.GetTriggerKey().SetConfigID(2);
  aTrigger_2_1.SetTriggerTime(7.);
  aTrigger_2_1.SetTriggerLength(1.);
  
  eachTrig = th.append_child(subIter, tpTrigger_2_1);
  th.append_child(eachTrig, aTrigger_2_1);
  
  // TESTCASE CONSTRUCTED

  std::vector<int> configIDs;
  I3TriggerHierarchy clip_th;
  
  // testcase: take everything and tell me if its the same
  configIDs = std::vector<int>();
  clip_th = ClipTriggerHierarchy(th, 0., 10., configIDs);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_1_0.GetTriggerKey()), (size_t)1);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_2_0.GetTriggerKey()), (size_t)2);
  
  //testcase: take first half
  configIDs = std::vector<int>();
  clip_th = ClipTriggerHierarchy(th, 0., 5., configIDs);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_1_0.GetTriggerKey()), (size_t)1);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_2_0.GetTriggerKey()), (size_t)1);
  
  //testcase: take only triggers with ID=1
  configIDs = std::vector<int>(1,1);
  clip_th = ClipTriggerHierarchy(th, 0., 5., configIDs);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_1_0.GetTriggerKey()), (size_t)1);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_2_0.GetTriggerKey()), (size_t)0);
  
  //testcase: select nothing
  clip_th = ClipTriggerHierarchy(th, 0., 0., configIDs);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_1_0.GetTriggerKey()), (size_t)0);
  ENSURE_EQUAL(I3TriggerHierarchyUtils::Count(clip_th, aTrigger_2_0.GetTriggerKey()), (size_t)0);
};

TEST(UniteTriggerHierarchies) {
  //make a scenario where triggers are order [t[i]t]-[t[i]t]
  
  I3TriggerHierarchy trigHierU;
  I3TriggerHierarchy trigHier1;
  I3TriggerHierarchy::iterator eachTrig;
  
  I3Trigger hsTrigger;
  hsTrigger.GetTriggerKey() = TriggerKey(TriggerKey::IN_ICE, TriggerKey::FRAGMENT_MULTIPLICITY, 1);
  hsTrigger.SetTriggerFired(true);
  
  I3Trigger tpTrigger;
  tpTrigger.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
  tpTrigger.SetTriggerFired(true);
  
  hsTrigger.SetTriggerTime(1);
  hsTrigger.SetTriggerLength(1);
  tpTrigger.SetTriggerTime(0);
  tpTrigger.SetTriggerLength(3);

  eachTrig = trigHier1.append_child(trigHier1.end(), tpTrigger);
  trigHier1.append_child(eachTrig, hsTrigger);
  eachTrig = trigHierU.append_child(trigHierU.end(), tpTrigger);
  trigHierU.append_child(eachTrig, hsTrigger);
  

  I3TriggerHierarchy trigHier2;
  
  hsTrigger.SetTriggerTime(5);
  hsTrigger.SetTriggerLength(1);

  tpTrigger.SetTriggerTime(4);
  tpTrigger.SetTriggerLength(3);
  
  eachTrig = trigHier2.append_child(trigHier2.end(), tpTrigger);
  trigHier2.append_child(eachTrig, hsTrigger);
  eachTrig = trigHierU.append_child(trigHierU.end(), tpTrigger);
  trigHierU.append_child(eachTrig, hsTrigger);
  //scenario ready
  
  //FIXME
  //I3TriggerHierarchy unite12 = UniteTriggerHierarchies (trigHier1, trigHier2);
  // make the test if everything did work out;
  //ENSURE(trigHierU==unite12);
}