/**
 * @file HitFacility.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: HitFacility.h 153711 2017-03-03 11:19:05Z mzoll $
 * @version $Revision: 153711 $
 * @date $Date: 2017-03-03 12:19:05 +0100 (Fri, 03 Mar 2017) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef HITFACILITY_H
#define HITFACILITY_H

static const unsigned hitfacility_version_ = 0;

#include <numeric>
#include <queue>

#include "ToolZ/HitSorting.h"

#include "dataclasses/physics/I3RecoPulse.h"
#include "dataclasses/I3MapOMKeyMask.h" //one of the bitmasks that is explicitly supported
#include "dataclasses/physics/I3DOMLaunch.h"
#include "dataclasses/physics/I3MCHit.h"
#include "simclasses/I3MCPulse.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


//===================== CLASS OMKeyMap_HitFacility =============================

/**
  * A class which helps to extract hits from an ResponseMap Object in the frame;
  * which also holds all the functionality so you do not do anything stupid
  * \template Response: an I3object of DOM-response type like I3DOMLaunch, I3MCHit, I3RecoPulse etc
  */
template<class Response>
class OMKeyMap_HitFacility {
  SET_LOGGER("OMKeyMap_HitFacility");
protected:
  typedef std::vector<Response> ResponseSeries;
  typedef I3Map<OMKey, ResponseSeries > I3ResponseSeriesMap;
  typedef HitObject<Response> ResponseHitObject;
  typedef std::set<ResponseHitObject, typename ResponseHitObject::RetrievalOrdered> ResponseHitObjectSetRO;
  typedef std::list<ResponseHitObject> ResponseHitObjectList; 
  
protected://properties/parameters
  ///PARAM: the frame this might have been extracted from
  const I3FrameConstPtr frame_;
  ///PARAM: name of this map in the frame
  const std::string key_;
  
protected://properties/internal
  ///hold a pointer to the OMKeyMap the Hits have been extracted from
  const boost::shared_ptr<const I3ResponseSeriesMap> map_;
  /// the hasher which resolves OMKeys and Hashes 
  const CompactOMKeyHashServiceConstPtr hasher_;
  /// here are the extracted objects
  mutable boost::shared_ptr<ResponseHitObjectList> hitObjects_;
  
public://methods
  /** constructor
    * @param frame frame to extract a map from 
    * @param key key in the frame at which we going to find a ResponseMap/Mask
    * @param hasher a hasher which can hash the omkeys, will be created on the spot if not existent  
    */
  OMKeyMap_HitFacility(
    const I3FrameConstPtr& frame,
    const std::string& key,
    CompactOMKeyHashServiceConstPtr& hasher);
  OMKeyMap_HitFacility(
    const I3FrameConstPtr& frame,
    const std::string& key,
    const CompactOMKeyHashServiceConstPtr& hasher);
  /// Extract Hit-objects from the ResponseMap and put/insert them into the container 
  template <class HitContainer>
  HitContainer GetHits() const;
  /// Extract AbsHit-objects from the ResponseMap and put/insert them into the container 
  template <class AbsHitContainer>
  AbsHitContainer GetAbsHits() const;
  /// Extract AbsDAQHit-objects from the ResponseMap and put/insert them into the container 
  template <class AbsDAQHitContainer>
  AbsDAQHitContainer GetAbsDAQHits() const;
  
  /// Revert once extracted Hits back to a subMap of the original ResonseMap
  /// \param hits a container with the hits to revert
  template<class HitContainer>
  I3ResponseSeriesMap
  MapFromHits (const HitContainer &hits) const;
  
  /// Revert once extracted Hits back to a subMap of the original ResonseMap
  /// \param abshits a container with the hits to revert
  template<class AbsHitContainer>
  I3ResponseSeriesMap
  MapFromAbsHits (const AbsHitContainer &abshits) const;
  
  /// Get the internally held hasher
  CompactOMKeyHashServiceConstPtr GetHasher() const;
  
  /// Get the HitObject associated to this hit
  /// \param h the hit //FIXME static
  const HitObject<Response>& GetHitObject(const Hit &h) const;
  
