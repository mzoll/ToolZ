/**
 * \file GCDinfo.cxx
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 */

#include "ToolZ/GCDinfo.h"

GCDinfo::GCDinfo(
  const I3GeometryConstPtr& geometry,
  const I3CalibrationConstPtr& calibration,
  const I3DetectorStatusConstPtr& status)
: geometry_(geometry),
  calibration_(calibration),
  status_(status)
{};
