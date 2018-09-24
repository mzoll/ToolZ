/**
 * \file ResponseMapHelpers.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: ResponseMapHelpers.cxx 153958 2017-03-17 13:42:47Z mzoll $
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Pybindings to the IceHiveHelpers
 */

#include <boost/python.hpp>

#include "ToolZ/ResponseMapHelpers.h"

//specialize for these Hit/Pulse-like classes, which come in the flavours of I3Map<OMKey, vector<Hit> >
#include "dataclasses/physics/I3RecoPulse.h"
#include "dataclasses/physics/I3DOMLaunch.h"
#include "simclasses/I3MCPulse.h"

namespace bp = boost::python;

I3RecoPulseSeriesMap Clip_I3RecoPulses (const I3RecoPulseSeriesMap &pulses,
                                        const I3TimeWindow &tw) {
  return GetResponsesInTimeRange<I3RecoPulse> (pulses, tw);
};

I3DOMLaunchSeriesMap Clip_I3DOMLaunches (const I3DOMLaunchSeriesMap &pulses,
                                        const I3TimeWindow &tw) {
  return GetResponsesInTimeRange<I3DOMLaunch> (pulses, tw);
};

I3MCPulseSeriesMap Clip_I3MCPulses (const I3MCPulseSeriesMap &pulses,
                                        const I3TimeWindow &tw) {
  return GetResponsesInTimeRange<I3MCPulse> (pulses, tw);
};


//Do the pybindings
void register_ResponseMapHelpers() {
  bp::def("clip_I3RecoPulses", &Clip_I3RecoPulses
    , (bp::arg("pulsemap"), bp::arg("timewindow"))
    , "Clips a I3RecoPulseSeriesMap based on time window."
  );
  
  bp::def("clip_I3DOMLaunches", &Clip_I3DOMLaunches
    , (bp::arg("launchmap"), bp::arg("timewindow"))
    , "Clips a I3DOMLaunchSeriesMap based on time window."
  );
  
  bp::def("clip_I3MCPulses", &Clip_I3MCPulses
    , (bp::arg("launchmap"), bp::arg("timewindow"))
    , "Clips a I3MCPulseSeriesMap based on time window."
  );
  
    bp::def("GetMaskAncestry", &GetMaskAncestry,
          (bp::arg("frame"), bp::arg("key")),
          "get a complete list of the maps that this possible mask found at 'key' derives from.");

  bp::def("GetCommonMaskAncestry", &GetCommonMaskAncestry,
          (bp::arg("frame"), bp::arg("keyA"), bp::arg("keyB")),
          "get a complete list of ancestors which are common to these two possible masks.");
  
  bp::def("UniteRecoMaps", &UniteRecoMaps,
          (bp::arg("mapA"), bp::arg("mapB")),
          "A convenience function to unite two RecoMaps into a single one");
};