  //NOTE FUTURE here could be a general implementation of "MaskFromHits()", however the OMKeyMapMasks are not templated
  
protected:
  /// a callable stuct which can internally used for example with the Masking of I3RecoPulseSeriesMapMask
  struct predicate {
    ///keep a pointer to the outer facility
    const OMKeyMap_HitFacility* outer_;
    ///hold the hits which need to be backconverted
    const HitSetRO hits_;
    ///stepping iterator
    HitSetRO::const_iterator it_;
    ///marking the static end for the stepping operator
    const HitSetRO::const_iterator it_end_;
    
    ///constructor
    template <class HitContainer>
    predicate(const OMKeyMap_HitFacility* outer, const HitContainer &hits);
    
    ///call operator to this object
    bool operator() (const OMKey& o, size_t, const Response& r);
  };
  
  /// a callable stuct which can internally used for example with the Masking of I3RecoPulseSeriesMapMask
  struct predicate_abs {
    ///keep a pointer to the outer facility
    const OMKeyMap_HitFacility* outer_;
    ///hold the hits which need to be backconverted
    const AbsHitSetRO hits_;
    ///stepping iterator
    AbsHitSetRO::const_iterator it_;
    ///marking the static end for the stepping operator
    const AbsHitSetRO::const_iterator it_end_;
    
    ///constructor
    template <class HitContainer>
    predicate_abs(const OMKeyMap_HitFacility* outer, const HitContainer &hits);
    
    ///call operator to this object
    bool operator() (const OMKey& o, size_t, const Response& r);
  };
}; //class OMKeyMap_HitFacility

//shorthands
typedef OMKeyMap_HitFacility<I3DOMLaunch> I3DOMLaunchSeriesMap_HitFacility;
typedef OMKeyMap_HitFacility<I3MCHit> I3MCHitSeriesMap_HitFacility;
typedef OMKeyMap_HitFacility<I3MCPulse> I3MCPulseSeriesMap_HitFacility;

typedef boost::shared_ptr<I3DOMLaunchSeriesMap_HitFacility> I3DOMLaunchSeriesMap_HitFacilityPtr;
typedef boost::shared_ptr<const I3DOMLaunchSeriesMap_HitFacility> I3DOMLaunchSeriesMap_HitFacilityConstPtr;
typedef boost::shared_ptr<I3MCHitSeriesMap_HitFacility> I3MCHitSeriesMap_HitFacilityPtr;
typedef boost::shared_ptr<const I3MCHitSeriesMap_HitFacility> I3MCHitSeriesMap_HitFacilityConstPtr;
typedef boost::shared_ptr<I3MCPulseSeriesMap_HitFacility> I3MCPulseSeriesMap_HitFacilityPtr;
typedef boost::shared_ptr<const I3MCPulseSeriesMap_HitFacility> I3MCPulseSeriesMap_HitFacilityConstPtr;

//============ CLASS I3RecoPulseSeriesMap_HitFacility ===========

/**
  * specialize OMKeyMap_HitFacility to I3RecoPulse, and add the functionality to produce Masks
  */
class I3RecoPulseSeriesMap_HitFacility : public OMKeyMap_HitFacility<I3RecoPulse> {
  SET_LOGGER("I3RecoPulseSeriesMap_HitFacility");
public://methods
  /** constructor
    * @param frame frame to extract a map from 
    * @param key key in the frame at which we going to find a ResponseMap/Mask
    * @param hasher a hasher which can hash the omkeys, will be created on spot if not existent
    */
  I3RecoPulseSeriesMap_HitFacility(
    const I3FrameConstPtr& frame,
    const std::string &key,
    CompactOMKeyHashServiceConstPtr& hasher);
  
  I3RecoPulseSeriesMap_HitFacility(
    const I3FrameConstPtr& frame,
    const std::string &key,
    const CompactOMKeyHashServiceConstPtr& hasher);
  
  
  /** get the subset of hits which corresponds to the mask at this key
    * @param key a the key of a Map/mask that pulses should be extracted from;
    *  needs to inherit from map;
    *  blank parameter returns the Hits from the base-object map_
    */
  template <class HitContainer>
  HitContainer GetHits (const std::string &key) const;
  template <class HitContainer>
  HitContainer GetHits () const; //NOTE just returns result of base-classe method
  
