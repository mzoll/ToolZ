/**
 * \file ResponseMapHelpers.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: ResponseMapHelpers.h 153958 2017-03-17 13:42:47Z mzoll $
 * \version $Revision: 153958 $
 * \date $Date: 2017-03-17 14:42:47 +0100 (Fri, 17 Mar 2017) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Helper functions for the small man that wants to perform some common tasks
 */

#ifndef RESPONSEMAPHELPERS_H
#define RESPONSEMAPHELPERS_H

static const unsigned responsemaphelpers_version_ = 0;

#include "icetray/OMKey.h"

#include "dataclasses/I3Map.h"
#include "dataclasses/I3MapOMKeyMask.h"
#include "dataclasses/physics/I3RecoPulse.h"
#include "dataclasses/I3TimeWindow.h"

/** @brief Give me all pulses from this ResponseMap which fall within (inclusive) this time-window 
  *  A simple selection function
  *  @param responseSeriesMap from this pulses
  *  @param tw_start starting at this time
  *  @param tw_stop ending at this time
  *  @return the subset of pulses
  */
template <class Response>
I3Map<OMKey,std::vector<Response> > GetResponsesInTimeRange (
  const I3Map<OMKey,std::vector<Response> >& responseSeriesMap,
  const double tw_start,
  const double tw_stop);

/** @brief Give me all pulses from this ResponseMap, which fall within (inclusive) this time-window
  * A simple selection function
  *  @param responseSeriesMap from this pulses
  *  @param tw within this time-window
  *  @return the subset of pulses
  */
template <class Response>
I3Map<OMKey,std::vector<Response> > GetResponsesInTimeRange (
  const I3Map<OMKey,std::vector<Response> >& responseSeriesMap,
  const I3TimeWindow &tw);

/** Get a list of all derivatives of this mask
  * @param frame The frame which is searched for the the mask and its ancestors
  * @param key The name of a I3RecoPulseSeriesMap or I3RecoPulseSeriesMapMask
  * @return the ancestry of this mask, increasingly removed the deeper
  */
std::vector<std::string> GetMaskAncestry (
  I3FrameConstPtr frame,
  const std::string &key);

/** Get me the common ancestors for masks
  * @param frame The frame which is searched for the the mask and its ancestors
  * @param key1 the one I3RecoPulseSeriesMapMask
  * @param key2 the other I3RecoPulseSeriesMapMask
  * @return the common ancestry of this mask, increasingly removed the deeper
  */
std::vector<std::string> GetCommonMaskAncestry (
  I3FrameConstPtr frame,
  const std::string &key1,
  const std::string &key2);


/// an convenience time order operator
bool I3RecoPulse_timeorder (const I3RecoPulse& lhs, const I3RecoPulse& rhs);

/** @brief A convenience function to unite two RecoMaps into a single one
  * @param mapA the one map
  * @param mapB the other map
  * @return the united map
  */
I3RecoPulseSeriesMap UniteRecoMaps (
  const I3RecoPulseSeriesMap& mapA,
  const I3RecoPulseSeriesMap& mapB);


/** @brief A convenience function to unite multiple RecoMaps found in frames to one Mask pointing to a q-frame
  * @note it is assumed that all Masks/Maps have similar derived structure
  * @param key the key to the map or mask object in the frames
  * @param qframe the Q frame the frames derived from
  * @param frames a number of frames where maps/masks should be reunited
  * @return the united mask
  */
I3RecoPulseSeriesMapMask UniteRecoMaps_To_Mask(
  const std::string &key,
  const I3FramePtr qframe,
  const std::vector<I3FramePtr> &frames);

//=================================================================
//=================== IMPLEMENTATIONS =============================
//=================================================================

#include "ToolZ//HitSorting.h"
#include <boost/foreach.hpp>

//================ ICEHIVE_HELPERS ======================

template <class Response>
I3Map<OMKey, std::vector<Response> >
GetResponsesInTimeRange (const I3Map<OMKey, std::vector<Response> > &pulses,
                                      const double tw_start,
                                      const double tw_stop) {
  typedef std::vector<Response> ResponseSeries;
  typedef I3Map<OMKey,ResponseSeries> ResponseSeriesMap;
  ResponseSeriesMap pulses_inRange;
  BOOST_FOREACH(const typename ResponseSeriesMap::value_type &omkey_pulseseries, pulses) {
    const OMKey& omkey = omkey_pulseseries.first;
    BOOST_FOREACH(const Response& pulse, omkey_pulseseries.second) {
      const double time = GetInferredTime(pulse);
      if (tw_start <= time && time <= tw_stop) {
        pulses_inRange[omkey].push_back(pulse);
      }
    }
  }
  return pulses_inRange;
};


template <class Response>
I3Map<OMKey,std::vector<Response> > GetResponsesInTimeRange (
  const I3Map<OMKey,std::vector<Response> > &responseSeriesMap,
  const I3TimeWindow &tw)
  {return GetResponsesInTimeRange(responseSeriesMap, tw.GetStart(), tw.GetStop());};

  
inline 
bool I3RecoPulse_timeorder (const I3RecoPulse& lhs, const I3RecoPulse& rhs)
  { return lhs.GetTime()<rhs.GetTime(); };  
  

#endif // RESPONSEMAPHELPERS_H
