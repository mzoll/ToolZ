/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: FrameRegisterModule.cxx 151333 2016-11-04 13:59:59Z mzoll $
 *
 * @file FrameRegister.cxx
 * @version $Revision: 1.00$
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 */

#include "ToolZ/FrameRegister.h"

#include <algorithm>
#include <boost/foreach.hpp>

using namespace std;

PosFrame nowherePosFrame() {
  I3FramePtr nowhere;
  return std::make_pair(-1, nowhere);
};

//____________________________________________________________________________
FrameRegister::FrameRegister(const FramePack& packet)
: frames_(packet.begin(), packet.end()), //copy the content 1-to-1
  runID_(-1),
  eventID_(-1)
{
  const I3FramePtr frame = frames_[0];
  //its the Q-Frame: try to locate special objects [splitName]SplitCount and [splitName]ReducedCount
  if (frame->GetStop() != I3Frame::DAQ)
    log_error("potential error loading and registering the series of frames!");
    
  const I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>("I3EventHeader");
  if (!eh)
    log_fatal("Frame does not contain an valid I3EventHeader");
  
  runID_ = eh->GetRunID();
  eventID_ = eh->GetEventID();
  
  BuildSubEventFramesMap();
};

//____________________________________________________________________________
void FrameRegister::BuildSubEventFramesMap() {
  for (size_t frame_index = 0; frame_index < frames_.size(); frame_index++) {
    const I3FramePtr& frame = frames_.at(frame_index);
    
    //its the Q-Frame: try to locate special objects [splitName]SplitCount and [splitName]ReducedCount
    if (frame->GetStop() == I3Frame::DAQ) {
      if (frame_index!=0)
        log_fatal("potential error loading and registering the series of frames!");
    }
    // if the frame is Physics: see if its a SplitFrame or HypoFrame
    else if (frame->GetStop() == I3Frame::Physics) { 
      const I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>("I3EventHeader");
      if (!eh)
        log_fatal("Frame does not contain an valid I3EventHeader");
      
      const string this_stream = eh->GetSubEventStream();
      const unsigned this_ID = eh->GetSubEventID();
      int last_ID;
      if (subEventFramesMap_.count(eh->GetSubEventStream()))
        last_ID = subEventFramesMap_[this_stream].size()-1;
      else  //no entry for this subeventStream yet
        last_ID = -1;
      
      int ID_diff = this_ID-last_ID;
      if (ID_diff <=0)
        log_fatal("Error in chronologic order of frames");
      else if (ID_diff==1)  
        subEventFramesMap_[this_stream].push_back(std::make_pair(frame_index, frame));
      else {
        while (ID_diff>1 ) {
          subEventFramesMap_[this_stream].push_back(nowherePosFrame());
          ID_diff--;
        };
        subEventFramesMap_[this_stream].push_back(std::make_pair(frame_index, frame));
      }
    }
  }
}

//____________________________________________________________________________
void FrameRegister::clear() {
  frames_.clear();
  subEventFramesMap_.clear();
}
  
//____________________________________________________________________________
void FrameRegister::InsertFrame(const I3FramePtr& frame) {
  log_debug("Entering InsertFrame");
  if (frame->GetStop()!=I3Frame::Physics)
    log_fatal("Can only insert Physics frames");
  
  const I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>("I3EventHeader");
  if (!eh)
    log_fatal("Frame does not contain an valid I3EventHeader");
  
  if (eh->GetEventID()!=eventID_) {
    log_error_stream(eh->GetEventID()<<" "<< eventID_);
    log_fatal("EventID for new frame and FrameRegister differ");
  }
  if (eh->GetRunID()!=runID_) {
    log_error_stream(eh->GetRunID()<< runID_);
    log_fatal("EventID for new frame and FrameRegister differ");
  }
  
  //determine where to insert this new frame
  const string this_stream = eh->GetSubEventStream();
  const unsigned this_ID = eh->GetSubEventID();
  int last_ID;
  if (! subEventFramesMap_.count(eh->GetSubEventStream())) {
    //no entry for this subeventStream yet
    //just push this frame at the end of the P frames in frames_ and register its new stream
    
    FramePack tmp_frames; tmp_frames.reserve(frames_.size()+1);
    
    int last_valid_position = frames_.size()-1;
    while (last_valid_position>0 && frames_[last_valid_position]->GetStop()!=I3Frame::Physics) {
      last_valid_position--;
    }
    
    for (int head=0; head<=last_valid_position; head++)
      tmp_frames.push_back(frames_[head]);
    tmp_frames.push_back(frame);
    for (size_t tail=last_valid_position+1; tail<frames_.size(); tail++)
      tmp_frames.push_back(frames_[tail]);
    //congruent implementations
  //     tmp_frames.push_back(frames.begin(),frames.begin()+last_valid_position);
  //     tmp_frames.push_back(frame);
  //     tmp_frames.push_back(frames.begin()+last_valid_position+1,frames.end()-1);
    frames_= tmp_frames;

    const size_t frame_index = last_valid_position+1;
    for (size_t pushin=0; pushin < this_ID; pushin++) {
      subEventFramesMap_[this_stream].push_back(nowherePosFrame());
    }
    subEventFramesMap_[this_stream].push_back(std::make_pair(frame_index, frame));
    return;
  }

  //frame is of type of an already registered subEventStream
  last_ID = subEventFramesMap_[this_stream].size()-1;
  
  //insert the frame at the right position into frames_
  int ID_diff = this_ID-last_ID;
  if (ID_diff <=0)
    log_fatal("Error in chronologic order of frames");
  
  //find last valid frame entry and position
  int last_valid_ID = subEventFramesMap_[this_stream].size()-1;
  while (last_valid_ID>=0 && subEventFramesMap_[this_stream][last_valid_ID].first==-1) {
    last_valid_ID--;
  }
  size_t last_valid_position = subEventFramesMap_[this_stream][last_valid_ID].first;
  
  while (ID_diff>1 ) {
    subEventFramesMap_[this_stream].push_back(nowherePosFrame());
    ID_diff--;
  };
  subEventFramesMap_[this_stream].push_back(std::make_pair(last_valid_position+1, frame));
  
  //also enter the new frames into the frames_ register at position last_valid_position
  FramePack tmp_frames; tmp_frames.reserve(frames_.size()+1);
  for (size_t head=0; head<=last_valid_position; head++)
    tmp_frames.push_back(frames_[head]);
  tmp_frames.push_back(frame);
  for (size_t tail=last_valid_position+1; tail<frames_.size(); tail++)
    tmp_frames.push_back(frames_[tail]);
  //congruent implementations
//     tmp_frames.push_back(frames.begin(),frames.begin()+last_valid_position);
//     tmp_frames.push_back(frame);
//     tmp_frames.push_back(frames.begin()+last_valid_position+1,frames.end()-1);
  frames_= tmp_frames;
};

