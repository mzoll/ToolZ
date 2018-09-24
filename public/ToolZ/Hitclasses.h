/**
 * @file HitSorting.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: Hitclasses.h 153711 2017-03-03 11:19:05Z mzoll $
 * @version $Revision: 153711 $
 * @date $Date: 2017-03-03 12:19:05 +0100 (Fri, 03 Mar 2017) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef HITCLASSES_H
#define HITCLASSES_H

static const unsigned hitclasses_version_ = 0;

#include <numeric>
#include <queue>

#include "ToolZ/SetHelpers.h"
#include "ToolZ/OMKeyHash.h"


#include "dataclasses/physics/I3RecoPulse.h"
#include "dataclasses/I3MapOMKeyMask.h" //one of the bitmasks that is explicitly supported
#include "dataclasses/physics/I3DOMLaunch.h"
#include "dataclasses/physics/I3MCHit.h"
#include "simclasses/I3MCPulse.h"

  
//============ CLASS AbsHit ===========

/**
  *  A abstract description of a hit
  * This type needs to be as small as possible so that copies are cheap.
  */
class AbsHit{
private:
  ///The index of the DOM on which this hit occurred, within the set of hit DOMs of the current event
  const CompactHash domIndex_;
  ///The actual time of the hit
  const double time_;
  
public:
  /** Constructor
    * @param domIndex The index of the DOM where the hit occurred
    * @param time The time of the hit
    */
  AbsHit(const CompactHash domIndex, const double time);

public:
  //access properties: Getters
  ///simply return the DOMindex of this hit
  const CompactHash& GetDOMIndex() const;    
  ///simply return the Time of this hit
  const double& GetTime() const;
  
  //comparisions
  /// computes the time difference of this hit to another hit; positive value if h is later
  /// \param h the other hit
  double TimeDiff(const AbsHit& h) const;
  
  //operators
  ///Comparison Operator for AbsHits;
  bool operator==(const AbsHit& rhs) const;
  ///for sorting AbsHits we're mostly interested in their time, and only use their indices as tie-breakers
  bool operator<(const AbsHit& rhs) const;
  
  ///struct that provides order principle for retrieval sorting
  struct RetrievalOrdered {
    ///internal call operator
    bool operator() (const AbsHit& lhs, const AbsHit& rhs) const;
  };
};

///dump object to (string)stream
std::ostream& operator<< (std::ostream& oss, const AbsHit& h);

//define some shorthands
typedef std::set<AbsHit> AbsHitSet;
typedef std::vector<AbsHit> AbsHitSeries;
typedef std::list<AbsHit> AbsHitList;
typedef std::queue<AbsHit> AbsHitQueue;
typedef std::deque<AbsHit> AbsHitDeque;

typedef std::set<AbsHit, AbsHit::RetrievalOrdered> AbsHitSetRO;

///wrapper around HitSets to establish time order in the set of SubEvents
struct HitSetTimeOrder {
  /// implement the order principle
  template <class Hitclass>
  bool operator()(const std::set<Hitclass> &lhs, const std::set<Hitclass> &rhs) const;
};

//a time-ordered sequence of time-ordered HitSets
typedef std::set<AbsHitSet, HitSetTimeOrder> AbsHitSetSequence;

typedef std::vector<AbsHitSeries> AbsHitSeriesSeries;
typedef std::deque<AbsHitSet> AbsHitSetDeque;
typedef std::list<AbsHitSet> AbsHitSetList;


//============ CLASS AbsDAQHit =========================

/**
  * A abstract description of a hit
  * Same as AbsHit, but more provice since it internally stores DAQticks
  */
class AbsDAQHit{
private:
  ///The index of the DOM on which this hit occurred, within the set of hit DOMs of the current event
  const CompactHash domIndex_;
  ///The actual time of the hit
  const int64_t daqTicks_;

public:
  /** Constructor
  * @param domIndex The index of the DOM where the hit occurred
  * @param time The time of the hit
  */
  AbsDAQHit(const CompactHash domIndex, const int64_t daqTicks);

public:
  //access properties: Getters
  ///simply return the DOMindex of this hit
  const CompactHash& GetDOMIndex() const;    
  ///simply return the Time of this hit in DAQticks
  const int64_t& GetDAQTicks() const;
  
