/**
 * \file HitSortingTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: HitFacilityTest.cxx 153494 2017-02-23 17:13:55Z mzoll $
 * \version $Revision: 153494 $
 * \date $Date: 2017-02-23 18:13:55 +0100 (Thu, 23 Feb 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of HitSorting as it is implemented in HiveSplitter/HitSorting.h
 */

#include "ToolZ/HitFacility.h"
#include "TestHelpers.h"

#include <I3Test.h>

#include <boost/make_shared.hpp>

using namespace HitSorting;

//Do testing on the example of an I3RecoPulse

TEST_GROUP(HitFacility);

/// Extract and back-convert Hits by a HitFacility
TEST(OMKeyMap_HitFacility) {
  const I3RecoPulseSeriesMap recoMap= GenerateTestRecoPulses();

  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  frame->Put("KEY", boost::make_shared<I3RecoPulseSeriesMap>(recoMap));
  
  CompactOMKeyHashServiceConstPtr hasher;
  const OMKeyMap_HitFacility<I3RecoPulse> hf(frame, "KEY", hasher);
  
  HitSet hits = hf.GetHits<HitSet>();
  
  ENSURE_EQUAL(hits.size(), 86*60*20, "Expected number of entries for extracted hits");
  
  const I3RecoPulseSeriesMap cycledMap = hf.MapFromHits(hits);
  ENSURE(cycledMap.size() == recoMap.size());
};

///Test the additional Mask creation functionality for I3RecoPulseSeriesMaps
TEST(I3RecoPulseSeriesMap_HitFacility) {
  I3RecoPulseSeriesMap recoMap= GenerateTestRecoPulses();

  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  frame->Put("KEY", boost::make_shared<I3RecoPulseSeriesMap>(recoMap));
  
  I3RecoPulseSeriesMap recoMap_sub;

  recoMap_sub.insert(*recoMap.begin());
  
  CompactOMKeyHashServiceConstPtr hasher;
  const I3RecoPulseSeriesMap_HitFacility hf(frame, "KEY", hasher);
  
  HitList hits = hf.GetHits<HitList>();
  
  ENSURE_EQUAL(hits.size(), 86*60*20, "Expected number of entries for extracted hits");
  
  const I3RecoPulseSeriesMap cycledMap = hf.MapFromHits(hits);
  ENSURE_EQUAL(cycledMap.size(), recoMap.size());

  const I3RecoPulseSeriesMapMask cycledMask = hf.MaskFromHits(hits);
  const I3RecoPulseSeriesMap appliedMask = *(cycledMask.Apply(*frame));  
  ENSURE(appliedMask.size() == recoMap.size());
};


