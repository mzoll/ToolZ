/**
 * @file HitSorting.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: HitSorting.h 153711 2017-03-03 11:19:05Z mzoll $
 * @version $Revision: 153711 $
 * @date $Date: 2017-03-03 12:19:05 +0100 (Fri, 03 Mar 2017) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef HITSORTING_H
#define HITSORTING_H

static const unsigned hitsorting_version_ = 0;

#include <numeric>
#include <queue>

#include "OMKeyHash.h"

#include "dataclasses/physics/I3RecoPulse.h"
#include "dataclasses/I3MapOMKeyMask.h" //one of the bitmasks that is explicitly supported
#include "dataclasses/physics/I3DOMLaunch.h"
#include "dataclasses/physics/I3MCHit.h"
#include "simclasses/I3MCPulse.h"

#include "ToolZ/Hitclasses.h"

///provides functionality to extract and sort Hit-objects
namespace HitSorting {
  //==================== Convert between OMKeyMaps and Hit derivates ============
  
  /** Convert a ResponsePulseSeriesMap into a vector of pair-like HitObjectOriginals
   * NOTE the map the objects are created from MUST NOT move in memory
   * @param responsemap the Pulses to buffer up
   * @return the retrieval ordered(if not something else) HitObjects
   */
  template <class Response, class HitObjectContainer>
  HitObjectContainer
  OMKeyMap_To_HitObjects (const I3Map<OMKey, std::vector<Response> > &responsemap);
  
  /** Convert a ResponsePulseSeriesMap into a vector of pair-like HitObjectOriginals
  * @param responsemap the Pulses to buffer up
  * @return the retrieval ordered(if not something else) HitObjectOriginals
  */
  template <class Response, class HitObjectOriginalContainer>
  HitObjectOriginalContainer
  OMKeyMap_To_HitObjectOriginals (const I3Map<OMKey, std::vector<Response> > &responsemap);
  
  /** @brief Convert a series of HitObjects back to a native ResponseSeriesMap 
  * @param hits the HitObjects to revert
  * @return the ResponseMap of reverted Hits
  */
  template <class Response, class HitObjectContainer>
  I3Map<OMKey, std::vector<Response> >
  HitObjects_To_OMKeyMap (const HitObjectContainer &hitsobjects);
  
  /** Convert a series of HitObjects to AbsHits 
   * @param hoc the hitobjectcontainer
   */
  template<class HitObjectContainer, class AbsHitContainer>
  AbsHitContainer
  HitObjects_To_AbsHits (
    const HitObjectContainer& hoc,
    const CompactOMKeyHashServiceConstPtr& hasher);
  
  /** Convert a series of HitObjects to AbsDAQHits 
   * @param hoc the hitobjectcontainer
   * @param hasher OMKeyHasher
   */
  template<class HitObjectContainer, class AbsDAQHitContainer>
  AbsDAQHitContainer
  HitObjects_To_AbsDAQHits (
    const HitObjectContainer& hoc,
    const CompactOMKeyHashServiceConstPtr& hasher);
  
  /** Convert a series of HitObjects to Hits 
   * @param hoc the hitobjectcontainer
   * @param hasher OMKeyHasher
   */
  template<class HitObjectContainer, class HitContainer>
  HitContainer
  HitObjects_To_Hits (
    const HitObjectContainer& hoc,
    const CompactOMKeyHashServiceConstPtr& hasher);
  
 /** @brief Convert a series of HitObjects back to a native ResponseSeriesMap 
  * @param hits the HitObjects to revert
  * @return the ResponseMap of reverted Hits
  */
  template <class Response, class HitContainer>
  I3Map<OMKey, std::vector<Response> >
  Hits_To_OMKeyMap (
    const HitContainer &hits,
    const CompactOMKeyHashServiceConstPtr& hasher);
};


//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

template <class Response, class HitObjectContainer>
HitObjectContainer
HitSorting::OMKeyMap_To_HitObjects (const I3Map<OMKey, std::vector<Response> > &pulses)
{
  HitObjectContainer hitObjs;

  for (typename I3Map<OMKey, std::vector<Response> >::const_iterator domIt=pulses.begin(); domIt!=pulses.end(); ++domIt) {
    const OMKey& omkey = domIt->first;
    for (typename std::vector<Response>::const_iterator pulseIt =domIt->second.begin() ; pulseIt!=domIt->second.end(); ++pulseIt) {
      hitObjs.insert(hitObjs.end(), HitObject<Response>(omkey, *pulseIt));
    }
  }
  
  return hitObjs;
};


