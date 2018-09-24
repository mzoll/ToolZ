/**
 * \file TriggerHierarchyHelpers.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: TriggerHierarchyHelpers.cxx 153958 2017-03-17 13:42:47Z mzoll $
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Pybindings to the IceHiveHelpers
 */

#include <boost/python.hpp>

#include "ToolZ/TriggerHierarchyHelpers.h"


namespace bp = boost::python;

I3TriggerHierarchy Clip_TriggerHierarchy (
  const I3TriggerHierarchy &trigHier,
  const I3TimeWindow& twindow,
  const bp::list& l) 
{
  std::vector<int> trig_ids;
  for (int i=0; i<bp::len(l); i++)
    trig_ids.push_back(bp::extract<int>(l[i]));
  return ClipTriggerHierarchy (trigHier,twindow,trig_ids);
};

//Do the pybindings
void register_TriggerHierarchyHelpers() {
  bp::def("clip_TriggerHierarchy", &Clip_TriggerHierarchy,
    bp::args("trigHier","twindow","configIDs"),
    "Clips a TriggerHierarchy based on config IDs and time window.");
  
  bp::def("UniteTriggerHierarchies", &UniteTriggerHierarchies,
    bp::args("trigA","trigB"),
    "A convenience function to unite two TriggererHierarchies into a single one.");
};

