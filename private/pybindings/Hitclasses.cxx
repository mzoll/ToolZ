/**
 * copyright  (C) 2012
 * the IceCube Collaboration
 * $Id: $
 *
 * @file HitclassesPybindings.h
 * @date $Date: 2012-12-20$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include <string>

#include "icetray/python/stream_to_string.hpp"
#include "dataclasses/ostream_overloads.hpp"
#include "ToolZ/Hitclasses.h"
#include "ToolZ/HitSorting.h"

namespace bp = boost::python;

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include "icetray/python/dataclass_suite.hpp"

///namespace containing specializations and casting-functions for pybindings
namespace pyHitclasses {
  
  //=== I3RecoPulse ===
  bp::list I3RecoPulse_GetHitObjects_pyList(const I3RecoPulseSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3RecoPulse> > hitObjs 
      = HitSorting::OMKeyMap_To_HitObjects<I3RecoPulse, std::list<HitObject<I3RecoPulse> > >(m);
    BOOST_FOREACH (const HitObject<I3RecoPulse> &ho, hitObjs)
      l.append(ho);
    return l;
  }

  I3RecoPulse_HitObject I3RecoPulse_HitObjectFromHit(const Hit &h) {
    return I3RecoPulse_HitObject(h.GetAssociatedHitObject<I3RecoPulse>());
  }
  
  
  //=== I3DOMLaunch ===
  bp::list I3DOMLaunch_GetHitObjects_pyList(const I3DOMLaunchSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3DOMLaunch> > hitObjs 
      = HitSorting::OMKeyMap_To_HitObjects<I3DOMLaunch,std::list<HitObject<I3DOMLaunch> > >(m);
    BOOST_FOREACH (const HitObject<I3DOMLaunch> &ho, hitObjs)
      l.append(ho);
    return l;
  }

  I3DOMLaunch_HitObject I3DOMLaunch_HitObjectFromHit(const Hit &h) {
    return I3DOMLaunch_HitObject(h.GetAssociatedHitObject<I3DOMLaunch>());
  }
  
  
  //=== I3MCHit ===
  bp::list I3MCHit_GetHitObjects_pyList(const I3MCHitSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3MCHit> > hitObjs 
      = HitSorting::OMKeyMap_To_HitObjects<I3MCHit, std::list<HitObject<I3MCHit> > >(m);
    BOOST_FOREACH (const HitObject<I3MCHit> &ho, hitObjs)
      l.append(ho);
    return l;
  }

  I3MCHit_HitObject I3MCHit_HitObjectFromHit(const Hit &h) {
    return I3MCHit_HitObject(h.GetAssociatedHitObject<I3MCHit>());
  }
  
  
  //=== I3MCHit ===
  bp::list I3MCPulse_GetHitObjects_pyList(const I3MCPulseSeriesMap &m) {
    bp::list l;
    const std::list<HitObject<I3MCPulse> > hitObjs
      = HitSorting::OMKeyMap_To_HitObjects<I3MCPulse,std::list<HitObject<I3MCPulse> > >(m);
    BOOST_FOREACH (const HitObject<I3MCPulse> &ho, hitObjs)
      l.append(ho);
    return l;
  }
  
  I3MCPulse_HitObject I3MCPulse_HitObjectFromHit(const Hit &h) {
    return I3MCPulse_HitObject(h.GetAssociatedHitObject<I3MCPulse>());
  };
};


///namespace containing pybinings for HitSorting library
void register_Hitclasses()
{
  //=== HitObjects
  bp::class_<I3RecoPulse_HitObject>("I3RecoPulse_HitObject", bp::init<OMKey, I3RecoPulse>(bp::args("omkey","recoPulse"),"Description of a simplyfied Hit" ))
     .def_readonly("omkey", 
         bp::make_function(&I3RecoPulse_HitObject::GetOMKey, bp::return_value_policy<bp::copy_const_reference>()))
    .def_readonly("time", 
         &I3RecoPulse_HitObject::GetTime)
    .def_readonly("response", 
         bp::make_function(&I3RecoPulse_HitObject::GetResponseObj, bp::return_value_policy<bp::copy_const_reference>()))
    .def(bp::self < bp::self)
    .def("__str__", 
         &stream_to_string<HitObject<I3RecoPulse> >); 

  bp::class_<I3DOMLaunch_HitObject>("I3DOMLaunch_HitObject", bp::init<OMKey, I3DOMLaunch>(bp::args("omkey","domLaunch"),"Description of a simplyfied Hit" ))
    .def_readonly("omkey", 
         bp::make_function(&I3DOMLaunch_HitObject::GetOMKey, bp::return_value_policy<bp::copy_const_reference>()))
    .def_readonly("time", 
         &I3DOMLaunch_HitObject::GetTime)
    .def_readonly("response", 
         bp::make_function(&I3DOMLaunch_HitObject::GetResponseObj, bp::return_value_policy<bp::copy_const_reference>()))
    .def(bp::self < bp::self)
    .def("__str__", 
         &stream_to_string<HitObject<I3DOMLaunch> >); 

  bp::class_<I3MCHit_HitObject>("I3MCHit_HitObject", bp::init<OMKey, I3MCHit>(bp::args("omkey","mcHit"),"Description of a simplyfied Hit" ))
    .def_readonly("omkey", 
      bp::make_function(&I3MCHit_HitObject::GetOMKey, bp::return_value_policy<bp::copy_const_reference>()))
    .def_readonly("time", 
         &I3MCHit_HitObject::GetTime)
    .def_readonly("response", 
         bp::make_function(&I3MCHit_HitObject::GetResponseObj, bp::return_value_policy<bp::copy_const_reference>()))
    .def(bp::self < bp::self)
    .def("__str__", 
         &stream_to_string<HitObject<I3MCHit> >); 
  
  bp::class_<I3MCPulse_HitObject>("I3MCPulse_HitObject", bp::init<OMKey, I3MCPulse>(bp::args("omkey","mcPulse"),"Description of a simplified Hit" ))
    .def_readonly("omkey", 
         bp::make_function(&I3MCPulse_HitObject::GetOMKey, bp::return_value_policy<bp::copy_const_reference>()))
    .def_readonly("time", 
         &I3MCPulse_HitObject::GetTime)
    .def_readonly("response", 
         bp::make_function(&I3MCPulse_HitObject::GetResponseObj, bp::return_value_policy<bp::copy_const_reference>()))
    .def(bp::self < bp::self)
    .def("__str__", 
         &stream_to_string<HitObject<I3MCPulse> >);
  

  //===== class Hit
  bp::class_<Hit, boost::shared_ptr<Hit> >("Hit", bp::no_init)
    .def_readonly("domindex", 
      bp::make_function(&Hit::GetDOMIndex, bp::return_value_policy<bp::copy_const_reference>()))
    .def_readonly("time", 
      bp::make_function(&Hit::GetTime, bp::return_value_policy<bp::copy_const_reference>()))
    .def(bp::self == bp::self)
    .def(bp::self < bp::self)
    .def("__str__", 
         &stream_to_string<Hit>);

  bp::def("GetAssociatedI3RecoPulse_HitObject",  &pyHitclasses::I3RecoPulse_HitObjectFromHit, //&Hit::GetAssociatedHitObject<I3RecoPulse>, bp::return_value_policy<bp::return_internal_reference>,
          bp::args("hit"),
          "Get the associated I3RecoPulse_HitObject of this hit");
  bp::def("GetAssociatedI3DOMLaunch_HitObject", &pyHitclasses::I3DOMLaunch_HitObjectFromHit,
          bp::args("hit"),
          "Get the associated I3DOMLaunch_HitObject of this hit");
  bp::def("GetAssociatedI3MCPulse_HitObject", &pyHitclasses::I3MCHit_HitObjectFromHit,
          bp::args("hit"),
          "Get the associated I3MCHit_HitObject of this hit");
  bp::def("GetAssociatedI3MCPulse_HitObject", &pyHitclasses::I3MCPulse_HitObjectFromHit,
          bp::args("hit"),
          "Get the associated I3MCPulse_HitObject of this hit");
}
