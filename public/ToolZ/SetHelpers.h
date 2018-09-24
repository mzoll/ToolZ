/**
 * @file HitSorting.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: Hitclasses.h 153510 2017-02-24 15:05:31Z mzoll $
 * @version $Revision: 153510 $
 * @date $Date: 2017-02-24 16:05:31 +0100 (fre, 24 feb 2017) $
 * @author mzoll <marcel.zoll@fysik.su.se>
 * 
 * Provides functionality to extract and sort Hit-objects from various kinds of representations of hits in icetray
 */

#ifndef SETHELPERS_H
#define SETHELPERS_H

static const unsigned sethelpers_version_ = 0;

#include <set>

//=================== Now some Helper functions for ordered sets ==================

/**Test whether any item in a sorted container is also found in another container
* NOTE: requirements: iterator and operator< (order!) defined
* @param lhs the one container
* @param rhs the other container having the same sorting as lhs_iter
* @return (true) if lhs and rhs intersect by at least one element
*/
template <class ordered_set>
bool SetsIntersect(const ordered_set &lhs, const ordered_set &rhs);

/** returns the number of intersecting items in two sets
* NOTE: requirements: iterator and operator< (order!) defined
* @param lhs the one container
* @param rhs the other container having the same sorting as lhs_iter
* @return return the number of intersecting items
*/
template <class ordered_set>
unsigned SetsIntersectionCount(const ordered_set &lhs, const ordered_set &rhs);

/** returns the intersecting items in two sets
* NOTE: requirements: iterator and operator< (order!) defined
* @param lhs the one container
* @param rhs the other container having the same sorting as lhs_iter
* @return return the set of insecting elements
*/
template <class ordered_set>
ordered_set SetsIntersection(const ordered_set &lhs, const ordered_set &rhs);

/** @brief a helper function that compares two sets and evaluates the identity by evaluating the identity of every element
* NOTE: requirements: iterator and operator< (order!) defined
* @param lhs the one container
* @param rhs the other container having the same sorting as lhs_iter
* @return (true) if sets are identical in every element
*/
template <class ordered_set>
bool SetsIdentical(const ordered_set &lhs, const ordered_set &rhs);

/** @brief Return the union of lhs and rhs
* @param lhs the one container
* @param rhs the other container having the same sorting as lhs_iter
* @return the union of the sets
*/
template <class ordered_set>
ordered_set UniteSets(const ordered_set &lhs, const ordered_set &rhs);

  
//==============================================================================
//========================== IMPLEMENTATIONS ===================================
//==============================================================================

template <class ordered_set>
bool 
SetsIntersect(const ordered_set &lhs,
                          const ordered_set &rhs)
{
  typename ordered_set::const_iterator iter1 = lhs.begin();
  typename ordered_set::const_iterator iter2 = rhs.begin();
  while (iter1!=lhs.end() && iter2!=rhs.end()) {
    if (*iter1<*iter2)
      ++iter1;
    else if (*iter2<*iter1)
      ++iter2;
    else
      return(true);
  }
  return(false);
};


template <class ordered_set>
ordered_set
SetsIntersection(const ordered_set &lhs,
                 const ordered_set &rhs)
{ 
  ordered_set outset;
  typename ordered_set::const_iterator iter1 = lhs.begin();
  typename ordered_set::const_iterator iter2 = rhs.begin();
  while (iter1!=lhs.end() && iter2!=rhs.end()) {
    if (*iter1<*iter2)
      ++iter1;
    else if (*iter2<*iter1)
      ++iter2;
    else {
      outset.insert(outset.end(), *iter1);
      ++iter1;
      ++iter2;
    }
  }
  return outset;
};

template <class ordered_set>
unsigned 
SetsIntersectionCount(const ordered_set &lhs,
                          const ordered_set &rhs)
{
  unsigned overlap=0;
  typename ordered_set::const_iterator iter1 = lhs.begin();
  typename ordered_set::const_iterator iter2 = rhs.begin();
  while (iter1!=lhs.end() && iter2!=rhs.end()) {
    if (*iter1<*iter2)
      ++iter1;
    else if (*iter2<*iter1)
      ++iter2;
    else {
      overlap++;
      ++iter1;
      ++iter2;
    }
  }
  return overlap;
};



template <class ordered_set>
bool 
SetsIdentical(const ordered_set &lhs,
                          const ordered_set &rhs)
{
  if (lhs.size() != rhs.size())
    return false;
  
  return (std::equal(lhs.begin(), lhs.end(), rhs.begin()) );
};

template <class ordered_set>
ordered_set 
UniteSets(const ordered_set &lhs,
                      const ordered_set &rhs)
{
  ordered_set unite;

  for (typename ordered_set::const_iterator lhs_iter = lhs.begin(); lhs_iter!=lhs.end(); ++lhs_iter){
    unite.insert(*lhs_iter);
  }
  for (typename ordered_set::const_iterator rhs_iter = rhs.begin(); rhs_iter!=rhs.end(); ++rhs_iter){
    unite.insert(*rhs_iter);
  }
  return unite;
};

#endif //SETHELPERS_H