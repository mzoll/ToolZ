/**
 * @file HitSorting.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: HitFacility_FirstHit.h 151868 2016-11-30 22:05:56Z mzoll $
 * @version $Revision: 151868 $
 * @date $Date: 2016-11-30 23:05:56 +0100 (ons, 30 nov 2016) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef HITFACILITY_FIRSTHIT_H
#define HITFACILITY_FIRSTHIT_H

#include <numeric>

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/HitSorting.h"
#include "dataclasses/physics/I3RecoPulse.h" //one of templates that are explicitly supported

#include "ToolZ/HitSorting_FirstHit.h"

//===================== CLASS OMKeyMap_HitFacility_FirstHit =============================

/**
  * A class which helps to extract hits from an OMKeyMap Object in the frame;
  * which also holds all the functionality so you do not do anything stupid
  * For each DOM only the first hit time will be extracted and acuumulate properties will be assigned
  * \template Response: an I3object of DOM-response type like I3DOMLaunch, I3MCHit, I3RecoPulse etc
  * \template Accumulator a struct providing a call operator how a vector of Responses are accumulated into one single Response object (signature function<Response (const std::vector<Response>&)>), e.g. accumulating the charge in I3RecoPulses
  */
template <class Response>
class OMKeyMap_HitFacility_FirstHit {
  SET_LOGGER("OMKeyMap_HitFacility_FirstHit");
protected:
  typedef std::vector<Response> ResponseSeries;
  typedef I3Map<OMKey, ResponseSeries > I3ResponseSeriesMap;
  typedef boost::shared_ptr<const I3ResponseSeriesMap> I3ResponseSeriesMapConstPtr;
  typedef HitObject<Response> ResponseHitObject;
  typedef HitObjectOriginal<Response> ResponseHitObjectOriginal;
  typedef std::set<ResponseHitObjectOriginal, typename ResponseHitObject::RetrievalOrdered> ResponseHitObjectOriginalSetRO;
  typedef std::list<ResponseHitObjectOriginal> ResponseHitObjectOriginalList; 
  
protected://properties/parameters
  ///PARAM: the frame this might have been extracted from
  const I3FrameConstPtr frame_;
  ///PARAM: name of this map in the frame
  const std::string key_;
  
protected://properties/internal
  ///hold a pointer to the OMKeyMap the Hits have been extracted from
  I3ResponseSeriesMapConstPtr map_;
  /// the hasher which resolves OMKeys and Hashes 
  const CompactOMKeyHashServiceConstPtr hasher_;
  /// here are the extracted objects NOTE losing or modifying this object means loosing coherence
  mutable boost::shared_ptr<ResponseHitObjectOriginalList> hitObjects_;
  
public://methods
  /** constructor
    * @param frame frame to extract a map from 
    * @param key key in the frame at which we going to find a I3RecoPulseMap/Mask
    * @param hasher a hasher which can hash the omkeys, will be created on the spot if not existent  
    */
  OMKeyMap_HitFacility_FirstHit(
    const I3FrameConstPtr& frame,
    const std::string& key,
    CompactOMKeyHashServiceConstPtr& hasher);
  OMKeyMap_HitFacility_FirstHit(
    const I3FrameConstPtr& frame,
    const std::string& key,
    const CompactOMKeyHashServiceConstPtr& hasher);
  /// Extract Hit-objects from the I3RecoPulseMap and put/insert them into the container 
  template <class HitContainer>
  HitContainer GetHits() const;
  /// Extract AbsHit-objects from the I3RecoPulseMap and put/insert them into the container 
  template <class AbsHitContainer>
  AbsHitContainer GetAbsHits() const;
  /// Extract AbsDAQHit-objects from the I3RecoPulseMap and put/insert them into the container 
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
  /// \param h the hit
  const ResponseHitObject& GetHitObject(const Hit &h) const;
  
  //NOTE FUTURE here could be a general implementation of "MaskFromHits()", however the OMKeyMapMasks are not templated
  
protected:
  /// a callable stuct which can internally used for example with the Masking of I3ResponseSeriesMapMask
  struct predicate {
    ///keep a pointer to the outer facility
    const OMKeyMap_HitFacility_FirstHit* outer_;
    ///hold the hits which need to be backconverted
    const HitSetRO hits_;
    ///stepping iterator
    HitSetRO::const_iterator it_;
    ///marking the static end for the stepping operator
    const HitSetRO::const_iterator it_end_;
    
    ///constructor
    template <class HitContainer>
    predicate(const OMKeyMap_HitFacility_FirstHit* outer, const HitContainer &hits);
    
    ///call operator to this object
    bool operator() (const OMKey& o, size_t, const Response& r);
  };
  
