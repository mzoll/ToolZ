/**
 * copyright  (C) 2012
 * the IceCube Collaboration
 * $Id: $
 *
 * @file HitSortingPybindings.h
 * @date $Date: 2012-12-20$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include <string>

#include "icetray/python/stream_to_string.hpp"
#include "dataclasses/ostream_overloads.hpp"
#include "ToolZ/HitSorting.h"

using namespace HitSorting;
namespace bp = boost::python;

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include "icetray/python/dataclass_suite.hpp"

///namespace containing specializations and casting-functions for pybindings
namespace pyHitSorting {
  
  //=== I3RecoPulse ===
  bp::list I3RecoPulse_GetHitObjects_pyList(const I3RecoPulseSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3RecoPulse> > hitObjs 
      = OMKeyMap_To_HitObjects<I3RecoPulse, std::list<HitObject<I3RecoPulse> > >(m);
    BOOST_FOREACH (const HitObject<I3RecoPulse> &ho, hitObjs)
      l.append(ho);
    return l;
  }
  
  I3RecoPulseSeriesMap I3RecoPulseSeriesMapFromHitObjects_pyList(const bp::list& l) {
    std::list<HitObject<I3RecoPulse> > hitObjs_vec;
    for (int i=0; i<bp::len(l); i++)
      hitObjs_vec.push_back(bp::extract<HitObject<I3RecoPulse> >(l[i]));
    return HitObjects_To_OMKeyMap<I3RecoPulse>(hitObjs_vec);
  }

  I3RecoPulse_HitObject I3RecoPulse_HitObjectFromHit(const Hit &h) {
    return I3RecoPulse_HitObject(h.GetAssociatedHitObject<I3RecoPulse>());
  }
  
  
  //=== I3DOMLaunch ===
  bp::list I3DOMLaunch_GetHitObjects_pyList(const I3DOMLaunchSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3DOMLaunch> > hitObjs 
      = OMKeyMap_To_HitObjects<I3DOMLaunch,std::list<HitObject<I3DOMLaunch> > >(m);
    BOOST_FOREACH (const HitObject<I3DOMLaunch> &ho, hitObjs)
      l.append(ho);
    return l;
  }
  
  I3DOMLaunchSeriesMap I3DOMLaunchSeriesMapFromHitObjects_pyList(const bp::list& l) {
    std::list<HitObject<I3DOMLaunch> > hitObjs_vec;
    for (int i=0; i<bp::len(l); i++)
      hitObjs_vec.push_back(bp::extract<HitObject<I3DOMLaunch> >(l[i]));
    return HitObjects_To_OMKeyMap<I3DOMLaunch>(hitObjs_vec);
  }
  
  I3DOMLaunch_HitObject I3DOMLaunch_HitObjectFromHit(const Hit &h) {
    return I3DOMLaunch_HitObject(h.GetAssociatedHitObject<I3DOMLaunch>());
  }
  
  
  //=== I3MCHit ===
  bp::list I3MCHit_GetHitObjects_pyList(const I3MCHitSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3MCHit> > hitObjs 
      = OMKeyMap_To_HitObjects<I3MCHit, std::list<HitObject<I3MCHit> > >(m);
    BOOST_FOREACH (const HitObject<I3MCHit> &ho, hitObjs)
      l.append(ho);
    return l;
  }
  
  I3MCHitSeriesMap I3MCHitSeriesMapFromHitObjects_pyList(const bp::list& l) {
    std::list<HitObject<I3MCHit> > hitObjs_vec;
    for (int i=0; i<bp::len(l); i++)
      hitObjs_vec.push_back(bp::extract<HitObject<I3MCHit> >(l[i]));
    return HitObjects_To_OMKeyMap<I3MCHit>(hitObjs_vec);
  }
  
  I3MCHit_HitObject I3MCHit_HitObjectFromHit(const Hit &h) {
    return I3MCHit_HitObject(h.GetAssociatedHitObject<I3MCHit>());
  }
  
  
  //=== I3MCHit ===
  bp::list I3MCPulse_GetHitObjects_pyList(const I3MCPulseSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3MCPulse> > hitObjs
      = OMKeyMap_To_HitObjects<I3MCPulse,std::list<HitObject<I3MCPulse> > >(m);
    BOOST_FOREACH (const HitObject<I3MCPulse> &ho, hitObjs)
      l.append(ho);
    return l;
  }
  
  I3MCPulseSeriesMap I3MCPulseSeriesMapFromHitObjects_pyList(const bp::list& l) {
    std::list<HitObject<I3MCPulse> > hitObjs_vec;
    for (int i=0; i<bp::len(l); i++)
      hitObjs_vec.push_back(bp::extract<HitObject<I3MCPulse> >(l[i]));
    return HitObjects_To_OMKeyMap<I3MCPulse>(hitObjs_vec);
  }
  
  I3MCPulse_HitObject I3MCPulse_HitObjectFromHit(const Hit &h) {
    return I3MCPulse_HitObject(h.GetAssociatedHitObject<I3MCPulse>());
  };
};


///namespace containing pybinings for HitSorting library
void register_HitSorting()
{
  //Extracting / Converting HitObjects
  bp::def("I3RecoPulseSeriesMap_To_HitObjects", &pyHitSorting::I3RecoPulse_GetHitObjects_pyList,
          bp::args("map"),
          "Convert OMKeyMap to Series of HitObjects");
  bp::def("I3RecoPulseHitObjects_To_OMKeyMap", &pyHitSorting::I3RecoPulseSeriesMapFromHitObjects_pyList,
        bp::args("list"),
        "Convert HitObjects back to a OMKeyMap");
  bp::def("I3DOMLaunchSeriesMap_To_HitObjects", &pyHitSorting::I3DOMLaunch_GetHitObjects_pyList,
          bp::args("map"),
          "Convert OMKeyMap to Series of HitObjects");
  bp::def("I3DOMLaunchHitObjects_To_OMKeyMap", &pyHitSorting::I3DOMLaunchSeriesMapFromHitObjects_pyList,
          bp::args("list"),
          "Convert HitObjects back to a OMKeyMap");
  bp::def("I3MCHitSeriesMap_To_HitObjects", &pyHitSorting::I3MCHit_GetHitObjects_pyList,
          bp::args("map"),
          "Convert OMKeyMap to Series of HitObjects");
  bp::def("I3MCHitHitObjects_To_OMKeyMap", &pyHitSorting::I3MCHitSeriesMapFromHitObjects_pyList,
          bp::args("list"),
          "Convert HitObjects back to a OMKeyMap");  
  bp::def("I3MCPulseSeriesMap_To_HitObjects", &pyHitSorting::I3MCPulse_GetHitObjects_pyList,
          bp::args("map"),
          "Convert OMKeyMap to Series of HitObjects");
  bp::def("I3MCPulseHitObjects_To_OMKeyMap", &pyHitSorting::I3MCPulseSeriesMapFromHitObjects_pyList,
          bp::args("list"),
          "Convert HitObjects back to a OMKeyMap");
}