template <class Response, class HitObjectOriginalContainer>
HitObjectOriginalContainer
HitSorting::OMKeyMap_To_HitObjectOriginals (const I3Map<OMKey, std::vector<Response> > &pulses)
{
  HitObjectOriginalContainer hitObjs;

  for (typename I3Map<OMKey, std::vector<Response> >::const_iterator domIt=pulses.begin(); domIt!=pulses.end(); ++domIt) {
    const OMKey& omkey = domIt->first;
    for (typename std::vector<Response>::const_iterator pulseIt =domIt->second.begin() ; pulseIt!=domIt->second.end(); ++pulseIt) {
      hitObjs.insert(hitObjs.end(), HitObjectOriginal<Response>(omkey, *pulseIt));
    }
  }
  
  return hitObjs;
};


template <class Response, class HitObjectContainer>
I3Map<OMKey, std::vector<Response> >
HitSorting::HitObjects_To_OMKeyMap (const HitObjectContainer &hitObjs)
{
  I3Map<OMKey, std::vector<Response> > responseMap;
  
  typedef std::set<HitObject<Response>, typename HitObject<Response>::RetrievalOrdered> HitObjectsRO;
  HitObjectsRO ro_ho(hitObjs.begin(), hitObjs.end());
  for (typename HitObjectsRO::const_iterator ho_iter=ro_ho.begin(); ho_iter!=ro_ho.end(); ++ho_iter) {
    responseMap[ho_iter->GetOMKey()].push_back(ho_iter->GetResponseObj());
  }
  
  return responseMap;
};


template<class HitObjectContainer, class AbsHitContainer>
AbsHitContainer
HitSorting::HitObjects_To_AbsHits (
  const HitObjectContainer& hoc,
  const CompactOMKeyHashServiceConstPtr& hasher) 
{
  AbsHitContainer ahc;
  for (typename HitObjectContainer::const_iterator hoc_iter=hoc.begin(); hoc_iter!=hoc.end(); ++hoc_iter)
    ahc.insert(ahc.end(), AbsHit(hasher->HashFromOMKey(hoc_iter->GetOMKey()), hoc_iter->GetTime()));
  return ahc;
};


template<class HitObjectContainer, class AbsDAQHitContainer>
AbsDAQHitContainer
HitSorting::HitObjects_To_AbsDAQHits (
  const HitObjectContainer& hoc,
  const CompactOMKeyHashServiceConstPtr& hasher) 
{
  AbsDAQHitContainer ahc;
  for (typename HitObjectContainer::const_iterator hoc_iter=hoc.begin(); hoc_iter!=hoc.end(); ++hoc_iter)
    ahc.insert(ahc.end(), AbsDAQHit(hasher->HashFromOMKey(hoc_iter->GetOMKey()), hoc_iter->GetDAQTicks()));
  return ahc;
};

template<class HitObjectContainer, class HitContainer>
HitContainer
HitSorting::HitObjects_To_Hits(
  const HitObjectContainer& hoc,
  const CompactOMKeyHashServiceConstPtr& hasher) 
{
  HitContainer hc;
  for (typename HitObjectContainer::const_iterator hoc_iter=hoc.begin(); hoc_iter!=hoc.end(); ++hoc_iter) {
    hc.insert(hc.end(), Hit(hasher->HashFromOMKey(hoc_iter->GetOMKey()), hoc_iter->GetTime(), *hoc_iter));
  }
  return hc;
};

template <class Response, class HitContainer>
I3Map<OMKey, std::vector<Response> >
HitSorting::Hits_To_OMKeyMap (
  const HitContainer &hits,
  const CompactOMKeyHashServiceConstPtr& hasher)
{
  I3Map<OMKey, std::vector<Response> > responseMap;
  
  typedef std::set<Hit, Hit::RetrievalOrdered> RO_Hits;
  RO_Hits ro_hits(hits.begin(), hits.end());
  for (RO_Hits::const_iterator h_iter=ro_hits.begin(); h_iter!=ro_hits.end(); ++h_iter) {
    const HitObject<Response>& ho = h_iter->GetAssociatedHitObject<Response>();
    responseMap[ho.GetOMKey()].push_back(ho.GetResponseObj());
  }
  
  return responseMap;
};

#endif //HITSORTING_H