  /// a callable stuct which can internally used for example with the Masking of I3ResponseSeriesMapMask
  struct predicate_abs {
    ///keep a pointer to the outer facility
    const OMKeyMap_HitFacility_FirstHit* outer_;
    ///hold the hits which need to be backconverted
    const AbsHitSetRO hits_;
    ///stepping iterator
    AbsHitSetRO::const_iterator it_;
    ///marking the static end for the stepping operator
    const AbsHitSetRO::const_iterator it_end_;
    
    ///constructor
    template <class HitContainer>
    predicate_abs(const OMKeyMap_HitFacility_FirstHit* outer, const HitContainer &hits);
    
    ///call operator to this object
    bool operator() (const OMKey& o, size_t, const Response& r);
  };
}; //class OMKeyMap_HitFacility_FirstHit



//============ CLASS I3RecoPulseSeriesMap_HitFacility ===========

/**
  * specialize OMKeyMap_HitFacility to I3RecoPulse, and add the functionality to produce Masks
  */
class I3RecoPulseSeriesMap_HitFacility_FirstHit : public OMKeyMap_HitFacility_FirstHit<I3RecoPulse> {
  SET_LOGGER("I3RecoPulseSeriesMap_HitFacility");
public://methods
  /** constructor
    * @param frame frame to extract a map from 
    * @param key key in the frame at which we going to find a ResponseMap/Mask
    * @param hasher a hasher which can hash the omkeys, will be created on spot if not existent
    */
  I3RecoPulseSeriesMap_HitFacility_FirstHit(
    const I3FrameConstPtr& frame,
    const std::string &key,
    CompactOMKeyHashServiceConstPtr& hasher);
  
