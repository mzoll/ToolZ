/**
* \file HitSorting.cxx
*
* (c) 2012 the IceCube Collaboration
*
* $Id: HitFacility.cxx 153492 2017-02-23 16:51:31Z mzoll $
* \version $Revision: 153492 $
* \date $Date: 2017-02-23 17:51:31 +0100 (Thu, 23 Feb 2017) $
* \author Marcel Zoll <marcel.zoll@fysik.su.se>
*/

#include "ToolZ/HitFacility.h"

using namespace HitSorting;
using namespace std;

//========== CLASS I3RecoPulseSeriesMapHitFacility =============
I3RecoPulseSeriesMap_HitFacility::I3RecoPulseSeriesMap_HitFacility(
  const I3FrameConstPtr& frame,
  const std::string& key,
  CompactOMKeyHashServiceConstPtr& hasher)
: OMKeyMap_HitFacility<I3RecoPulse>(frame, key, hasher)
{};

I3RecoPulseSeriesMap_HitFacility::I3RecoPulseSeriesMap_HitFacility(
  const I3FrameConstPtr& frame,
  const std::string& key,
  const CompactOMKeyHashServiceConstPtr& hasher)
: OMKeyMap_HitFacility<I3RecoPulse>(frame, key, hasher)
{};