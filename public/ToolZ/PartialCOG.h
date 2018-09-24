/**
 * \file PartialCOG.h
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: PartialCOG.h 151990 2016-12-06 13:40:27Z mzoll $
 * \version $Revision: 151990 $
 * \date $Date: 2016-12-06 14:40:27 +0100 (tis, 06 dec 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Contains functions to compute quantiles on for example a set/series of hits/pulses
 * and also compute the center of gravity (COG) of any series of hits.
 * Both function have the option to 
 */

#ifndef PARTIALCOG_H
#define PARTIALCOG_H

#include <algorithm>
#include <cassert>
#include <limits>
#include <list>
#include <map>

#include <boost/function.hpp>
#include <boost/make_shared.hpp>

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/Hitclasses.h"
#include "ToolZ/PositionService.h"
#include "dataclasses/physics/I3RecoPulse.h"

/** compute the fraction of hits using quartiles of their hit-time order
  * @param hits a set of hits
  * @param nFrac number of fractions
  * @param startFrac start counting from this fraction 
  * @param endFrac end counting after this fraction (included)
  * @param reverseOrder count in reverse, default is false
  * @param min_inc include at least that many entries from the start of counting, default is 0
  * @param max_inc include no more than that many entries from the start of counting, default is INF
  */
template<class T>
std::set<T> Fraction_count (
  const std::set<T>& tset,
  const size_t nFrac,
  const size_t startFrac, //natural counting
  const size_t endFrac, //natural counting
  const bool reverseOrder = false,
  const size_t min_inc=std::numeric_limits<size_t>::min(),
  const size_t max_inc=std::numeric_limits<size_t>::max());

/** compute the fraction of hits using quartiles of their accumulated charge
  * @param hits a set of hits
  * @param nFrac number of fractions that should be used
  * @param getweight a function to retrieve a specific weight for every entry in tset
  * @param useFrac use that fraction (always absolute counting from the front)
  * @param reverseOrder within that fraction count from back instead as from the front
  * @param min_inc include at least that many doms in the COG
  * @param max_inc include no more than that many doms in the COG
  */
template <class T>
std::set<T> Fraction_weight (
  const std::set<T> tset,
  const boost::function<double (const T&)>& getweight,
  const size_t nFrac,
  const size_t startFrac, //natural counting
  const size_t endFrac, //natural counting
  const bool reverseOrder=false,
  const size_t min_inc=std::numeric_limits<size_t>::min(),
  const size_t max_inc=std::numeric_limits<size_t>::max());


//NOTE The code below is very IceCube specific as it uses icetray containers, while the code above is of more general nature

///new type holding the equivalent of a fourvector of position (in meter) and time (in ns)
typedef std::pair<I3Position,double> I3PosTime;

/**
 * Compute the COG of this part of Hits
 * @param hits a set of hits
 * @param useCharge weight each hit by its charge
 * @return a pair of COG position and time (as average of the configured pulses)
 */
template <class Hitclass>
I3PosTime ComputeCOG_count(
  const PositionServiceConstPtr& posService,
  const std::set<Hitclass>& hits);

/**
 * Compute the COG of this part of Hits
 * @param hits a set of hits
 * @param getweight a function to retrieve a specific weight for every entry in tset
 * @return a pair of COG position and time (as average of the configured pulses)
 */
template <class Hitclass>
I3PosTime ComputeCOG_weight(
  const PositionServiceConstPtr& posService,
  const std::set<Hitclass>& hits,
  const boost::function<double (const Hitclass&)>& getweight);

//=========================================================================
//======================= IMPLEMENTATIONS =================================
//=========================================================================

