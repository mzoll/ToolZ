/**
 * $Id: TestHelpers.cxx 124115 2014-10-03 16:42:30Z mzoll $
 * $Author: mzoll $
 * $Date: 2014-10-03 18:42:30 +0200 (fre, 03 okt 2014) $
 * $Revision: 124115 $
 *
 * A Unit test which generates some artificial test cases and let the Cleaning gnaw on them;
 */

#include "TestHelpers.h"

#include "boost/make_shared.hpp"

I3RecoPulse MakeRecoPulse (const double t, const double c, const double w, const uint8_t flags) {
  I3RecoPulse p;
  p.SetTime(t);
  p.SetCharge(c);
  p.SetWidth(w);
  p.SetFlags(flags);
  return p;
}

I3RecoPulseSeriesMap GenerateTestRecoPulses() {
  I3RecoPulseSeriesMap pulseMap;
  for (uint64_t i=1; i<=86; i++) {
    for (uint64_t j=1; j<=60; j++) {
      for (uint64_t k=0; k<20; k++) {
        pulseMap[OMKey(i,j)].push_back(MakeRecoPulse(k, k));
      }
    }
  }
  return pulseMap;
};


#include <gsl/gsl_rng.h>
#include "ToolZ/OMKeyHash.h"
#include "ToolZ/HitSorting.h"
I3RecoPulseSeriesMap GenerateDetectorNoiseRecoPulses(const double max_time_range_ns) {
  using namespace HitSorting;
  
  assert(max_time_range_ns>0.);
  
  const double DOM_NOISE_RATE_IC = 500.; //Hz
  const double DOM_NOISE_RATE_DC = 800.; //Hz
  
  const uint64_t N_HITS_EXPECTED_IC = uint64_t(max_time_range_ns*1.E-9*DOM_NOISE_RATE_IC*78*60);
  const uint64_t N_HITS_EXPECTED_DC = uint64_t(max_time_range_ns*1.E-9*DOM_NOISE_RATE_DC*8*60);
  
  //create hashable range
  std::set<OMKey> omkey_set;
  for (uint64_t i=1; i<=86; i++) {
    for (uint64_t j=1; j<=60; j++) {
      omkey_set.insert(omkey_set.end(), OMKey(i,j));
    }
  }
  const CompactOMKeyHashServiceConstPtr hasher
    = boost::make_shared<CompactOMKeyHashService>(omkey_set);
  
  //initialize the random number generator
  const gsl_rng_type * T;
  gsl_rng * r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  //typedef std::list<HitObject<I3RecoPulse> > HitObjectsList;
  typedef std::list<HitObjectOriginal<I3RecoPulse> > HitObjectsList;
  HitSet hits;
  
  //for IceCube DOMs
  HitObjectsList hitobjects_ic;
  while (hitobjects_ic.size()<N_HITS_EXPECTED_IC) {
    uint64_t string_ic = gsl_rng_uniform_int (r, 77)+1;
    uint64_t om = gsl_rng_uniform_int (r, 59)+1;
    uint64_t time = gsl_rng_uniform (r) * max_time_range_ns;
    
    hitobjects_ic.push_back(HitObjectOriginal<I3RecoPulse>(OMKey(string_ic,om),MakeRecoPulse(time, 1)));
    hits.insert(Hit(hasher->HashFromOMKey(hitobjects_ic.rbegin()->GetOMKey()), time, hitobjects_ic.back()));
  }
  
  //for DeepCore DOMs
  HitObjectsList hitobjects_dc;
  while (hitobjects_dc.size()<N_HITS_EXPECTED_DC) {
    uint64_t string_dc = gsl_rng_uniform_int (r, 8)+79;
    uint64_t om = gsl_rng_uniform_int (r, 59)+1;
    uint64_t time = gsl_rng_uniform (r) * max_time_range_ns;
    
    hitobjects_dc.push_back(HitObjectOriginal<I3RecoPulse>(OMKey(string_dc,om),MakeRecoPulse(time, 1)));    
    hits.insert(Hit(hasher->HashFromOMKey(hitobjects_dc.rbegin()->GetOMKey()), time, hitobjects_dc.back()));
  }
  
  gsl_rng_free (r);
  log_info_stream("Delivered "<<hits.size()<<" detector noise hits");
  
  I3RecoPulseSeriesMap recoMap = HitSorting::Hits_To_OMKeyMap<I3RecoPulse, HitSet>(hits, hasher);

  return recoMap;
};