//____________________________________________________________________________
std::vector<I3FramePtr> FrameRegister::RetrieveFrames() const {
  return frames_;
};

//____________________________________________________________________________
void FrameRegister::RemoveFrame(const FrameIndex frameindex) {
  assert(frameindex<frames_.size());
  if (frameindex==0) {
    log_fatal("removing the first frame identical to the Qframe will destroy coherence in the register");
    return;
  }
  frames_.erase(frames_.begin()+frameindex);
  //congruent implementations
//   for (size_t tail=frameindex; tail<=frames_.size()-2; tail++)
//     frames_[tail]=frames_[tail+1]);
//   frames_.erase(frames_.rbegin());
};

  
void FrameRegister::RemoveFrames(const std::vector<FrameIndex> frameindexes) {
  std::set<FrameIndex> frameindexe_set;
  BOOST_FOREACH(const FrameIndex& frameindex, frameindexes)
    frameindexe_set.insert(frameindex);
  RemoveFrames(frameindexe_set);
};
    
void FrameRegister::RemoveFrames(const std::list<FrameIndex> frameindexes) {
  std::set<FrameIndex> frameindexe_set;
  BOOST_FOREACH(const FrameIndex& frameindex, frameindexes)
    frameindexe_set.insert(frameindex);
  RemoveFrames(frameindexe_set);
};

//____________________________________________________________________________
void FrameRegister::RemoveFrames(const std::set<FrameIndex> frameindexes) {
  if (frameindexes.empty())
    return;
  
  std::set<FrameIndex>::const_reverse_iterator riter = frameindexes.rbegin();
  const std::set<FrameIndex>::const_reverse_iterator rend = frameindexes.rend();
  while (riter!= rend) {
    const FrameIndex& frameindex = *riter;
    assert(frameindex<frames_.size());
    if (frameindex==0) {
      log_fatal("removing the first frame identical to the Qframe will destroy coherence in the register");
      return;
    }
    frames_.erase(frames_.begin()+frameindex);
    riter++;
  }
  //congruent implementations
//   FramePack tmp_frames; tmp_frames.reserve(frames_.size()-frameindexes.size());
//   size_t head=0;
//   FramePack::const_iterator frameindexes_iter= frameindexes.begin();
//   while (head<frames_.size()) {
//     if (head== *frameindexes_iter) {
//       //hopp this one
//       head++;
//       if (frameindexes_iter!=frameindexes_iter.end()-1)
//         frameindexes_iter++;
//     }
//     tmp_frames.push_back(frames_[head]);
//   }
  subEventFramesMap_.clear();
  BuildSubEventFramesMap();
};

I3FramePtr FrameRegister::GetFrame(const size_t position) const {
  assert(position<frames_.size());
  return frames_.at(position);
}

PosFrame FrameRegister::GetSubEventPosFrame(
  const std::string& subEventStream,
  const unsigned subEventID) const 
{
  assert(subEventFramesMap_.count(subEventStream));
  assert(subEventID < subEventFramesMap_.at(subEventStream).size());
  return subEventFramesMap_.at(subEventStream).at(subEventID);
};

I3FramePtr FrameRegister::GetSubEventFrame(
  const std::string& subEventStream,
  const unsigned subEventID) const 
{ return GetSubEventPosFrame(subEventStream, subEventID).second; };

const I3FramePtr& FrameRegister::GetQFrame() const {
  assert(frames_.size()>0);
  assert(frames_.at(0)->GetStop()==I3Frame::DAQ);
  return frames_.at(0);
};

void FrameRegister::Report() const {
  log_info_stream("FramgeRegister for "<<GetRunID()<<":"<<GetEventID()<<"::");
  
  BOOST_FOREACH(const I3FramePtr& frame, frames_) {
    if (frame->GetStop()==I3Frame::DAQ) {
      I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>();
      log_info_stream("Q-frame: "<<eh->GetRunID()<<":"<<eh->GetEventID()<<"-"<<eh->GetSubEventStream()<<":"<< eh->GetSubEventID());
    }
    else if (frame->GetStop()==I3Frame::Physics) {
      I3EventHeaderConstPtr eh = frame->Get<I3EventHeaderConstPtr>();
      log_info_stream("P-frame: "<<eh->GetRunID()<<":"<<eh->GetEventID()<<"-"<<eh->GetSubEventStream()<<":"<< eh->GetSubEventID());
    }
    else
      log_info_stream(frame->GetStop().str()<<"-frame");
  }  
};
