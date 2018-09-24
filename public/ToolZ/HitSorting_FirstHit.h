/**
 * @file HitSorting_FirstHit.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: HitSorting_FirstHit.h 153492 2017-02-23 16:51:31Z mzoll $
 * @version $Revision: 153492 $
 * @date $Date: 2017-02-23 17:51:31 +0100 (Thu, 23 Feb 2017) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef HITSORTING_FIRSTHIT_H
#define HITSORTING_FIRSTHIT_H

#include <numeric>

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/HitSorting.h"

#include "dataclasses/physics/I3RecoPulse.h" //one of templates that are explicitly supported


//============= HitSorting_FirstHit===================================

namespace HitSorting {
  /**
   * Accumulator a struct providing a call operator how a vector of Responses are accumulated into one single Response object
   */
  template <class Response>
  class Accumulator {
  public:
    Response operator()(const std::vector<Response>&) const;
  };
  
  template<>
  I3RecoPulse
  HitSorting::Accumulator<I3RecoPulse>::operator()(
    const std::vector<I3RecoPulse>& responsevec) const;
  
  template <class Response, class HitObjectOriginalContainer>
  HitObjectOriginalContainer
  OMKeyMap_FirstHits_To_HitObjectOriginals(
    const I3Map<OMKey, std::vector<I3RecoPulse> >& map);
  
  template <class Response, class AbsHitContainer>
  AbsHitContainer
  OMKeyMap_FirstHits_To_AbsHits(
    const CompactOMKeyHashServiceConstPtr& hasher,
    const I3Map<OMKey, std::vector<I3RecoPulse> >& map);

  template <class Response, class AbsDAQHitContainer>
  AbsDAQHitContainer
  OMKeyMap_FirstHits_To_AbsDAQHits(
    const CompactOMKeyHashServiceConstPtr& hasher,
    const I3Map<OMKey, std::vector<I3RecoPulse> >& map);
};


//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

template <class Response, class HitObjectOriginalContainer>
HitObjectOriginalContainer
HitSorting::OMKeyMap_FirstHits_To_HitObjectOriginals(
  const I3Map<OMKey, std::vector<I3RecoPulse> >& map)
{
  HitObjectOriginalContainer hitObjs;

  typedef I3Map<OMKey, std::vector<I3RecoPulse> > I3ResponseSeriesMap;
  typedef typename I3ResponseSeriesMap::value_type OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& entry, map) {
    if (entry.second.empty())
      continue;
    
    HitSorting::Accumulator<Response> accu;
    const Response acc_response = accu(entry.second);
    hitObjs.insert(hitObjs.end(), HitObjectOriginal<Response>(entry.first, acc_response) );
  }
  
  return hitObjs;
};

template <class Response, class AbsHitContainer>
AbsHitContainer
HitSorting::OMKeyMap_FirstHits_To_AbsHits(
  const CompactOMKeyHashServiceConstPtr& hasher,
  const I3Map<OMKey, std::vector<I3RecoPulse> >& map)
{
  AbsHitContainer hits;
  
  typedef I3Map<OMKey, std::vector<I3RecoPulse> > I3ResponseSeriesMap;
  typedef typename I3ResponseSeriesMap::value_type OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& o_rvec, map) {
    if (o_rvec.second.empty())
      continue;
    
    const Response& r = o_rvec.second.front();
    hits.insert(AbsHit(hasher->HashFromOMKey(o_rvec.first), GetInferredTime(r)));
  }
  return hits;
};

template <class Response, class AbsDAQHitContainer>
AbsDAQHitContainer
HitSorting::OMKeyMap_FirstHits_To_AbsDAQHits(
  const CompactOMKeyHashServiceConstPtr& hasher,
  const I3Map<OMKey, std::vector<I3RecoPulse> >& map)
{
  AbsDAQHitContainer hits;
  typedef I3Map<OMKey, std::vector<I3RecoPulse> > I3ResponseSeriesMap;
  typedef typename I3ResponseSeriesMap::value_type OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& o_rvec, map) {
    if (o_rvec.second.empty())
      continue;
    
    const Response& r = o_rvec.second.front();
    hits.insert(AbsDAQHit(hasher->HashFromOMKey(o_rvec.first), GetInferredDAQTicks(r)));
  }
  return hits;
};

#endif
