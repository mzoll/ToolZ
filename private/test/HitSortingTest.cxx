/**
 * \file HitSortingTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: HitSortingTest.cxx 152601 2017-01-11 13:03:18Z mzoll $
 * \version $Revision: 152601 $
 * \date $Date: 2017-01-11 14:03:18 +0100 (Wed, 11 Jan 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of HitSorting as it is implemented in HiveSplitter/HitSorting.h
 */

#include "ToolZ/HitSorting.h"
#include "TestHelpers.h"

#include <I3Test.h>

#include <boost/make_shared.hpp>

using namespace HitSorting;

//Do testing on the example of an I3RecoPulse

TEST_GROUP(HitSorting);

///Forth and back conververt between a OMKeyMap an HitObjects
TEST(HitObjectConvertions){
  typedef std::list<HitObject<I3RecoPulse> > HitObjectList;
  
  const I3RecoPulseSeriesMap pulseMap = GenerateTestRecoPulses();
  const HitObjectList hitObjs = OMKeyMap_To_HitObjects<I3RecoPulse, HitObjectList>(pulseMap);
  
//   ENSURE_EQUAL(hitObjs.size(), 100*100*20);
  
  I3RecoPulseSeriesMap pulseMap_cycled = HitObjects_To_OMKeyMap<I3RecoPulse>(hitObjs);
  
  I3RecoPulseSeriesMap::const_iterator pulseMap_iter = pulseMap.begin();
  I3RecoPulseSeriesMap::const_iterator pulseMap_cycled_iter = pulseMap_cycled.begin();
  ENSURE_EQUAL(pulseMap_cycled.size(), pulseMap.size(), "Format of cycled Map is not as expected");
  ENSURE_EQUAL(pulseMap_cycled_iter->second.size(), pulseMap_iter->second.size(), "Format of cycled Map is not as expected");
  ENSURE_EQUAL(pulseMap_cycled_iter->second[0], pulseMap_iter->second[0], "First Pulse in each map are identical");
  
  //cycle once trough the maps and see if things are congruent
//   for (uint64_t i=0; i<100; i++) {
//     for (uint64_t j=0; j<100; j++) {
//       for (uint64_t k=0; k<20; k++) {
//         ENSURE_EQUAL(pulseMap_cycled[OMKey(i,j)][k], pulseMap[OMKey(i,j)][k], "pulses do not compare identical identical");
//       }
//     }
//   }
  
  while (pulseMap_cycled_iter!= pulseMap_cycled.end()) {
    ENSURE_EQUAL(pulseMap_cycled_iter->first, pulseMap_iter->first, "Not the same OMKey");
    I3RecoPulseSeries::const_iterator pulses_cycled_iter = pulseMap_cycled_iter->second.begin();
    I3RecoPulseSeries::const_iterator pulses_iter = pulseMap_iter->second.begin();
    while (pulses_cycled_iter != pulseMap_cycled_iter->second.end()) {
      ENSURE_EQUAL(*pulses_cycled_iter, *pulses_iter, "pulses are not equal");
      ++pulses_cycled_iter;
      ++pulses_iter;
    }
    ++pulseMap_cycled_iter;
    ++pulseMap_iter;
  }
};
