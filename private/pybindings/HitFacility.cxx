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
#include "ToolZ/HitFacility.h"

using namespace HitSorting;
namespace bp = boost::python;

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include "icetray/python/dataclass_suite.hpp"

///namespace containing specializations and casting-functions for pybindings
namespace pyHitFacility {
  
  //=== I3RecoPulse ===
  bp::list I3RecoPulse_GetHits_pyList(I3RecoPulseSeriesMap_HitFacility &hf, const std::string& key) {
    bp::list l;
    const HitList hits(hf.GetHits<HitList>(key));
    BOOST_FOREACH (const Hit& h, hits)
      l.append(h);
    return l;
  }
  
  I3RecoPulseSeriesMap I3RecoPulseSeriesMapFromHits_pyList(const I3RecoPulseSeriesMap_HitFacility &hf, const bp::list& l) {
    HitList hits_vec;
    for (int i=0; i<bp::len(l); i++)
      hits_vec.push_back(bp::extract<Hit>(l[i]));
    return hf.MapFromHits(hits_vec);
  }
  
  I3RecoPulseSeriesMapMask I3RecoPulseSeriesMapMaskFromHits_pyList(const I3RecoPulseSeriesMap_HitFacility &hf, const bp::list& l) {
   HitList hits_vec;
    for (int i=0; i<bp::len(l); i++)
      hits_vec.push_back(bp::extract<Hit>(l[i]));
    return hf.MaskFromHits(hits_vec);
  }
  
  I3RecoPulse_HitObject I3RecoPulse_HitObjectFromHitFacility(const I3RecoPulseSeriesMap_HitFacility &hf, const Hit &h) {
    return I3RecoPulse_HitObject(hf.GetHitObject(h));
  }
  
  boost::shared_ptr<OMKeyMap_HitFacility<I3RecoPulse> >
  pyOMKeyMap_HitFacility_I3RecoPulse_Ctor_wrapper (
    const I3FrameConstPtr& frame,
    const std::string & key)
  { 
    CompactOMKeyHashServiceConstPtr hasher;
    return boost::make_shared<OMKeyMap_HitFacility<I3RecoPulse> >(frame, key, hasher);
  };
  

  //=== I3DOMLaunch ===
  bp::list I3DOMLaunch_GetHits_pyList(I3DOMLaunchSeriesMap_HitFacility &hf) {
    bp::list l;
    const HitList hits = hf.GetHits<HitList>();
    BOOST_FOREACH (const Hit &h, hits)
      l.append(h);
    return l;
  }
  
  I3DOMLaunchSeriesMap I3DOMLaunchSeriesMapFromHits_pyList(const I3DOMLaunchSeriesMap_HitFacility &hf, const bp::list& l) {
    HitList hits_vec;
    for (int i=0; i<bp::len(l); i++)
      hits_vec.push_back(bp::extract<Hit>(l[i]));
    return hf.MapFromHits(hits_vec);
  }
  
  I3DOMLaunch_HitObject I3DOMLaunch_HitObjectFromHitFacility(const I3DOMLaunchSeriesMap_HitFacility &hf, const Hit &h) {
    return I3DOMLaunch_HitObject(hf.GetHitObject(h));
  }
  
  boost::shared_ptr<const OMKeyMap_HitFacility<I3DOMLaunch> >
  pyOMKeyMap_HitFacility_I3DOMLaunch_Ctor_wrapper (
    const I3FrameConstPtr& frame,
    const std::string & key)
  { 
    CompactOMKeyHashServiceConstPtr hasher;
    return boost::make_shared<const OMKeyMap_HitFacility<I3DOMLaunch> >(frame, key, hasher);
  };
  
  
  //=== I3MCHit ===
  bp::list I3MCHit_GetHits_pyList(I3MCHitSeriesMap_HitFacility &hf) {
    bp::list l;
    const HitList hits = hf.GetHits<HitList>();
    BOOST_FOREACH (const Hit &h, hits)
      l.append(h);
    return l;
  }
  
  I3MCHitSeriesMap I3MCHitSeriesMapFromHits_pyList(const I3MCHitSeriesMap_HitFacility &hf, const bp::list& l) {
    HitList hits_vec;
    for (int i=0; i<bp::len(l); i++)
      hits_vec.push_back(bp::extract<Hit>(l[i]));
    return hf.MapFromHits(hits_vec);
  }
  
  I3MCHit_HitObject I3MCHit_HitObjectFromHitFacility(const I3MCHitSeriesMap_HitFacility &hf, const Hit &h) {
    return I3MCHit_HitObject(hf.GetHitObject(h));
  }
  
  boost::shared_ptr<const OMKeyMap_HitFacility<I3MCHit> >
  pyOMKeyMap_HitFacility_I3MCHit_Ctor_wrapper (
    const I3FrameConstPtr& frame,
    const std::string & key)
  { 
    CompactOMKeyHashServiceConstPtr hasher;
    return boost::make_shared<const OMKeyMap_HitFacility<I3MCHit> >(frame, key, hasher);
  };
  
  //=== I3MCHit ===
  bp::list I3MCPulse_GetHits_pyList(I3MCPulseSeriesMap_HitFacility &hf) {
    bp::list l;
    const HitList hits = hf.GetHits<HitList>();
    BOOST_FOREACH (const Hit &h, hits)
      l.append(h);
    return l;
  }
  
  I3MCPulseSeriesMap I3MCPulseSeriesMapFromHits_pyList(const I3MCPulseSeriesMap_HitFacility &hf, const bp::list& l) {
    HitList hits_vec;
    for (int i=0; i<bp::len(l); i++)
      hits_vec.push_back(bp::extract<Hit>(l[i]));
    return hf.MapFromHits(hits_vec);
  }
  