I3DOMLaunch MakeDOMLaunch (const double t, const bool lcbit)
{
  I3DOMLaunch l;
  l.SetStartTime(t);
  l.SetLCBit(lcbit);
//     void SetStartTime(double starttime)
//     void SetTriggerType(TriggerType trigger)
//     void SetTriggerMode(TriggerMode situation)
//     void SetWhichATWD(ATWDselect WhichATWD)
//     void SetRawATWD( std::vector<std::vector<int> >& v )
//     void SetRawFADC( std::vector<int>& v)
//     void SetLCBit(bool LCBit)
//     void SetIsPedestalSub(bool Pedestal)
//     void SetChargeStampHighestSample(unsigned int highsample)
//     void SetRawATWDChargeStamp(unsigned int chargesum)
//     void SetWhichATWDChargeStamp(unsigned int channelid)
  return l;
}

I3DOMLaunchSeriesMap GenerateTestDOMLaunches() {
  I3DOMLaunchSeriesMap launchMap;
  for (uint64_t i=1; i<=86; i++) {
    for (uint64_t j=1; j<=60; j++) {
      for (uint64_t k=0; k<20; k++) {
        launchMap[OMKey(i,j)].push_back(MakeDOMLaunch(k));
      }
    }
  }
  return launchMap;
};


I3Geometry CreateGeometry () {
  /* Make an icecube hexagon, inter-string spacing is 1., inter-dom spacing is 1;
   * positive x-axis to the right
   * positive y-axis upwards
   * positive z-axis perpendicular into the plane
   * 
   *         2      7
   * 
   *      3     1     6
   * 
   *         4      5
   */
  
  I3OMGeo omgeo;
  omgeo.omtype = I3OMGeo::IceCube;
  omgeo.orientation = I3Orientation(I3Direction(0, 0),I3Direction(M_PI, 0)); //straight_down
  omgeo.area = 0.0443999990821; //MAGIC hardcoding number
  
  I3Geometry i3geo;
  //depth
  omgeo.position = I3Position(0.,0.,0.);
  i3geo.omgeo[OMKey(1,1)] = omgeo;
  omgeo.position = I3Position(0.,0.,1.);
  i3geo.omgeo[OMKey(1,2)] = omgeo;
  omgeo.position = I3Position(0.,0.,2.);
  i3geo.omgeo[OMKey(1,3)] = omgeo;
  omgeo.position = I3Position(0.,0.,3.);
  i3geo.omgeo[OMKey(1,4)] = omgeo;
  omgeo.position = I3Position(0.,0.,4.);
  i3geo.omgeo[OMKey(1,5)] = omgeo;
  omgeo.position = I3Position(0.,0.,5.);
  i3geo.omgeo[OMKey(1,6)] = omgeo;
  omgeo.position = I3Position(0.,0.,6.);
  i3geo.omgeo[OMKey(1,7)] = omgeo;
  omgeo.position = I3Position(0.,0.,8.);
  i3geo.omgeo[OMKey(1,8)] = omgeo;
  //surrounding
  omgeo.position = I3Position(-1./2.,  sqrt(3./4.),0.); //north west
  i3geo.omgeo[OMKey(2,1)] = omgeo;
  omgeo.position = I3Position(    -1,           0.,0.); //west
  i3geo.omgeo[OMKey(3,1)] = omgeo;
  omgeo.position = I3Position(-1./2., -sqrt(3./4.),0.); //south west
  i3geo.omgeo[OMKey(4,1)] = omgeo;
  omgeo.position = I3Position( 1./2., -sqrt(3./4.),0.); //south east
  i3geo.omgeo[OMKey(5,1)] = omgeo;
  omgeo.position = I3Position(    1.,           0.,0.); //east
  i3geo.omgeo[OMKey(6,1)] = omgeo;
  omgeo.position = I3Position( 1./2.,  sqrt(3./4.),0.); //north east
  i3geo.omgeo[OMKey(7,1)] = omgeo;
  return i3geo;
};

//create some (global) objects which are used in all the hashings
// I3GeometryConstPtr geo = boost::make_shared<const I3Geometry>(IC86Topology::Build_IC86_Geometry());
// CompactOMKeyHashServiceConstPtr hasher = boost::make_shared<CompactOMKeyHashService>(geo);
// DistanceServiceConstPtr distService = boost::make_shared<DistanceService>(geo,hasher); 

CompactOMKeyHashServiceConstPtr
DummyHashService(const size_t size) {
  static std::set<OMKey> omkeys;
  for (size_t i=0; i<size; i++)
    omkeys.insert(omkeys.end(),OMKey(i,i));
  
  return boost::make_shared<const CompactOMKeyHashService>(omkeys);
};