/**
 * \file TriggerHierarchyHelpers.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: TriggerHierarchyHelpers.h 153958 2017-03-17 13:42:47Z mzoll $
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Helper functions for the small man that wants to perform some common tasks
 */

#ifndef TRIGGERHIERARCHYHELPERS_H
#define TRIGGERHIERARCHYHELPERS_H

static const unsigned triggerHierarchyhelpers_version_ = 0;

#include "dataclasses/I3TimeWindow.h"
#include "dataclasses/physics/I3TriggerHierarchy.h"


/** @brief Clip out the triggers with the given configIDs and within the specified TimeWindow
  * Take all triggers firing within the specified time-window (triggers can thereby extend beyond the specified time-window)
  * If no trigger-configIDs are specified take every trigger in the time-window regardlessly
  * Each TroughPut-trigger, not the global, will have the length of the original trigger (a neccessity to maintain object-structure)
  * @param trigHier find triggers in there
  * @param configIDs the configured Trigger-IDs of thoese we gonna look for
  * @param tw_start the start time from which clipping commenses
  * @param tw_stop the stop time until which clipping commenses
  * @return a clipped trigger Hierarchy
  */
I3TriggerHierarchy ClipTriggerHierarchy(
  const I3TriggerHierarchy& trigHier,
  const double tw_start,
  const double tw_stop,
  const std::vector<int>& configIDs = std::vector<int>());

/** @brief Clip out the triggers with the given configIDs and within the specified TimeWindow
  * Take all triggers firing within the specified time-window (triggers can thereby extend beyond the specified time-window)
  * If no trigger-configIDs are specified take every trigger in the time-window regardlessly
  * Each TroughPut-trigger, not the global, will have the length of the original trigger (a neccessity to maintain object-structure)
  * @param trigHier find triggers in there
  * @param configIDs the configured Trigger-IDs of thoese we gonna look for
  * @param twindow the time window around which we gonna look for triggers
  * @return a clipped trigger Hierarchy
  */
I3TriggerHierarchy ClipTriggerHierarchy(
  const I3TriggerHierarchy& trigHier,
  const I3TimeWindow& twindow,
  const std::vector<int>& configIDs = std::vector<int>());

/** Unite two TriggererHierarchies into a single one
  * @param trigA the one TriggerHierarchy
  * @param trigB the other TriggerHierarchy
  * @return the united Hierarchy
  */
I3TriggerHierarchy UniteTriggerHierarchies (
  const I3TriggerHierarchy& trigA,
  const I3TriggerHierarchy& trigB);

#endif // TRIGGERHIERARCHYHELPERS_H