  I3RecoPulseSeriesMap_HitFacility_FirstHit(
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

typedef boost::shared_ptr<I3RecoPulseSeriesMap_HitFacility_FirstHit> I3RecoPulseSeriesMap_HitFacility_FirstHitPtr;
typedef boost::shared_ptr<const I3RecoPulseSeriesMap_HitFacility_FirstHit> I3RecoPulseSeriesMap_HitFacility_FirstHitConstPtr;



//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

//========== CLASS OMKeyMap_HitFacility_FirstHit =============

#include <boost/make_shared.hpp>

template <class Response>
OMKeyMap_HitFacility_FirstHit<Response>::OMKeyMap_HitFacility_FirstHit(
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
OMKeyMap_HitFacility_FirstHit<Response>::OMKeyMap_HitFacility_FirstHit(
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
OMKeyMap_HitFacility_FirstHit<Response>::GetHits() const {
  if (!hitObjects_) //if not yet pointing to a valid object 
    hitObjects_ = boost::make_shared<ResponseHitObjectOriginalList>(HitSorting::OMKeyMap_FirstHits_To_HitObjectOriginals<Response, ResponseHitObjectOriginalList> (*map_));
  
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
OMKeyMap_HitFacility_FirstHit<Response>::GetAbsHits() const {
  return OMKeyMap_FirstHits_To_AbsHits(hasher_, *map_);
}

template <class Response> template <class AbsDAQHitContainer>
AbsDAQHitContainer
OMKeyMap_HitFacility_FirstHit<Response>::GetAbsDAQHits() const {
  return OMKeyMap_FirstHits_To_AbsDAQHits(hasher_, *map_);
};


template <class Response> template<class HitContainer>
typename OMKeyMap_HitFacility_FirstHit<Response>::I3ResponseSeriesMap
OMKeyMap_HitFacility_FirstHit<Response>::MapFromHits (const HitContainer &hits) const {
  if (!hitObjects_)
    log_fatal("hits were not correctly extracted");
  
  I3ResponseSeriesMap responseMap;

  std::set<OMKey> omkeys;
  BOOST_FOREACH(const Hit& hit, hits)
    omkeys.insert(hit.GetAssociatedHitObject<I3RecoPulse>().GetOMKey());
  
  BOOST_FOREACH(const OMKey& omkey, omkeys)
    responseMap.insert(*(map_->find(omkey)));
  
  return responseMap;
};

template <class Response> template<class AbsHitContainer>
typename OMKeyMap_HitFacility_FirstHit<Response>::I3ResponseSeriesMap
OMKeyMap_HitFacility_FirstHit<Response>::MapFromAbsHits (const AbsHitContainer &abshits) const {
  I3ResponseSeriesMap responseMap;

  std::set<OMKey> omkeys;
  BOOST_FOREACH(const AbsHit& hit, abshits)
    omkeys.insert(hasher_->OMKeyFromHash(hit.GetDOMIndex()));
  
  BOOST_FOREACH(const OMKey& omkey, omkeys)
    responseMap.insert(*(map_->find(omkey)));
  
  return responseMap;
};


template <class Response>
const HitObject<Response>&
OMKeyMap_HitFacility_FirstHit<Response>::GetHitObject(const Hit &h) const {
  return (h.GetAssociatedHitObject<Response>());
}

template <class Response> template <class HitContainer>
OMKeyMap_HitFacility_FirstHit<Response>::predicate::predicate
  (const OMKeyMap_HitFacility_FirstHit* outer,
   const HitContainer &hits) :
  outer_(outer),
  hits_(hits.begin(),hits.end()),
  it_(hits_.begin()),
  it_end_(hits_.end())
{};

template <class Response>
bool OMKeyMap_HitFacility_FirstHit<Response>::predicate::operator() 
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
OMKeyMap_HitFacility_FirstHit<Response>::predicate_abs::predicate_abs
  (const OMKeyMap_HitFacility_FirstHit* outer,
   const AbsHitContainer &hits) :
  outer_(outer),
  hits_(hits.begin(),hits.end()),
  it_(hits_.begin()),
  it_end_(hits_.end())
{};

template <class Response>
bool OMKeyMap_HitFacility_FirstHit<Response>::predicate_abs::operator() 
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
CompactOMKeyHashServiceConstPtr OMKeyMap_HitFacility_FirstHit<Response>::GetHasher() const
  {return hasher_;};

  
//========== CLASS I3RecoPulseSeriesMap_HitFacility_FirstHit =============
  
template <class HitContainer>
HitContainer
I3RecoPulseSeriesMap_HitFacility_FirstHit::GetHits () const {
  return OMKeyMap_HitFacility_FirstHit<I3RecoPulse>::GetHits<HitContainer>(); //go though the base-class method
};

template <class HitContainer>
HitContainer
I3RecoPulseSeriesMap_HitFacility_FirstHit::GetHits (const std::string &key) const {
  
  //first check if the mask is really derived object from 'key_'
  if (key.empty() || key==key_)
    return OMKeyMap_HitFacility_FirstHit<I3RecoPulse>::GetHits<HitContainer>(); //go though the base-class method
  
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
  
  //create HitObjectOriginal-list and take only entries of OMKeys which are in this series
  if (!hitObjects_) //if not yet pointing to a valid object 
    hitObjects_ = boost::make_shared<ResponseHitObjectOriginalList>(HitSorting::OMKeyMap_FirstHits_To_HitObjectOriginals<I3RecoPulse, ResponseHitObjectOriginalList>(*map_));
  
  //just reextract the hitobjects
  const I3RecoPulseSeriesMapConstPtr derived_map= derived_mask->Apply(*frame_);
  
  const std::set<OMKey> derived_omkeys = ExtractOMKeys(*derived_map);

  //make the hits by finding the correct hitobject
  HitContainer hits;
  
  //step through
  ResponseHitObjectOriginalList::const_iterator iter=hitObjects_->begin();
  BOOST_FOREACH(const OMKey &omkey, derived_omkeys) {
    while (iter != hitObjects_->end()) {
      if (iter->GetOMKey() == omkey) {
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
I3RecoPulseSeriesMap_HitFacility_FirstHit::GetAbsHits () const {
  return OMKeyMap_HitFacility_FirstHit<I3RecoPulse>::GetAbsHits<AbsHitContainer>();
}

template <class AbsHitContainer>
AbsHitContainer
I3RecoPulseSeriesMap_HitFacility_FirstHit::GetAbsHits (const std::string &key) const {
  
  //first check if the mask is really derived object from 'key_'
  if (key.empty() || key==key_)
    return OMKeyMap_HitFacility_FirstHit<I3RecoPulse>::GetAbsHits<AbsHitContainer>(); //go though the base-class method
  
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
  
  return OMKeyMap_HitFacility_FirstHit<I3RecoPulse>::GetAbsHits<AbsHitContainer>();
};

  
template<class HitContainer>
I3RecoPulseSeriesMapMask
I3RecoPulseSeriesMap_HitFacility_FirstHit::MaskFromHits (const HitContainer &hits) const {
  return I3RecoPulseSeriesMapMask(*frame_, key_, predicate(this, hits));
};

template<class AbsHitContainer>
I3RecoPulseSeriesMapMask
I3RecoPulseSeriesMap_HitFacility_FirstHit::MaskFromAbsHits (const AbsHitContainer &hits) const {
  return I3RecoPulseSeriesMapMask(*frame_, key_, predicate_abs(this, hits));
};
  
  
#endif
