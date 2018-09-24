/**
 * @file PartialCOGTest.cxx
 * 
 * $Id: PartialCOGTest.cxx 104975 2013-06-03 18:46:34Z mzoll $
 * $Author: mzoll <marcel.zoll@fysik.su.se> $
 * $Date: 2013-06-03 20:46:34 +0200 (Mon, 03 Jun 2013) $
 * $Revision: 104975 $
 *
 * A Unit test which generates some artificial test cases and let the Splitter gnaw on it;
 */

//need to define operational parameters to define the correct tests
#include <I3Test.h>

#include "dataclasses/geometry/I3Geometry.h"

#include "ToolZ/PartialCOG.h"
#include "ToolZ/HitSorting_FirstHit.h"
#include "TestHelpers.h"

#include <math.h>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

using namespace boost;
using namespace std;
using namespace HitSorting;

const I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(CreateGeometry());
const CompactOMKeyHashServiceConstPtr hashService = boost::make_shared<const CompactOMKeyHashService>(ExtractOMKeys(geo));
const PositionServiceConstPtr posService = boost::make_shared<const PositionService>(geo->omgeo, hashService);

typedef std::set<I3RecoPulse_HitObjectOriginal> I3RecoPulse_HitObjectOriginalSet;
typedef boost::shared_ptr<I3RecoPulse_HitObjectOriginalSet> I3RecoPulse_HitObjectOriginalSetPtr;
  
