/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: I3RegisterModule.cxx 152616 2017-01-11 15:25:59Z mzoll $
 *
 * @file I3RegisterModule.cxx
 * @version $Revision: 1.00$
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 */

#include "ToolZ/I3RegisterModule.h"

#include <boost/foreach.hpp>

using namespace std;

I3RegisterModule::I3RegisterModule(const I3Context& context):
  I3PacketModule(context)
{
  AddOutBox("OutBox");
};

void I3RegisterModule::FramePacket(std::vector<I3FramePtr> &packet) {
  frameRegister_ = new FrameRegister(packet);
  Operate(); //operation implemented in here
  BOOST_FOREACH(const I3FramePtr& frame, frameRegister_->RetrieveFrames())
    PushFrame(frame);
  delete frameRegister_;
};

std::string I3RegisterModule::GetBufferedEventString() const {
  std::ostringstream oss;
  if (frameRegister_->GetNumberFrames()) 
    oss<<"Run"<<frameRegister_->GetRunID()<<":Event"<<frameRegister_->GetEventID();
  else
    oss<<"NONE";
  return oss.str();
};