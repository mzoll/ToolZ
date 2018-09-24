/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: CoincSuiteHelpers.cxx 151816 2016-11-28 12:50:15Z mzoll $
 *
 * @file MiscHelpers.cxx
 * @version $Revision: 1.00$
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 */

#include "ToolZ/MiscHelpers.h"

#include <sstream>

std::string FrameIDstring (const I3FrameConstPtr frame) {
  std::ostringstream oss;
  if (frame->GetStop() == I3Frame::Geometry)
    {oss << "Geometry";}
  else if (frame->GetStop() == I3Frame::Calibration)
    {oss << "Calibration";}
  else if (frame->GetStop() == I3Frame::DetectorStatus)
    {oss << "DetectorStatus";}
  else if (frame->GetStop() == I3Frame::DAQ)
    {oss << "DAQ";}
  else if (frame->GetStop() ==I3Frame::Physics)
    {oss << "Physics";}
  else
    {oss << frame->GetStop();}
    
  I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>("I3EventHeader");
  if (eh) {
    oss <<"::"<<eh->GetRunID()<<"/"<<eh->GetEventID()<<"_"<<eh->GetSubEventStream()<<"/"<<eh->GetSubEventID(); 
  }

  return oss.str();
};