  //comparisions
  ///get me the difference in time to another hit; positive value if h is later
  ///\param h the other hit
  double TimeDiff(const AbsDAQHit& h) const;
  ///Comparison Operator for AbsHits; just compare the (unique) index in the time ordered AbsHit-series
  bool operator==(const AbsDAQHit& rhs) const;
  ///for sorting AbsHits we're mostly interested in their time, and only use their indices as tie-breakers
  bool operator<(const AbsDAQHit& rhs) const;
  
  ///struct that provides order principle for retrieval sorting
  struct RetrievalOrdered {
    ///internal call operator
    bool operator() (const AbsDAQHit& lhs, const AbsDAQHit& rhs) const;
  };
};

///dump object to (string)stream
std::ostream& operator<< (std::ostream& oss, const AbsDAQHit& h);

//shorthands
typedef std::set<AbsDAQHit> AbsDAQHitSet;
typedef std::vector<AbsDAQHit> AbsDAQHitSeries;
typedef std::list<AbsDAQHit> AbsDAQHitList;
typedef std::queue<AbsDAQHit> AbsDAQHitQueue;
typedef std::deque<AbsDAQHit> AbsDAQHitDeque;

typedef std::set<AbsDAQHit, AbsDAQHit::RetrievalOrdered> AbsDAQHitSetRO;

//a time-ordered sequence of time-ordered HitSets
typedef std::set<AbsDAQHitSet, HitSetTimeOrder> AbsDAQHitSetSequence;

typedef std::vector<AbsDAQHitSeries> AbsDAQHitSeriesSeries;
typedef std::deque<AbsDAQHitSet> AbsDAQHitSetDeque;
typedef std::list<AbsDAQHitSet> AbsDAQHitSetList;


//=========== Helpers to get arbitrary time info from objects =============

///Get a sensible time-information of an object: this might be exact time, start-time,
/// or whatever makes sense to order objects in time; in nanoseconds and their ffractions
/// @param r the respones object interesed in
template <class Response>
double 
GetInferredTime(const Response &r);

//some specializations for explicitly supported types
template<> double GetInferredTime(const I3RecoPulse &r);
template<> double GetInferredTime(const I3DOMLaunch &r);
template<> double GetInferredTime(const I3MCHit &r);
template<> double GetInferredTime(const I3MCPulse &r);

///Get a sensible time-information of an object: this might be exact time, start-time,
/// or whatever makes sense to order objects in time; here DAQtick precision == 1/10ns
/// @param r the respones object interesed in
template <class Response>
uint64_t
GetInferredDAQTicks(const Response &r);

///establish time-ordering
template <class Response>
bool
time_sort (const Response &i, const Response &j);

//============ CLASS HitObject ===========
  
/** @brief the representation of a Hit as a pair of a OMKey and a (detector-)response-object like a pulse/lunch/hit/whatever
  * this class is templated and needs explicitly the specialization of the GetTime()-routine
  * NOTE this class holds references, so objects it is constructed from must not move in memory
  */
template <class Response>
class HitObject {
protected: //properties
  /// holds the OMKey
  const OMKey& omkey_;
  /// holds the pulse/lunch/hit/whatever
  const Response& response_obj_;
  
public:
  /// constructor
  /// \param omkey a OMKey this response was registered on
  /// \param response_obj the Response object
  HitObject(const OMKey& omkey,
            const Response& response_obj);
  
  //propties access:Getters
  /// get the characteristic time-stamp of this pulse/launch/hit/whatever;
  double GetTime() const;
  /// get the characteristic time-stamp of this pulse/launch/hit/whatever in terms of DAQTicks
  uint64_t GetDAQTicks() const;
  /// get the OMKey of this object
  const OMKey& GetOMKey() const;
  /// return the ResponseObject
  const Response& GetResponseObj() const;
  
  ///for sorting AbsHits we're mostly interested in their time, and only use their indices as tie-breakers
  bool operator<(const HitObject& rhs) const;
  
  ///struct that provides order principle for retrieval sorting
  struct RetrievalOrdered {
    ///internal call operator
    bool operator() (const HitObject<Response>& lhs, const HitObject<Response>& rhs) const;
  };
};

//shorthands
typedef HitObject<I3RecoPulse> I3RecoPulse_HitObject;
typedef HitObject<I3DOMLaunch> I3DOMLaunch_HitObject;
typedef HitObject<I3MCHit> I3MCHit_HitObject;
typedef HitObject<I3MCPulse> I3MCPulse_HitObject;

///dump the object to a (string)stream
template <class Response>
std::ostream& operator<<(std::ostream& oss, const HitObject<Response>& h);

