/**
 * copyright (C) 2011
 * the IceCube collaboration
 * $Id: I3RegisterModule.h 152616 2017-01-11 15:25:59Z mzoll $
 *
 * @file FrameRegisterModule.h
 * @version $Revision: 152616 $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * @date $Date: Nov 30 2011
 *
 * The main purpose of this class and collection of functions is to provide a unified interface and random access
 * to all frames that come along in a single FramePackage.
 * Each FramePackage is therefore buffered up in an ordered register 'FrameRegister',
 * that indicates the frame-type (Q-frame, SplitFrame, HypoFrame) and its later purpose.
 */

#ifndef I3REGISTERMODULE_H
#define I3REGISTERMODULE_H

#include "icetray/I3PacketModule.h"

#include "ToolZ/FrameRegister.h"

/** A Module that buffers and registers a whole FramePackage and provides access to the frameRegister to execute operations
 */
class I3RegisterModule : public I3PacketModule {
  SET_LOGGER("I3RegisterModule");
protected: //properties
  /// create my most private instance of FrameRegister
  FrameRegister* frameRegister_;
public: //icetray interaction
  /// std constructor
  I3RegisterModule(const I3Context& context);
  /// get the framePack and push it in the frame register, do operations on it and then push it to the outbox
  /// @param packet the FramePacket
  void FramePacket(std::vector<I3FramePtr> &packet);
  
protected:
  /// THIS FUNCTION NEEDS TO IMPLEMENT OPERATIONS on the frameRegister
  virtual void Operate() = 0;
  
public: //convenience functions
  /// return length of vector AllFrames
  size_t GetNumberFrames() const;
  /// return a string with the Event identifier
  std::string GetBufferedEventString() const;
};

//==============================================================================
//============================= IMPLEMENTATIONS ================================
//==============================================================================

inline 
size_t I3RegisterModule::GetNumberFrames() const
  {return frameRegister_->GetNumberFrames();};

#endif //I3REGISTERMODULE_H