  /** get the subset of hits which corresponds to the mask at this key
    * @param key a the key of a Map/mask that pulses should be extracted from;
    *  needs to inherit from map;
    *  blank parameter returns the AbsHits from the base-object map_
    */
  template <class AbsHitContainer>
  AbsHitContainer GetAbsHits (const std::string &key) const;
  template <class AbsHitContainer>
  AbsHitContainer GetAbsHits () const; //NOTE just returns result of base-classe method
  
  /// Revert once extracted Hits back to a subMap and a final ResponseMask of the original ResonseMap
  /// \param hits a container with the hits to revert
  template<class HitContainer>
  I3RecoPulseSeriesMapMask MaskFromHits (const HitContainer &hits) const;
  /// Revert once extracted Hits back to a subMap and a final ResponseMask of the original ResonseMap
  /// \param hits a container with the hits to revert
  template<class AbsHitContainer>
  I3RecoPulseSeriesMapMask MaskFromAbsHits (const AbsHitContainer &abshits) const;
};

typedef boost::shared_ptr<I3RecoPulseSeriesMap_HitFacility> I3RecoPulseSeriesMap_HitFacilityPtr;
typedef boost::shared_ptr<const I3RecoPulseSeriesMap_HitFacility> I3RecoPulseSeriesMap_HitFacilityConstPtr;


//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

#include <boost/make_shared.hpp>

//========== CLASS OMKeyMap_HitFacility =============

template <class Response>
OMKeyMap_HitFacility<Response>::OMKeyMap_HitFacility(
  const I3FrameConstPtr& frame,
  const std::string& key,
  CompactOMKeyHashServiceConstPtr& hasher)
: frame_(frame),
  key_(key),
  map_(frame_->Get<boost::shared_ptr<const I3ResponseSeriesMap> >(key_)),
  hasher_(hasher ? hasher : boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(*map_)))
  //hitObjects_(boost::make_shared<std::list<HitObject<Response> >(OMKeyMap_To_HitObjects<Response, std::list<HitObject<Response> > > (*map_))) //only created on demand
{
  if (!map_)
    log_fatal("No suitable map found at key '%s'", key_.c_str());
  hasher= hasher_;
};

template <class Response>
OMKeyMap_HitFacility<Response>::OMKeyMap_HitFacility(
  const I3FrameConstPtr& frame,
  const std::string& key,
  const CompactOMKeyHashServiceConstPtr& hasher)
: frame_(frame),
  key_(key),
  map_(frame_->Get<boost::shared_ptr<const I3ResponseSeriesMap> >(key_)),
  hasher_(hasher)
  //hitObjects_(boost::make_shared<std::list<HitObject<Response> >(OMKeyMap_To_HitObjects<Response, std::list<HitObject<Response> > > (*map_))) //only created on demand
{
  if (!map_)
    log_fatal("No suitable map found at key '%s'", key_.c_str());
};

template <class Response> template <class HitContainer>
HitContainer
OMKeyMap_HitFacility<Response>::GetHits() const {
  if (!hitObjects_) //if the pointer is not yet pointing to a valid object 
    hitObjects_ = boost::make_shared<ResponseHitObjectList>(HitSorting::OMKeyMap_To_HitObjects<Response, ResponseHitObjectList> (*map_));
  
  //make the according hits and link them
//   HitList hits;
//   BOOST_FOREACH(const ResponseHitObject &ho, hitObjects_)
//     hits.insert(hits.end(), Hit(hasher_->HashFromOMKey(ho.GetOMKey()), ho.GetTime(), ho));
//   return HitContainer(hits.begin(), hits.end());
  
  HitContainer hits;
  BOOST_FOREACH(const ResponseHitObject &ho, *hitObjects_)
    hits.insert(hits.end(), Hit(hasher_->HashFromOMKey(ho.GetOMKey()), ho.GetTime(), ho));
  return hits;
}

