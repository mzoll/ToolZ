/**
 * $Id: FrameRegisterTest.cxx 104975 2013-06-03 18:46:34Z mzoll $
 * $Author: mzoll <marcel.zoll@fysdik.su.se> $
 * $Date: 2013-06-03 20:46:34 +0200 (Mon, 03 Jun 2013) $
 * $Revision: 104975 $
 *
 * Test the functionality of the FrameRegister Class
 */

#include <I3Test.h>

#include "ToolZ/FrameRegister.h"

#include "icetray/I3Frame.h"
#include "dataclasses/physics/I3EventHeader.h"

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

using namespace boost;
using namespace std;

I3FramePtr makeFrame(
  const unsigned runID,
  const unsigned eventID,
  const std::string& subEventStream = "",
  const unsigned subEventID = 0) 
{
  I3FramePtr frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  I3EventHeaderPtr eh = boost::make_shared<I3EventHeader>();
  eh->SetRunID(runID);
  eh->SetEventID(eventID);
  eh->SetSubEventStream(subEventStream);
  eh->SetSubEventID(subEventID);
  frame->Put("I3EventHeader", eh);
  return frame;
};


FramePack CreateScenario() {
  FramePack packet;
  
  I3FramePtr frame;
  I3EventHeader eh = I3EventHeader();
  
  eh.SetRunID(0);
  eh.SetEventID(0);
  
  frame = boost::make_shared<I3Frame>(I3Frame::DAQ);

  frame->Put("I3EventHeader", boost::make_shared<I3EventHeader>(eh));
  frame->Put("splitSplitCount", boost::make_shared<I3Int>(2));
  frame->Put("splitReducedCount", boost::make_shared<I3Int>(0));
  packet.push_back(frame);
  
  frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  eh.SetSubEventStream("split");
  eh.SetSubEventID(0);
  frame->Put("I3EventHeader", boost::make_shared<I3EventHeader>(eh));
  packet.push_back(frame);

  frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  eh.SetSubEventStream("split");
  eh.SetSubEventID(1);
  frame->Put("I3EventHeader", boost::make_shared<I3EventHeader>(eh));
  packet.push_back(frame);

  frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  eh.SetSubEventStream("hypo");
  eh.SetSubEventID(0);
  frame->Put("I3EventHeader", boost::make_shared<I3EventHeader>(eh));
  packet.push_back(frame);
  
  //insert another frame and see if its correctly added
  frame = boost::make_shared<I3Frame>(I3Frame::Physics);
  eh.SetSubEventStream("diff");
  eh.SetSubEventID(0);
  frame->Put("I3EventHeader", boost::make_shared<I3EventHeader>(eh));
  packet.push_back(frame);
  
  //insert another frame, which is not one of the special GCDQPI type, and see if its correctly added
  frame = boost::make_shared<I3Frame>('F');
  packet.push_back(frame);

  //FramePacket is: Q Ps Ps Ph Pd F
  log_debug("Test scenario created");
  return packet;
}

//==========================================
TEST_GROUP(FrameCombiner);

TEST (TestCase1) { //create some artificial frames and push them into the frame register
  FramePack packet = CreateScenario(); //create test scenario

  ///construct the Frame Register
  FrameRegister fr(packet); //use the direct constructor
  
  //check properties
  ENSURE_EQUAL(fr.GetNumberFrames(), packet.size());
  ENSURE_EQUAL(packet.at(0), fr.GetQFrame());
  ENSURE_EQUAL(fr.GetEventID(), 0);
  
  //retrieve the content of the frame register
  const FramePack content = fr.RetrieveFrames();
  ENSURE_EQUAL(content.size(), packet.size(), "Identical");
  for(size_t i=0; i<packet.size(); i++)
    ENSURE_EQUAL(content.at(i), packet.at(i));
  
  //random access
  for(size_t i=0; i<fr.GetNumberFrames(); i++)
    ENSURE_EQUAL(packet.at(i), fr.GetFrame(i));
  
  PosFrame test_posframe;
  test_posframe = fr.GetSubEventPosFrame("split", 0);
  ENSURE_EQUAL(test_posframe.first, 1);
  ENSURE_EQUAL(test_posframe.second, packet.at(1));
  
  test_posframe = fr.GetSubEventPosFrame("split", 1);
  ENSURE_EQUAL(test_posframe.first, 2);
  ENSURE_EQUAL(test_posframe.second, packet.at(2));
  
  test_posframe = fr.GetSubEventPosFrame("hypo", 0);
  ENSURE_EQUAL(test_posframe.first, 3);
  ENSURE_EQUAL(test_posframe.second, packet.at(3));
  
  test_posframe = fr.GetSubEventPosFrame("diff", 0);
  ENSURE_EQUAL(test_posframe.first, 4);
  ENSURE_EQUAL(test_posframe.second, packet.at(4));
  
  ENSURE_EQUAL(fr.GetSubEventFrame("split", 0), packet.at(1));
  ENSURE_EQUAL(fr.GetSubEventFrame("split", 1), packet.at(2));
  ENSURE_EQUAL(fr.GetSubEventFrame("hypo", 0), packet.at(3));
  ENSURE_EQUAL(fr.GetSubEventFrame("diff", 0), packet.at(4));
  
  //insert a new frame
  I3FramePtr new_split = makeFrame(0, 0, "split", 2);
  fr.InsertFrame(new_split);
  ENSURE_EQUAL(fr.GetNumberFrames(), packet.size()+1);
  test_posframe = fr.GetSubEventPosFrame("split", 2);
  ENSURE_EQUAL(test_posframe.first, 3);
  
  I3FramePtr new_more = makeFrame(0, 0, "more", 0);
  fr.InsertFrame(new_more);
  ENSURE_EQUAL(fr.GetNumberFrames(), packet.size()+2);
  test_posframe = fr.GetSubEventPosFrame("more", 0);
  ENSURE_EQUAL(test_posframe.first, 6);

  //removeFrames
  fr.RemoveFrame(3); //remove the just created frame 'split-2'
  ENSURE_EQUAL(fr.GetNumberFrames(), packet.size()+1);
  ENSURE_EQUAL(fr.GetFrame(5), new_more);
  
  std::vector<FrameIndex> discard_these;
  discard_these.push_back(1); discard_these.push_back(2); //remove the other splitframes
  fr.RemoveFrames(discard_these);
  
  ENSURE_EQUAL(fr.GetNumberFrames(), packet.size()-1);
  test_posframe = fr.GetSubEventPosFrame("hypo", 0);
  ENSURE_EQUAL(test_posframe.first, 1);
};