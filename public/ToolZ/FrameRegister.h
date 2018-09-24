/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: FrameCombiner.h 122365 2014-08-11 13:21:53Z mzoll $
 *
 * @file FrameRegister.h
 * @version $Revision: 122365 $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 *
 * The main purpose of this class and collection of functions is to provide a unified interface and random access
 * to all frames that come along in a single FramePackage, for example delivered from the I3FramePacket Module
 * Each FramePackage is therefore buffered up in an ordered register 'FrameRegister', 
 * and random access is granted to all substreams in the P-frames
 */

#ifndef FRAMEREGISTER_H
#define FRAMEREGISTER_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

#include "icetray/I3Frame.h"
#include "icetray/I3Int.h"
#include "dataclasses/physics/I3EventHeader.h"

/// an object that buffers up all frames in a ordered structure, not different to FramePacket
typedef std::vector<I3FramePtr> FramePack; //NOTE the should be a global alias/typedef for this in I3Frame.h
/// an alias for accesses
typedef size_t FrameIndex;
/// an alias for position of an frame in a bigger series; -1 signals that this is a placeholder
typedef int FramePosition;
/// an object that remembers the (original) position[0,...] of a given frame in the FramePack(et), and the pointer to it
typedef std::pair<FramePosition, I3FramePtr> PosFrame;
/// a series of PosFrames of common traits
typedef std::vector<PosFrame> PosFrameSeries;

/** @brief a class which can buffer up and hold all frames in that package
  * consists of multiple vectors of PosFrames, which holds frames which fulfil certain criteria e.g. belong to a certain SubEventStream
  */
class FrameRegister {
public: // Properties //FIXME move private
  /// holds all Frames there are
  std::vector<I3FramePtr> frames_;
  ///the governing runID
  int64_t runID_;
  ///the governing eventID
  int64_t eventID_;
  ///holds all P-frames sorted for their subevent-stream
  std::map<std::string, PosFrameSeries> subEventFramesMap_;
public:
  /// constructor (building the FrameRegister directly)
  /// @param packet the FramePacket to fill the FrameRegister from
  FrameRegister(const FramePack& packet);
  
  /// reset everything : purge all objects and all variables
  void clear();
private:  
  /// build the internal register
  void BuildSubEventFramesMap();  
  
public:
  /// insert a singule frame (can only be of 'P' type)
  /// @param frame the frame to insert
  void InsertFrame(const I3FramePtr& frame);
  
  /// Get the Qframe, identical to the first frame in the packet
  const I3FramePtr& GetQFrame() const;
  ///retrieve the content of the frame register
  FramePack RetrieveFrames() const;

  /// Remove the frame at this position
  /// @param frameindex the position of the frame to remove
  void RemoveFrame(const size_t frameindex);
  /// remove a number of frames from the register, the register will be rebuild after this
  void RemoveFrames(const std::vector<FrameIndex> frameindexes);
  void RemoveFrames(const std::list<FrameIndex> frameindexes);
  void RemoveFrames(const std::set<FrameIndex> frameindexes);
  
  /// Get me the entry at this position
  I3FramePtr GetFrame(const FrameIndex position) const;
  
  /// find the PosFrame with the following subevent stream and ID
  /// @param subEventStream the stream to search for; special for "Q" which will give the Q-frame
  /// @param subEventID the ID to search for
  PosFrame GetSubEventPosFrame(
    const std::string& subEventStream,
    const unsigned subEventID) const;
  /// find the Frame with the following subevent stream and ID
  /// @param subEventStream the stream to search for; special for "Q" which will give the Q-frame
  /// @param subEventID the ID to search for
  I3FramePtr GetSubEventFrame(
    const std::string& subEventStream,
    const unsigned subEventID) const;
    
  ///Get the number of buffered frames in the register
  size_t GetNumberFrames() const;
  ///Get the run ID for this Q-frame and all its subframes in this register
  unsigned GetRunID() const;
  ///Get the event ID for this Q-frame and all its subframes in this register
  unsigned GetEventID() const;
  
  void Report() const;
};

//=============================================================
//=================== IMPLEMENTATIONS =========================
//=============================================================

inline
size_t FrameRegister::GetNumberFrames() const
  {return frames_.size();};

inline
unsigned FrameRegister::GetRunID() const
  {assert(eventID_>=0); return runID_;};
  
inline
unsigned FrameRegister::GetEventID() const
  {assert(eventID_>=0); return eventID_;};

#endif // FRAMEREGISTER