// eight hits same DOM
I3RecoPulse_HitObjectOriginalSet CreateHitObjectsOriginals_samedom (const OMKey &omkey= OMKey(1,1)) {
  I3RecoPulse_HitObjectOriginalSet hoo;
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(0., 1., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(1., 2., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(2., 3., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(3., 4., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(4., 5., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(5., 6., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(6., 7., 1, 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(omkey, MakeRecoPulse(7., 8., 1, 1<<0)));
  return hoo;
};

/// eight hits going downwards on the same string
I3RecoPulse_HitObjectOriginalSet CreateHitObjectsOriginals_samestring () {
  I3RecoPulse_HitObjectOriginalSet hoo;
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,1), MakeRecoPulse(0., 1., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,2), MakeRecoPulse(1., 2., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,3), MakeRecoPulse(2., 3., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,4), MakeRecoPulse(3., 4., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,5), MakeRecoPulse(4., 5., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,6), MakeRecoPulse(5., 6., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,7), MakeRecoPulse(6., 7., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,8), MakeRecoPulse(7., 8., 1., 1<<0)));
  return hoo;
};

// eight hits on a hexagon, centerstring is hit twice
I3RecoPulse_HitObjectOriginalSet CreateHitObjectsOriginals_hexagon () {
  I3RecoPulse_HitObjectOriginalSet hoo;
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,1), MakeRecoPulse(0., 0., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(2,1), MakeRecoPulse(0., 1., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(3,1), MakeRecoPulse(0., 2., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(4,1), MakeRecoPulse(0., 3., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(5,1), MakeRecoPulse(0., 4., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(6,1), MakeRecoPulse(0., 5., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(7,1), MakeRecoPulse(0., 6., 1., 1<<0)));
  hoo.insert(I3RecoPulse_HitObjectOriginal(OMKey(1,1), MakeRecoPulse(1., 7., 1., 1<<0)));
  return hoo;
};

//helper to facilitate charge extractation from I3RecoPulse-Hits for weighting
struct Getweight_I3RecoPulseHit_Charge {
  double operator() (const Hit& h) {
    return h.GetAssociatedHitObject<I3RecoPulse>().GetResponseObj().GetCharge();
  };
};


  //need some template class function for some basic classes with a bit of overwrite
template <class X>
bool IsEqual(const X& lhs, const X& rhs)
  {return lhs==rhs;};

template <>
bool IsEqual(const AbsHit& lhs, const AbsHit& rhs)
  {return lhs.GetDOMIndex()==rhs.GetDOMIndex() && lhs.GetTime()==rhs.GetTime() ;};  


//a diagniosis tool
template<class T>
std::string stringElements(const std::set<T>& s) {
  ostringstream oss;
  BOOST_FOREACH(const T& t, s)
    { oss << t;}
  return oss.str();
};


///does a superset contain a subset of elements at certain index PositionService
/// \param superset teh superset to search elemtens in
/// \param subset the subset of elements to be found in sequence
/// \param indexes find the elements at these indexes
/// the search should return true for this case: superset [0,2,4,6] subset [2,4] indexes [1,2]
template <class T>
bool contains_elements(
  const std::set<T> superset,
  const std::set<T> subset,
  const std::set<size_t> indexes) //computer counter [0...(size-1)]
{
  assert(subset.size()==indexes.size());
  assert(subset.size()<=superset.size());
  
  if (subset.empty())
    return indexes.empty();
  
  assert(!indexes.empty() && !subset.empty());
  typename std::set<T>::const_iterator superset_iter = superset.begin();
  typename std::set<T>::const_iterator subset_iter = subset.begin();
  std::set<size_t>::const_iterator index_iter = indexes.begin();
  
  size_t current_pos = 0;
  while (subset_iter!=subset.end()) {
    const size_t next_pos = *index_iter;
    assert(next_pos<superset.size());
    
    size_t next_offset = next_pos - current_pos;
    while (next_offset) {
      ++superset_iter;
      next_offset--;
    }
    current_pos=next_pos;
    
    if (! IsEqual<T>(*subset_iter, *superset_iter))
      return false;

    ++subset_iter;
    ++index_iter;
  }
  return true;
};


//==========================================
TEST_GROUP(PartialCOG);

TEST(Fraction_count_simpleNumbers) {
  //we execute the test on a better controlled set of ints, where elementwise uniqueness and ==-comparision is easy evaluated
  std::set<size_t> superset = {0,1,2,3,4,5,6,7};
  std::set<size_t> subset;
  std::set<size_t> indexes_test;

  //take all, by selecting nfrac==1
  subset = Fraction_count(superset, 1, 1, 1, false, 1, 8);
  indexes_test = {0,1,2,3,4,5,6,7};
  ENSURE(contains_elements(superset, subset, indexes_test));

  //take first six by max_inc==6
  subset = Fraction_count(superset, 1, 1, 1, false, 1, 6);
  indexes_test = {0,1,2,3,4,5};
  ENSURE(contains_elements(superset, subset, indexes_test));

  //take last six by min included max_inc==6 and heads_tails==false
  subset = Fraction_count(superset, 1, 1, 1, true, 1, 6);
  indexes_test = {2,3,4,5,6,7};
  ENSURE(contains_elements(superset, subset, indexes_test));
 
  //take the first half by nFrac==2 and useFrac==1
  subset = Fraction_count(superset, 2, 1, 1, false, 1, 8);
  indexes_test = {0,1,2,3};
  ENSURE(contains_elements(superset, subset, indexes_test));
 
  //take the last half  nFrac==2 and useFrac==2
  subset = Fraction_count(superset, 2, 2, 2, false, 1, 8);
  indexes_test = {4,5,6,7};
  ENSURE(contains_elements(superset, subset, indexes_test));

  //take first six by min_inc==6
  subset = Fraction_count(superset, 2, 1, 1, false, 6, 6);
  indexes_test = {0,1,2,3,4,5};
  ENSURE(contains_elements(superset, subset, indexes_test));

  ///take middle 4 by including 4 and excluding 2 at front
  subset = Fraction_count(superset, 4, 2, 2, false, 4, 4);
  indexes_test = {2,3,4,5};
  ENSURE(contains_elements(superset, subset, indexes_test));
  
  //same but do it reverse
  subset = Fraction_count(superset, 4, 3, 3, true, 4, 4);
  indexes_test = {2,3,4,5};
  ENSURE(contains_elements(superset, subset, indexes_test));
};

TEST(Fraction_count) {
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const AbsHitSet hits = HitObjects_To_AbsHits<I3RecoPulse_HitObjectOriginalSet, AbsHitSet>(hoo, hashService);
  
  AbsHitSet hits_test;
  std::set<size_t> indexes_test;
 
  //take all, by selecting nfrac==1
  hits_test = Fraction_count(hits, 1, 1, 1, false, 1, 8);
  indexes_test = {0,1,2,3,4,5,6,7};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //take first six by max_inc==6
  hits_test = Fraction_count(hits, 1, 1, 1, false, 1, 6);
  indexes_test = {0,1,2,3,4,5};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //take last six by min included max_inc==6 and heads_tails==false
  hits_test = Fraction_count(hits, 1, 1, 1, true, 1, 6);
  indexes_test = {2,3,4,5,6,7};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //take the first half by nFrac==2 and useFrac==1
  hits_test = Fraction_count(hits, 2, 1, 1, false, 1, 8);
  indexes_test = {0,1,2,3};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //take the last half  nFrac==2 and useFrac==2
  hits_test = Fraction_count(hits, 2, 2, 2, false, 1, 8);
  indexes_test = {4,5,6,7};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //take first six by min_inc==6
  hits_test = Fraction_count(hits, 2, 1, 1, false, 6, 6);
  indexes_test = {0,1,2,3,4,5};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  ///take middle 4 by including 4 and excluding 2 at front
  hits_test = Fraction_count(hits, 4, 2, 2, false, 4, 4);
  indexes_test = {2,3,4,5};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
  
  //same but do it reverse
  hits_test = Fraction_count(hits, 4, 3, 3, true, 4, 4);
  indexes_test = {2,3,4,5};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
};

TEST(Fraction_chargeweighting) { //change the last argument option
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const HitSet hits = HitObjects_To_Hits<I3RecoPulse_HitObjectOriginalSet, HitSet>(hoo, hashService);
  
  //the weighting function
  Getweight_I3RecoPulseHit_Charge getweight;

  HitSet hits_test;
  std::set<size_t> indexes_test;
  
  //take all
  hits_test = Fraction_weight<Hit>(hits, getweight, 1, 1, 1, false, 1, 8);
  indexes_test = {0,1,2,3,4,5,6,7};
  ENSURE(contains_elements(hits, hits_test, indexes_test));
};
  

TEST(PartialCOG) {
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const AbsHitSet hits = HitObjects_To_AbsHits<I3RecoPulse_HitObjectOriginalSet, AbsHitSet>(hoo, hashService);
  I3PosTime pos_time;
  
  //take all, by selecting nfrac==1
  pos_time = ComputeCOG (posService, hits);
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0+1+2+3+4+5+6+7)/8.);
};

TEST(PartialCOG_weighted) {
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const HitSet hits = HitObjects_To_Hits<I3RecoPulse_HitObjectOriginalSet, HitSet>(hoo, hashService);
  I3PosTime pos_time;
  
  Getweight_I3RecoPulseHit_Charge getweight;
  
  pos_time = ComputeCOG_weight<Hit>(posService, hits, getweight);
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6+6*7+7*8)/double(1+2+3+4+5+6+7+8));
};


/*

//===============================================================
TEST (PartialCOGhit) {
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const AbsHitSet hits = HitObjects_To_AbsHits<I3RecoPulse_HitObjectOriginalSet, AbsHitSet>(hoo, hashService);
  I3PosTime pos_time;
  
  //take all, by selecting nfrac==1
  pos_time = ComputeCOG (posService, Fraction_count(hits, 1, 1, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0+1+2+3+4+5+6+7)/8.);
  
  //take first six by max_inc==6
  pos_time = ComputeCOG (posService, HitFraction_countHits(hits, 1, 1, 1, false, 1, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0+1+2+3+4+5)/6.);
  
  //take last six by min included max_inc==6 and heads_tails==false
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 1, 1, 1, true, 1, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2+3+4+5+6+7)/6.);
  
  //take the first half by nFrac==2 and useFrac==1
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 2, 1, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0+1+2+3)/4.);
  
  //take the last half  nFrac==2 and useFrac==2
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 2, 2, 2, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(4+5+6+7)/4.);
  
  //take first six by min_inc==6
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 2, 1, 1, false, 6, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0+1+2+3+4+5)/6.);
  
  ///take middle 4 by including 4 and excluding 2 at front
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 4, 2, 2, false, 4, 4));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2+3+4+5)/4.);
  
  //same but do it reverse
  pos_time = ComputeCOG (posService, Fraction_countHits(hits, 4, 3, 3, true, 4, 4));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2+3+4+5)/4.);
};  


TEST(PartialCOGhit_chargeweighting) { //change the last argument option
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const HitSet hits = HitObjects_To_Hits<I3RecoPulse_HitObjectOriginalSet, HitSet>(hoo, hashService);
  std::pair<I3Position, double> pos_time;

  //take all
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 1, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6+6*7+7*8)/double(1+2+3+4+5+6+7+8));
  
  //take first six by min_included
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 1, 1, false, 6, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6)/double(1+2+3+4+5+6));
  
  //take last six by min included
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 1, 1, true, 6, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2*3+3*4+4*5+5*6+6*7+7*8)/double(3+4+5+6+7+8));
  
  //take the first half
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 2, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4)/double(1+2+3+4));
  
  //take the last half
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 2, 2, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(4*5+5*6+6*7+7*8)/double(5+6+7+8));
  
  //take first half but require 6 included
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 2, 1, false, 6, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6)/double(1+2+3+4+5+6));
  
  ///take middle 4 by including 4 and excluding 2 at front
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 4, 2, false, 4, 4));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2*3+3*4+4*5+5*6)/double(3+4+5+6));
  
  //same but do it reverse
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countHits(hits, 4, 3, true, 4, 4));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2*3+3*4+4*5+5*6)/double(3+4+5+6));
};

//==========================================================
TEST (PartialCOGcharge) {  
  const I3RecoPulse_HitObjectOriginalSet hoo = CreateHitObjectsOriginals_samedom();
  const HitSet hits = HitObjects_To_Hits<I3RecoPulse_HitObjectOriginalSet, HitSet>(hoo, hashService);
  std::pair<I3Position, double> pos_time;
  
  //===Enable Charge Counting===
  //take all, by selecting nfrac==1
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 1, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6+6*7+7*8)/double(1+2+3+4+5+6+7+8));
  
  //take first six by max_inc==6
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 1, 1, false, 6, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6)/double(1+2+3+4+5+6));
  
  //take last six by min included max_inc==6 and heads_tails==false
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 1, 1, true, 6, 6));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(2*3+3*4+4*5+5*6+6*7+7*8)/double(3+4+5+6+7+8));
  
  //take the first (charge) half by nFrac==2 and useFrac==1; first fraction is (h1,2,3,4,5,6; c=15) (h7,h8; c=21); hit6 is associated to first fractions
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 2, 1, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6)/double(1+2+3+4+5+6));
  
  //take the last half nFrac==2 and useFrac==2
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 2, 2, false, 1, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(6*7+7*8)/double(7+8));
  
  //take first six by min_inc==6
  pos_time = ComputeCOG_chargeWeighted (posService, HitFraction_countCharge(hits, 2, 1, false, 6, 8));
  ENSURE_EQUAL(pos_time.first, I3Position(0,0,0));
  ENSURE_EQUAL(pos_time.second, double(0*1+1*2+2*3+3*4+4*5+5*6)/double(1+2+3+4+5+6));
};

*/