template <class Response> template <class AbsHitContainer>
AbsHitContainer
OMKeyMap_HitFacility<Response>::GetAbsHits() const {
//   if (!hitObjects_) //if the pointer is not yet pointing to a valid object 
//     hitObjects_ = boost::make_shared<ResponseHitObjectList>(HitSorting::OMKeyMap_To_HitObjects<Response, ResponseHitObjectList> (*map_));
//   
//   AbsHitContainer hits;
//   BOOST_FOREACH(const ResponseHitObject &ho, *hitObjects_)
//     hits.insert(hits.end(), AbsHit(hasher_->HashFromOMKey(ho.GetOMKey()), ho.GetTime()));
//   return hits;
  
  AbsHitContainer hits;
  typedef std::pair<OMKey, std::vector<Response> > OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& o_rvec, *map_) {
    BOOST_FOREACH(const Response& r, o_rvec.second) {
      hits.insert(hits.end(), AbsHit(hasher_->HashFromOMKey(o_rvec.first), GetInferredTime(r)));
    }
  }
  return hits;
}

template <class Response> template <class AbsDAQHitContainer>
AbsDAQHitContainer
OMKeyMap_HitFacility<Response>::GetAbsDAQHits() const {
//   if (!hitObjects_) //if the pointer is not yet pointing to a valid object 
//     hitObjects_ = boost::make_shared<ResponseHitObjectList>(HitSorting::OMKeyMap_To_HitObjects<Response, ResponseHitObjectList> (*map_));
//   
//   AbsDAQHitContainer hits;
//   BOOST_FOREACH(const ResponseHitObject &ho, *hitObjects_)
//     hits.insert(hits.end(), AbsDAQHit(hasher_->HashFromOMKey(ho.GetOMKey()), ho.GetDAQTicks()));
//   return hits;

  //do NOT go through HitObjects and do things directly
  AbsDAQHitContainer hits;
  typedef std::pair<OMKey, std::vector<Response> > OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& o_rvec, *map_) {
    BOOST_FOREACH(const Response& r, o_rvec.second) {
      hits.insert(hits.end(), AbsDAQHit(hasher_->HashFromOMKey(o_rvec.first), GetInferredDAQTicks(r)));
    }
  }
  return hits;
};


template <class Response> template<class HitContainer>
typename OMKeyMap_HitFacility<Response>::I3ResponseSeriesMap
OMKeyMap_HitFacility<Response>::MapFromHits (const HitContainer &hits) const {

  if (!hitObjects_)
    log_fatal("hits were not correctly extracted");
  
  I3ResponseSeriesMap responseMap;

  const HitSetRO ro_hits(hits.begin(), hits.end());
  for (HitSetRO::const_iterator hit_iter=ro_hits.begin(); hit_iter!=ro_hits.end(); ++hit_iter) {
    const ResponseHitObject& resp = hit_iter->GetAssociatedHitObject<Response>();
    responseMap[resp.GetOMKey()].push_back(resp.GetResponseObj());
  }
  
  return responseMap;
};

template <class Response> template<class AbsHitContainer>
typename OMKeyMap_HitFacility<Response>::I3ResponseSeriesMap
OMKeyMap_HitFacility<Response>::MapFromAbsHits (const AbsHitContainer &abshits) const {

  I3ResponseSeriesMap responseMap;

  const AbsHitSetRO ro_hits(abshits.begin(), abshits.end());
  
  AbsHitSetRO::const_iterator hit_iter=ro_hits.begin();
  while ( hit_iter!=ro_hits.end()) {
    typename I3ResponseSeriesMap::const_iterator it = map_.find(hasher_->OMKeyFromHash(hit_iter->GetDOMIndex()));
    assert(it != map_.end());
    typename ResponseSeries::const_iterator resp_iter = it->second.begin();
    while (resp_iter!=it->second.end()) {
      while (resp_iter!=it->second.end() && GetInferredTime<Response>(resp_iter)<hit_iter->GetTime()) {
        ++resp_iter;
      }
      assert(resp_iter != it->second.end());
      if (GetInferredTime<Response>(resp_iter)==hit_iter->GetTime())
        responseMap[it->first].push_back(*resp_iter);
      
      //peak on the next hit element if its on the same DOM, if so retain that iterator position
      AbsHitSetRO::const_iterator peak_iter=hit_iter;
      ++peak_iter;
      if (peak_iter==ro_hits.end())
        break;
      if (peak_iter->GetDOMIndex()==hit_iter->GetDOMIndex()) {
        hit_iter = peak_iter;
        ++resp_iter;
        assert(resp_iter!=it->second.end());
      }
    }
  }
  
  return responseMap;
};


