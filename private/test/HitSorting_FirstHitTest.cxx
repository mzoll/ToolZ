/**
 * \file HitSortingTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id:$
 * \version $Revision:$
 * \date $Date:$
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of HitSorting as it is implemented in CoincSuite/HitSorting.h
 */

#include "ToolZ/HitSorting_FirstHit.h"

#include <I3Test.h>
#include "TestHelpers.h"

using namespace HitSorting;

TEST_GROUP(HitSorting_FirstHit);


const I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(CreateGeometry());
const CompactOMKeyHashServiceConstPtr hashService = boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(geo));

I3RecoPulseSeriesMap CreateTestMap() {
  I3RecoPulseSeriesMap recoMap;
  recoMap[OMKey(1,1)].push_back(MakeRecoPulse(0., 1.));
  recoMap[OMKey(1,1)].push_back(MakeRecoPulse(1., 2.));
  recoMap[OMKey(1,2)].push_back(MakeRecoPulse(2., 3.));

  return recoMap;
};


TEST (I3RecoPulseSeriesMap_FirstHits_To_HitObjects) {
  /*
  template<class HitObjectOriginalContainer>
  HitObjectOriginalContainer
  I3RecoPulseSeriesMap_FirstHits_To_HitObjectOriginals(
    const I3RecoPulseSeriesMap& pulses);
  */
  
  typedef std::set<I3RecoPulse_HitObjectOriginal> I3RecoPulse_HitObjectOriginalSet;
  
  I3RecoPulseSeriesMap recoMap = CreateTestMap();
  I3RecoPulse_HitObjectOriginalSet ho = OMKeyMap_FirstHits_To_HitObjectOriginals<I3RecoPulse,I3RecoPulse_HitObjectOriginalSet>(recoMap);
  
  ENSURE_EQUAL(ho.begin()->GetOMKey(), OMKey(1,1));
  const I3RecoPulse& first_pulse = ho.begin()->GetResponseObj();
  ENSURE_EQUAL(first_pulse.GetTime(), 0.);
  ENSURE_EQUAL(first_pulse.GetCharge(), 1.+2.);
  
  ENSURE_EQUAL(ho.rbegin()->GetOMKey(), OMKey(1,2));
  const I3RecoPulse& second_pulse = ho.rbegin()->GetResponseObj();
  ENSURE_EQUAL(second_pulse.GetTime(), 2.);
  ENSURE_EQUAL(second_pulse.GetCharge(), 3.);
}


TEST (I3RecoPulseSeriesMap_FirstHits_To_AbsHits) {
  /*
  template<class AbsHitContainer>
  AbsHitContainer
  I3RecoPulseSeriesMap_FirstHits_To_AbsHits(
    const CompactOMKeyHashServiceConstPtr& hasher,
    const I3RecoPulseSeriesMap& pulses);
  */

  I3RecoPulseSeriesMap recoMap = CreateTestMap();
  AbsHitSet abshits = OMKeyMap_FirstHits_To_AbsHits<I3RecoPulse,AbsHitSet>(hashService, recoMap);
  
  ENSURE_EQUAL(abshits.begin()->GetDOMIndex(), hashService->HashFromOMKey(OMKey(1,1)));
  ENSURE_EQUAL(abshits.begin()->GetTime(), 0.);
  
  ENSURE_EQUAL(abshits.rbegin()->GetDOMIndex(), hashService->HashFromOMKey(OMKey(1,2)));
  ENSURE_EQUAL(abshits.rbegin()->GetTime(), 2.);
};