/**
  * Evil expansion of the HitObject class, holding its own copies of objects.
  * most engeniously can be downcasted to HitObject, but must *NEVER* be downcasted and copied
  * from temporary objects, e.g. the case when inserting elements into std::containers,
  * as you will loose coherence between refernces held by Base-object and copies of 
  * those in the temporary and possibly destroyed object on the stack
  */
template <class Response>
class HitObjectOriginal : public HitObject<Response> {
private: //properties
  const OMKey omkey_; //NOTE shadows the variable of the baseclass
  const Response response_obj_; //NOTE shadows the variable of the baseclass
public:
  ///constructor
  HitObjectOriginal(
    const OMKey& omkey,
    const Response& response_obj) 
  : HitObject<Response>(omkey_,response_obj_),
    omkey_(omkey),
    response_obj_(response_obj)
  {};
  
  /// copy constructor (the implicit one is not enough)
  HitObjectOriginal(
    const HitObjectOriginal& hoo)
  : HitObject<Response>(omkey_,response_obj_),
    omkey_(hoo.omkey_),
    response_obj_(hoo.response_obj_)
  {};
};

//shorthands
typedef HitObjectOriginal<I3RecoPulse> I3RecoPulse_HitObjectOriginal;
typedef HitObjectOriginal<I3DOMLaunch> I3DOMLaunch_HitObjectOriginal;
typedef HitObjectOriginal<I3MCHit> I3MCHit_HitObjectOriginal;
typedef HitObjectOriginal<I3MCPulse> I3MCPulse_HitObjectOriginal;

//============ CLASS Hit ===========

/**
  * A description of a Hit, which can be traced to a HitObject of arbitray type
  */
class Hit: public AbsHit {
private:
  ///tracer back to the HitObject that is represented by this object
  const void* base_obj_ptr_;
public:
  ///get the associated HitObject to this hit 
  //DANGER user be warned; cast to the right type! 
  template<class Response>
  const HitObject<Response>&
  GetAssociatedHitObject() const;

public:
  ///constructor; hide it from the user, giving only access to it by friend-classes/functions
  /// @param domindex the hashed domindex of the base object
  /// @param time the caracteristic time of the base object
  /// @param base_obj the HitObject, this new Hit is constructed from and associated with
  template<class Response>
  Hit(const CompactHash di, const double time, const HitObject<Response> &base_obj);
  
  ///struct that provides order principle for retrieval sorting
  struct RetrievalOrdered {
    ///internal call operator
    bool operator() (const Hit& lhs, const Hit& rhs) const;
  };
};

///Dump the object into a string-stream (this is bound to python as string-cast)
std::ostream& operator<<(std::ostream& oss, const Hit& h);  

//some more definitions for frequent use, objects
typedef std::set<Hit> HitSet;
typedef std::vector<Hit> HitSeries;
typedef std::list<Hit> HitList;
typedef std::queue<Hit> HitQueue;
typedef std::deque<Hit> HitDeque;

typedef std::set<Hit, Hit::RetrievalOrdered> HitSetRO;

//a time-ordered sequence of time-ordered HitSets
typedef std::set<HitSet, HitSetTimeOrder> HitSetSequence;

typedef std::vector<HitSeries> HitSeriesSeries;
typedef std::deque<HitSet> HitSetDeque;
typedef std::list<HitSet> HitSetList;

  
//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

//================ CLASS AbsHit =======================

inline
const CompactHash& AbsHit::GetDOMIndex() const {return domIndex_;};

inline
const double& AbsHit::GetTime() const {return time_;};

inline
double AbsHit::TimeDiff(const AbsHit& h) const {return h.time_ - time_;};

inline
bool AbsHit::operator== (const AbsHit& rhs) const
  {return(domIndex_==rhs.domIndex_ && time_==rhs.time_);};

inline
bool AbsHit::operator< (const AbsHit& rhs) const {
  if (time_==rhs.time_)
    return (domIndex_<rhs.domIndex_);
  return (time_<rhs.time_);
};

inline
bool AbsHit::RetrievalOrdered::operator() (const AbsHit& lhs, const AbsHit& rhs) const {
  if (lhs.GetDOMIndex() == rhs.GetDOMIndex())
    return lhs.GetTime() < rhs.GetTime();
  return lhs.GetDOMIndex() < rhs.GetDOMIndex();
};