  I3MCPulse_HitObject I3MCPulse_HitObjectFromHitFacility(const I3MCPulseSeriesMap_HitFacility &hf, const Hit &h) {
    return I3MCPulse_HitObject(hf.GetHitObject(h));
  }
  
  boost::shared_ptr<const OMKeyMap_HitFacility<I3MCPulse> >
  pyOMKeyMap_HitFacility_I3MCPulse_Ctor_wrapper (
    const I3FrameConstPtr& frame,
    const std::string & key)
  { 
    CompactOMKeyHashServiceConstPtr hasher;
    return boost::make_shared<const OMKeyMap_HitFacility<I3MCPulse> >(frame, key, hasher);
  };
  
}


///namespace containing pybinings for HitSorting library
void register_HitFacility()
{
  //=== class OMKeyMap_HitFacility
  bp::class_<I3DOMLaunchSeriesMap_HitFacility>("I3DOMLaunchSeriesMap_HitFacility",
    bp::init<const I3FrameConstPtr&,
             const std::string &,
             CompactOMKeyHashServiceConstPtr&>
        (bp::args("frame","key","hasher"),
        "Facilitate Hit extraction with a predefined hashing function"))
    .def("__init__",
         &pyHitFacility::pyOMKeyMap_HitFacility_I3DOMLaunch_Ctor_wrapper,
         bp::args("frame","key"),
         "Facilitate Hit extraction; Hashing will follow natural order of OMKeys in the map")
    .def("GetHits", 
         &pyHitFacility::I3DOMLaunch_GetHits_pyList,
         "Extract hits from map at this key")
    .def("MapFromHits", 
         &pyHitFacility::I3DOMLaunchSeriesMapFromHits_pyList, 
         (bp::arg("hits")), 
         "Convert Hits extracted from this HitFacility back to a regular I3Map")
    .def("GetHitObject", 
         &pyHitFacility::I3DOMLaunch_HitObjectFromHitFacility,
         (bp::arg("hit"),
         "Retrieve this hit back to a HitObject"));  

    bp::class_<I3MCHitSeriesMap_HitFacility>("I3MCHitSeriesMap_HitFacility",
        bp::init<const I3FrameConstPtr&,
             const std::string &,
             CompactOMKeyHashServiceConstPtr&>
        (bp::args("frame","key","hasher"),
        "Facilitate Hit extraction with a predefined hashing function"))
    .def("__init__",
         &pyHitFacility::pyOMKeyMap_HitFacility_I3MCHit_Ctor_wrapper,
         bp::args("frame","key"),
         "Facilitate Hit extraction; Hashing will follow natural order of OMKeys in the map")
    .def("GetHits", 
          &pyHitFacility::I3MCHit_GetHits_pyList,
         "Extract hits from map at this key")
    .def("MapFromHits", 
         &pyHitFacility::I3MCHitSeriesMapFromHits_pyList,
         bp::arg("hits"), 
         "Convert Hits extracted from this HitFacility back to a regular I3Map")
    .def("GetHitObject", 
         &pyHitFacility::I3MCHit_HitObjectFromHitFacility, 
         (bp::arg("hit"),
          "Retrieve this hit back to a HitObject"));
  
  bp::class_<I3MCPulseSeriesMap_HitFacility>("I3MCPulseSeriesMap_HitFacility",
        bp::init<const I3FrameConstPtr&,
             const std::string &,
             CompactOMKeyHashServiceConstPtr&>
        (bp::args("frame","key","hasher"),
        "Facilitate Hit extraction with a predefined hashing function"))
    .def("__init__",
         &pyHitFacility::pyOMKeyMap_HitFacility_I3MCPulse_Ctor_wrapper,
         bp::args("frame","key"),
         "Facilitate Hit extraction; Hashing will follow natural order of OMKeys in the map")
    .def("GetHits", 
         &pyHitFacility::I3MCPulse_GetHits_pyList,
        "Extract hits from map at this key")
    .def("MapFromHits", 
         &pyHitFacility::I3MCPulseSeriesMapFromHits_pyList,
         bp::arg("hits"), 
         "Convert Hits extracted from this HitFacility back to a regular I3Map")
    .def("GetHitObject", 
         &pyHitFacility::I3MCPulse_HitObjectFromHitFacility, 
         bp::arg("hit"),
         "Retrieve this hit back to a HitObject");    

  bp::class_<I3RecoPulseSeriesMap_HitFacility>("I3RecoPulseSeriesMap_HitFacility",
         bp::init<const I3FrameConstPtr&,
             const std::string &,
             CompactOMKeyHashServiceConstPtr&>
        (bp::args("frame","key","hasher"),
        "Facilitate Hit extraction with a predefined hashing function"))
    .def("__init__",
         &pyHitFacility::pyOMKeyMap_HitFacility_I3RecoPulse_Ctor_wrapper,
         bp::args("frame","key"),
         "Facilitate Hit extraction; Hashing will follow natural order of OMKeys in the map")
    .def("GetHits", 
         &pyHitFacility::I3RecoPulse_GetHits_pyList,
         bp::arg("key"), 
        "Extract hits from map/mask at this key")
    .def("MapFromHits", 
         &pyHitFacility::I3RecoPulseSeriesMapFromHits_pyList,
         bp::arg("hits"), 
         "Convert Hits extracted from this HitFacility back to a regular I3Map")
    .def("MaskFromHits", 
         &pyHitFacility::I3RecoPulseSeriesMapMaskFromHits_pyList,
         bp::arg("hits"), 
         "Convert Hits extracted from this HitFacility back to a I3Mask")
    .def("GetHitObject", 
         &pyHitFacility::I3RecoPulse_HitObjectFromHitFacility, 
         bp::arg("hit"),
         "Retrieve this hit back to a HitObject");
}
