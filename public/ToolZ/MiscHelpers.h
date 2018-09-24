/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: MiscHelpers.h 153713 2017-03-03 14:14:55Z mzoll $
 *
 * @file MiscHelpers.h
 * @version $Revision: 153713 $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 *
 * Provide some convenience functions
 */

#ifndef MISCHELPERS_H
#define MISCHELPERS_H

#include <vector>
#include <string>
#include <utility>

#include "icetray/I3Frame.h"

#include "dataclasses/physics/I3EventHeader.h"


/** Get an unique identification string for frames with not so much information
  * @param frame 
  * @return a nicely formated string
  */
std::string FrameIDstring (const I3FrameConstPtr frame);

/** @brief Give the time-separation of two time windows; preferred either I3Time or double-expressions of whatever
* @note the '<' operator has to be defined for this; aka the times have to obey an ordering principle
* @param startA start of eventA
* @param endA stop of eventA
* @param startB start of eventB
* @param endB stop of eventB
* @return NAN: full inclusion,
*         negative value: partial inclusion by so many ns,
*         positive value: separated by that many ns
*/
template <class time_var>
double TimeSeparation(const time_var startA,
                      const time_var endA,
                      const time_var startB,
                      const time_var endB);

//=================================================================
//=================== IMPLEMENTATIONS =============================
//=================================================================

template <class time_var>
double TimeSeparation(const time_var startA,
                      const time_var endA,
                      const time_var startB,
                      const time_var endB) {
  if (startA < startB) {//A before B
    if (startB < endA) {//B starts within A
      if (endA < endB) //B ends after A ends
        return startB-endA; //-o-o-[A-A-[AB-AB]-B-B]-o-o-
      else //A fully includes B
        return NAN; //-o-o-[A-A-[AB-AB-AB]-A-A]-o-o-
    }else //A and B are separated
      return startB-endA; //-o-o-[A-A-A]-o-[B-B-B]-o-o-
  }else{ //B before A
    if (startA < endB) {//A starts within B
      if (endB < endA) //A ends after B ends
        return startA-endB; //-o-o-[B-B-[AB-AB]-A-A]-o-o-
      else //B fully includes A
        return NAN; //-o-o-[B-B-[AB-AB-AB]-B-B]-o-o-
    }else //B and A are separated
      return startA-endB; //-o-o-[B-B-B]-o-[A-A-A]-o-o-
  }
};

#endif // MISCHELPERS_H
