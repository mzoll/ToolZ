/**
 * \file GCDinfo.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * pack up the GCD info in a struct
 */

#ifndef GCDINFO_H
#define GCDINFO_H

#include "ToolZ/__SERIALIZATION.h"

#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/calibration/I3Calibration.h"
#include "dataclasses/status/I3DetectorStatus.h"

static const unsigned gcdinfo_version_ = 0;

class GCDinfo {
public:  
  const I3GeometryConstPtr geometry_;
  const I3CalibrationConstPtr calibration_;
  const I3DetectorStatusConstPtr status_;
  
public: //constructor
  GCDinfo(); //TODO depricated
  
  GCDinfo(
    const I3GeometryConstPtr& geo,
    const I3CalibrationConstPtr& calibration,
    const I3DetectorStatusConstPtr& status);
};

#endif //GCDINFO_H