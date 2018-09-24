/**
 * \file HitSortingTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: HitclassesTest.cxx 152601 2017-01-11 13:03:18Z mzoll $
 * \version $Revision: 152601 $
 * \date $Date: 2017-01-11 14:03:18 +0100 (Wed, 11 Jan 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of HitSorting as it is implemented in HiveSplitter/HitSorting.h
 */

#include "ToolZ/Hitclasses.h"
#include "TestHelpers.h"

#include <I3Test.h>

#include <boost/make_shared.hpp>

//Do testing on the example of an I3RecoPulse

TEST_GROUP(Hitclasses);

TEST(HitObjects) {
  OMKey omkey(1,1);
  I3RecoPulse pulse = MakeRecoPulse(0, 1.);
  HitObject<I3RecoPulse> ho(omkey, pulse);
  Hit h(0, GetInferredTime(pulse), ho);
};

