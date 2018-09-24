/**
 * \file MiscHelpersTest.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * \version $Revision: 152603 $
 * \date $Date: 2017-01-11 14:23:55 +0100 (ons, 11 jan 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of IceHiveHelpers
 */

#include "ToolZ/MiscHelpers.h"

#include <I3Test.h>

#include "TestHelpers.h"

TEST_GROUP(MiscHelpers);

TEST (TimeSeparation) {
  
  /* @brief Give the time-separation of two time windows; preferred either I3Time or double-expressions of whatever
  * @note the '<' operator has to be defined for this; aka the times have to obey an ordering principle
  * @param startA start of eventA
  * @param endA stop of eventA
  * @param startB start of eventB
  * @param endB stop of eventB
  * @return NAN: full inclusion,
  *         negative value: partial inclusion by so many ns,
  *         positive value: separated by that many ns
  */
  // create test scenarios like this: // -0-1--2-3--4--5---6-7--8-9-
                                      // -o-o-[A-A-[AB-AB]-B-B]-o-o-
  ENSURE( TimeSeparation<double>(2, 5, 4, 7) == -1 ); //-o-o-[A-A-[AB-AB]-B-B]-o-o-
  
  ENSURE( std::isnan(TimeSeparation<double>(2, 8, 4, 7)) ); //-o-o-[A-A-[AB-AB-AB]-A-A]-o-o-
  
  ENSURE( TimeSeparation<double>(2, 4, 6, 8) == 2 ); //-o-o-[A-A-A]-o-[B-B-B]-o-o-
  
  ENSURE( TimeSeparation<double>(4, 7, 2, 5) == -1 ); //-o-o-[B-B-[AB-AB]-A-A]-o-o-
  
  ENSURE( std::isnan(TimeSeparation<double>(4, 7, 2, 8)) ); //-o-o-[B-B-[AB-AB-AB]-B-B]-o-o-
  
  ENSURE( TimeSeparation<double>(6, 8, 2, 4) == 2 ); //-o-o-[B-B-B]-o-[A-A-A]-o-o-
};