//_____________________________________________________________________________
template<class T>
std::set<T> Fraction_count (
  const std::set<T>& tset,
  const size_t nFrac,
  const size_t startFrac, //natural counting
  const size_t endFrac, //natural counting
  const bool reverseOrder,
  const size_t min_inc,
  const size_t max_inc)
{
  log_debug("Entering HitFraction_countHits()");
  
  if (tset.empty())
    return tset;
  
  if (nFrac==0)
    log_fatal("'nFrac' must be greater than zero");
  
  if (startFrac>nFrac || endFrac>nFrac)
    log_fatal("'startFrac' and 'endFrac' must be smaller than the number of fractions");

  if (reverseOrder) {
    if (startFrac<endFrac)
      log_fatal("For reverse counting 'startFrac' must be greater or equal 'endFrac'");
  } else {
    if (startFrac>endFrac)
      log_fatal("For reverse counting 'startFrac' must be lesser or equal 'endFrac'");
  }
  
  const double n_per_frac = tset.size()/nFrac;

  std::set<T> outtset;
  
  if (!reverseOrder) { //counting forward
    const size_t start_index = (unsigned long)std::ceil((startFrac-1)*n_per_frac);
    const size_t end_index = std::min((unsigned long)std::floor(endFrac*n_per_frac)-1, tset.size()-1);
    const size_t n_many = end_index-start_index +1;
    
    typename std::set<T>::const_iterator tset_iter = tset.begin();
    for (size_t i=0; i<start_index; i++)
      tset_iter++;
    
    size_t n_added = 0;
    while(tset_iter!=tset.end()) {
      outtset.insert(outtset.end(),*tset_iter);
      n_added++;
      if ((n_added>=min_inc && n_added>=n_many) || n_added >=max_inc)
        break;
      tset_iter++;
    }
  }
  else { //counting backward
    const size_t start_index = std::min((unsigned long)std::floor(startFrac*n_per_frac)-1, tset.size()-1);
    const size_t end_index = (unsigned long)std::ceil((endFrac-1)*n_per_frac);
    const size_t n_many = start_index-end_index +1;

    typename std::set<T>::const_reverse_iterator tset_riter = tset.rbegin();
    for (size_t i=tset.size()-1; i>start_index; i--)
      ++tset_riter;
    
    size_t n_added = 0;
    while(tset_riter!=tset.rend()) {
      outtset.insert(outtset.begin(), *tset_riter);
      ++n_added;
      if ((n_added>=min_inc && n_added>=n_many) || n_added >=max_inc)
        break;
      ++tset_riter;
    }
  }

  log_debug("Leaving HitFraction_countHits");
  return outtset;
};


//_____________________________________________________________________________
template <class T>
std::set<T> Fraction_weight (
  const std::set<T> tset,
  const boost::function<double (const T&)>& getweight,
  const size_t nFrac,
  const size_t startFrac, //natural counting
  const size_t endFrac, //natural counting
  const bool reverseOrder,
  const size_t min_inc,
  const size_t max_inc)
{
   log_debug("Entering HitFraction_countCharge()");
  
  if (tset.empty())
    return tset;
  
  if (nFrac==0)
    log_fatal("'nFrac' must be greater than zero");
  
  if (startFrac>nFrac || endFrac>nFrac)
    log_fatal("'startFrac' and 'endFrac' must be smaller than the number of fractions");

  if (reverseOrder) {
    if (startFrac<endFrac)
      log_fatal("For reverse counting 'startFrac' must be greater or equal 'endFrac'");
  } else {
    if (startFrac>endFrac)
      log_fatal("For reverse counting 'startFrac' must be lesser or equal 'endFrac'");
  }
  
  //compute weight per fraction
  double tot_weight(0.);
  BOOST_FOREACH(const T& t, tset) 
    tot_weight+= getweight(t);
  
  const double weight_per_frac = tot_weight/nFrac;
  
  HitSet outtset;
  
  if (!reverseOrder) { //counting forward
    double start_weight = (startFrac-1)*weight_per_frac;
    double end_weight = endFrac*weight_per_frac;
    
    HitSet::const_iterator tset_iter = tset.begin();
    double c=0; //collected weight
    double c_add=0;
    while (tset_iter != tset.end()) {
      double c_add = getweight(*tset_iter);
      if (c+c_add > start_weight)
        break;
      c += c_add;
      tset_iter++;
    }
    //have found the start position
    size_t n_added=0;
    while (tset_iter != tset.end()) {
      outtset.insert(outtset.end(), *tset_iter);
      double c_add = getweight(*tset_iter);
      c += c_add;
      ++n_added;
      
      if ((n_added>=min_inc && c>=end_weight) || n_added >=max_inc)
        break;
      ++tset_iter;
    }
  }
  else { //counting backward
    double start_weight = (startFrac)*weight_per_frac;
    double end_weight = (endFrac-1)*weight_per_frac;
    
    HitSet::const_reverse_iterator tset_riter = tset.rbegin();
    double c=0; //collected weight
    double c_add=0;
    while (tset_riter != tset.rend()) {
      double c_add = getweight(*tset_riter);
      if (c+c_add > start_weight)
        break;
      c += c_add;
      tset_riter++;
    }
    //have found the start position
    size_t n_added=0;
    while (tset_riter != tset.rend()) {
      outtset.insert(outtset.begin(), *tset_riter);
      double c_add = getweight(*tset_riter);
      c += c_add;
      ++n_added;
      
      if ((n_added>=min_inc && c>=end_weight) || n_added >=max_inc)
        break;
      ++tset_riter;
    }
  }
  
  log_debug("Leaving HitFraction_countCharge");
  return outtset;
};