template <class Response>
const HitObject<Response>&
OMKeyMap_HitFacility<Response>::GetHitObject(const Hit &h) const {
  return (h.GetAssociatedHitObject<Response>());
}

template <class Response> template <class HitContainer>
OMKeyMap_HitFacility<Response>::predicate::predicate
  (const OMKeyMap_HitFacility* outer,
   const HitContainer &hits) :
  outer_(outer),
  hits_(hits.begin(),hits.end()),
  it_(hits_.begin()),
  it_end_(hits_.end())
{};

template <class Response>
bool OMKeyMap_HitFacility<Response>::predicate::operator() 
  (const OMKey& o, size_t, const Response& r)
{
  //DANGER potential memory leak;
  //function is using stepping iterators it_ and it_end_ initiatized at construction time
  //however in the function call the memory range of the between iterators seem to be different
  //than what they have been initialized as: a.k.a. it==hits_.begin() & it==hits_.end() is not true
  // at first call
  if (it_==it_end_)
    return false;
  
  const CompactHash ohash = outer_->hasher_->HashFromOMKey(o);
  const double time = GetInferredTime(r);
  
  while (it_!=it_end_) {
    if (it_->GetDOMIndex() > ohash) {
      return false;
    }
    if (it_->GetDOMIndex() == ohash) {
      if (it_->GetTime() == time) {
        assert(it_->GetAssociatedHitObject<Response>().GetResponseObj()==r); //make sure these object are the same
        return true;
      }
      else if (it_->GetTime() > time) {
        return false;
      }
    }
    it_++;
  }
  return false;
};


template <class Response> template <class AbsHitContainer>
OMKeyMap_HitFacility<Response>::predicate_abs::predicate_abs
  (const OMKeyMap_HitFacility* outer,
   const AbsHitContainer &hits) :
  outer_(outer),
  hits_(hits.begin(),hits.end()),
  it_(hits_.begin()),
  it_end_(hits_.end())
{};

template <class Response>
bool OMKeyMap_HitFacility<Response>::predicate_abs::operator() 
  (const OMKey& o, size_t, const Response& r)
{
  //DANGER potential memory leak;
  //function is using stepping iterators it_ and it_end_ initiatized at construction time
  //however in the function call the memory range of the between iterators seem to be different
  //than what they have been initialized as: a.k.a. it==hits_.begin() & it==hits_.end() is not true
  // at first call
  
  if (it_==it_end_)
    return false;
  
  const CompactHash ohash = outer_->hasher_->HashFromOMKey(o);
  const double time = GetInferredTime(r);
  while (it_!=it_end_) {
    if (it_->GetDOMIndex() > ohash) {
      return false;
    }
    if (it_->GetDOMIndex() == ohash) {
      if (it_->GetTime() == time) {
        return true;}
      else if (it_->GetTime() > time) {
        return false;}
    }
    it_++;
  }
  return false;
};


template <class Response>
CompactOMKeyHashServiceConstPtr OMKeyMap_HitFacility<Response>::GetHasher() const
  {return hasher_;};


//========== CLASS I3RecoPulseSeriesMapHitFacility =============

template<class HitContainer>
I3RecoPulseSeriesMapMask
I3RecoPulseSeriesMap_HitFacility::MaskFromHits (const HitContainer &hits) const {
  return I3RecoPulseSeriesMapMask(*frame_, key_, predicate(this, hits));
};

template<class AbsHitContainer>
I3RecoPulseSeriesMapMask
I3RecoPulseSeriesMap_HitFacility::MaskFromAbsHits (const AbsHitContainer &hits) const {
  return I3RecoPulseSeriesMapMask(*frame_, key_, predicate_abs(this, hits));
};


template <class HitContainer>
HitContainer
I3RecoPulseSeriesMap_HitFacility::GetHits () const {
  return OMKeyMap_HitFacility<I3RecoPulse>::GetHits<HitContainer>(); //go though the base-class method
};

