/**
 * \file I3RUsageTimerTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: OMKeyHashTest.cxx 148544 2016-07-18 17:14:06Z mzoll $
 * \version $Revision: 148544 $
 * \date $Date: 2016-07-18 19:14:06 +0200 (mån, 18 jul 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of the IceHiveZ internal Stopwatch
 */

#include <I3Test.h>

#include "ToolZ/I3RUsageTimer.h"

//##############################################################################


TEST_GROUP(I3RUsageTimer);

TEST(Stopwatch) {
  ///cycle the time once: Start, Stop, Start, Stop, Report, Reset, Start, Stop;
  
  I3RUsageTimer stopwatch;
  
  stopwatch.Start();
  sleep(1);
  stopwatch.Stop();
  
  stopwatch.Start();
  sleep(1);
  stopwatch.Stop();
  
  convertI3RUsageToString(*stopwatch.GetTotalRUsage());
  
  stopwatch.ResetTotalTimes();
  
  stopwatch.Start();
  sleep(1);
  stopwatch.Stop();
};