//_____________________________________________________________________________
template <class Hitclass>
I3PosTime ComputeCOG(
  const PositionServiceConstPtr& posService,
  const std::set<Hitclass>& hits)
{
  log_debug("Entering CompputeCOG()");
  
  double cog_time(0.);
  I3Position cog_pos(0.,0.,0.);
  
  size_t n_added(0);

  BOOST_FOREACH(const Hitclass& hit, hits) {
    n_added++;
    cog_time += hit.GetTime();
    const I3Position domPos = posService->GetPosition(hit.GetDOMIndex());
    cog_pos.SetX(cog_pos.GetX() + domPos.GetX());
    cog_pos.SetY(cog_pos.GetY() + domPos.GetY());
    cog_pos.SetZ(cog_pos.GetZ() + domPos.GetZ());
  }

  cog_time/= n_added;
  cog_pos.SetX(cog_pos.GetX() / n_added);
  cog_pos.SetY(cog_pos.GetY() / n_added);
  cog_pos.SetZ(cog_pos.GetZ() / n_added);

  log_debug("Leaving CompputeCOG");
  return std::make_pair(cog_pos, cog_time);
};


//_____________________________________________________________________________
template <class Hitclass> 
I3PosTime ComputeCOG_weight(
  const PositionServiceConstPtr& posService,
  const std::set<Hitclass>& hits,
  const boost::function<double (const Hitclass&)>& getweight)
{
  log_debug("Entering CompputeCOG_weight()");
  
  double cog_time(0.);
  I3Position cog_pos(0.,0.,0.);
  
  double coll_weight(0.); 

  BOOST_FOREACH(const Hit& hit, hits) {
    const double weight = getweight(hit);
    const I3Position domPos = posService->GetPosition(hit.GetDOMIndex());
    coll_weight+=weight;
    cog_time += hit.GetTime() * weight;
    cog_pos.SetX(cog_pos.GetX() + domPos.GetX() * weight );
    cog_pos.SetY(cog_pos.GetY() + domPos.GetY() * weight );
    cog_pos.SetZ(cog_pos.GetZ() + domPos.GetZ() * weight );
  }

  cog_time/= coll_weight;
  cog_pos.SetX(cog_pos.GetX() / coll_weight);
  cog_pos.SetY(cog_pos.GetY() / coll_weight);
  cog_pos.SetZ(cog_pos.GetZ() / coll_weight);

  log_debug("Leaving CompputeCOG");
  return std::make_pair(cog_pos, cog_time);
};




#endif
