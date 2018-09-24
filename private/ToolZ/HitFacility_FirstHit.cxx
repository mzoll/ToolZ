#include "ToolZ/HitFacility_FirstHit.h"


//========== CLASS I3RecoPulseSeriesMap_HitFacility_FirstHit =============

I3RecoPulseSeriesMap_HitFacility_FirstHit::I3RecoPulseSeriesMap_HitFacility_FirstHit(
  const I3FrameConstPtr& frame,
  const std::string& key,
  CompactOMKeyHashServiceConstPtr& hasher)
: OMKeyMap_HitFacility_FirstHit<I3RecoPulse>(frame, key, hasher)
{};

I3RecoPulseSeriesMap_HitFacility_FirstHit::I3RecoPulseSeriesMap_HitFacility_FirstHit(
  const I3FrameConstPtr& frame,
  const std::string& key,
  const CompactOMKeyHashServiceConstPtr& hasher)
: OMKeyMap_HitFacility_FirstHit<I3RecoPulse>(frame, key, hasher)
{};