template <class Hitclass>
bool HitSetTimeOrder::operator() (
  const std::set<Hitclass> &lhs, 
  const std::set<Hitclass> &rhs) const 
{
  typename std::set<Hitclass>::const_iterator lhs_iter= lhs.begin();
  typename std::set<Hitclass>::const_iterator rhs_iter= rhs.begin();
  const typename std::set<Hitclass>::const_iterator lhs_end= lhs.end();
  const typename std::set<Hitclass>::const_iterator rhs_end= rhs.end();
  // until any of the hits is decisive      
  while (lhs_iter!=lhs_end && rhs_iter!=rhs_end) {
    if (*lhs_iter == *rhs_iter) {
      ++lhs_iter;
      ++rhs_iter;
      continue;
    }
    return (*lhs_iter < *rhs_iter);
  }
  //if still the same up to this point, the smaller set concludes first and is therefore considered earlier
  return (lhs_iter==lhs_end);
};


//================ CLASS AbsDAQHit =======================

inline
const CompactHash& AbsDAQHit::GetDOMIndex() const {return domIndex_;};

inline
const int64_t& AbsDAQHit::GetDAQTicks() const {return daqTicks_;};

inline
double AbsDAQHit::TimeDiff(const AbsDAQHit& h) const {return double(h.daqTicks_ - daqTicks_)/10.;};

inline
bool AbsDAQHit::operator== (const AbsDAQHit& rhs) const
  {return domIndex_==rhs.domIndex_ && daqTicks_==rhs.daqTicks_;};

inline
bool AbsDAQHit::operator< (const AbsDAQHit& rhs) const {
  if (daqTicks_==rhs.daqTicks_)
    return(domIndex_<rhs.domIndex_);
  return(daqTicks_<rhs.daqTicks_);
};

inline
bool AbsDAQHit::RetrievalOrdered::operator() (const AbsDAQHit& lhs, const AbsDAQHit& rhs) const {
  if (lhs.GetDOMIndex() == rhs.GetDOMIndex())
    return lhs.GetDAQTicks() < rhs.GetDAQTicks();
  return lhs.GetDOMIndex() < rhs.GetDOMIndex();
};


//================ CLASS HitObject ====================

template <class Response>
HitObject<Response>::HitObject (
  const OMKey& omkey,
  const Response& response_obj)
: omkey_(omkey),
  response_obj_(response_obj)
{};

template <class Response>
double HitObject<Response>::GetTime() const
  {return GetInferredTime(response_obj_);};

template <class Response>
uint64_t HitObject<Response>::GetDAQTicks() const
  {return GetInferredDAQTicks(response_obj_);}; 

template <class Response>
const OMKey& HitObject<Response>::GetOMKey() const
  {return omkey_;};

template <class Response>
const Response& HitObject<Response>::GetResponseObj() const
  {return response_obj_;};

template <class Response>
bool HitObject<Response>::operator<(const HitObject& rhs) const
  {return GetTime()<rhs.GetTime();};

template <class Response>
bool HitObject<Response>::RetrievalOrdered::operator() (
  const HitObject<Response>& lhs,
  const HitObject<Response>& rhs) const 
{
  if (lhs.GetOMKey() == rhs.GetOMKey())
    return lhs.GetTime() < rhs.GetTime();
  return lhs.GetOMKey() < rhs.GetOMKey();
};

template <class Response>
std::ostream& operator<<(std::ostream& oss, const HitObject<Response>& h) {
  oss << "HitObject(" <<
  " omkey : " << h.GetOMKey() <<
  ", response : " << h.GetResponseObj() << " )";
  return oss;
};


//============= CLASS Hit ====================

inline
bool Hit::RetrievalOrdered::operator() (const Hit& lhs, const Hit& rhs) const {
  if (lhs.GetDOMIndex() == rhs.GetDOMIndex())
    return lhs.GetTime() < rhs.GetTime();
  return lhs.GetDOMIndex() < rhs.GetDOMIndex();
};

template<class Response>
Hit::Hit(const CompactHash di, const double time, const HitObject<Response> &base_obj):
  AbsHit(di, time),
  base_obj_ptr_(&base_obj)
{};

//================== CLASS Hit =============

template<class Response>
const HitObject<Response>&
Hit::GetAssociatedHitObject () const {
  return (const HitObject<Response>&)*(HitObject<Response>*)base_obj_ptr_;
};


//====================== EXTRA ===================

template <class Response>
uint64_t
GetInferredDAQTicks(const Response &r)
  {return uint64_t(std::abs(GetInferredTime<Response>(r)*10));};

///establish time-ordering
template <class Response>
bool
time_sort (const Response &i, const Response &j)
  { return (GetInferredTime(i)<GetInferredTime(j)); };

#endif //HITCLASSES_H