template <class HitContainer>
HitContainer
I3RecoPulseSeriesMap_HitFacility::GetHits (const std::string &key) const {
  
  //first check if the mask is really derived object from 'key_'
  if (key.empty() || key==key_)
    return OMKeyMap_HitFacility<I3RecoPulse>::GetHits<HitContainer>(); //go though the base-class method
  
  I3RecoPulseSeriesMapMaskConstPtr derived_mask = frame_->Get<I3RecoPulseSeriesMapMaskConstPtr>(key);
  if (!derived_mask) {
    log_fatal("Key is no Mask!");
  }
  while (derived_mask && (derived_mask->GetSource() != key_)) {
    std::string ancestor_name = derived_mask->GetSource();
    derived_mask = frame_->Get<I3RecoPulseSeriesMapMaskConstPtr>(ancestor_name);
  }
  if (!derived_mask)
    log_fatal("the mask is not derived from key_");
  
  //just reextract the hitobjects
  ResponseHitObjectList hitObjects_derived = HitSorting::OMKeyMap_To_HitObjects<I3RecoPulse, ResponseHitObjectList >(*derived_mask->Apply(*frame_));//as an alternative choose a priporityque

  //make the hits by finding the correct hitobject
  HitContainer hits;
  
  ResponseHitObjectList::const_iterator iter=hitObjects_->begin();
  BOOST_FOREACH(const I3RecoPulse_HitObject &ho, hitObjects_derived) {
    while (iter != hitObjects_->end()) {
      if (iter->GetOMKey() == ho.GetOMKey() && iter->GetResponseObj() == ho.GetResponseObj()) {
        hits.insert(hits.end(), Hit(hasher_->HashFromOMKey(iter->GetOMKey()), iter->GetTime(), *iter));
        break;
      }
      iter++;
    }
  }

  return hits;
};

template <class AbsHitContainer>
AbsHitContainer
I3RecoPulseSeriesMap_HitFacility::GetAbsHits () const {
  return OMKeyMap_HitFacility<I3RecoPulse>::GetAbsHits<AbsHitContainer>();
}

template <class AbsHitContainer>
AbsHitContainer
I3RecoPulseSeriesMap_HitFacility::GetAbsHits (const std::string &key) const {
  
  //first check if the mask is really derived object from 'key_'
  if (key.empty() || key==key_)
    return OMKeyMap_HitFacility<I3RecoPulse>::GetAbsHits<AbsHitContainer>(); //go though the base-class method
  
  I3RecoPulseSeriesMapMaskConstPtr derived_mask = frame_->Get<I3RecoPulseSeriesMapMaskConstPtr>(key);
  if (!derived_mask) {
    log_fatal("Key is no Mask!");
  }
  while (derived_mask && (derived_mask->GetSource() != key_)) {
    std::string ancestor_name = derived_mask->GetSource();
    derived_mask = frame_->Get<I3RecoPulseSeriesMapMaskConstPtr>(ancestor_name);
  }
  if (!derived_mask)
    log_fatal("the mask is not derived from key_");
  
  //just reextract the hitobjects
  ResponseHitObjectList hitObjects_derived = HitSorting::OMKeyMap_To_HitObjects<I3RecoPulse, ResponseHitObjectList >(*derived_mask->Apply(*frame_));//as an alternative choose a priporityque

  //make the hits by finding the correct hitobject
//   AbsHitContainer hits;
//   
//   ResponseHitObjectList::const_iterator iter=hitObjects_->begin();
//   BOOST_FOREACH(const I3RecoPulse_HitObject &ho, hitObjects_derived) {
//     while (iter != hitObjects_->end()) {
//       if (iter->GetOMKey() == ho.GetOMKey() && iter->GetResponseObj() == ho.GetResponseObj()) {
//         hits.insert(hits.end(), AbsHit(hasher_->HashFromOMKey(iter->GetOMKey()), iter->GetTime()));
//         break;
//       }
//       iter++;
//     }
//   }
  
  AbsHitContainer hits;
  typedef std::pair<OMKey, std::vector<I3RecoPulse> > OMKey_RespVec;
  BOOST_FOREACH(const OMKey_RespVec& o_rvec, *map_) {
    BOOST_FOREACH(const I3RecoPulse& r, o_rvec.second) {
      hits.insert(hits.end(), AbsHit(hasher_->HashFromOMKey(o_rvec.first), GetInferredTime(r)));
    }
  }
  return hits;
};


#endif