/**
 * copyright  (C) 2012
 * the IceCube Collaboration
 * $Id: $
 *
 * @file OMKeyHash.h
 * @date $Date: 2012-12-20$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include <string>

#include "icetray/python/stream_to_string.hpp"
#include "dataclasses/ostream_overloads.hpp"

#include "dataclasses/physics/I3RecoPulse.h"

#include "ToolZ/OMKeyHash.h"

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include "icetray/python/dataclass_suite.hpp"

namespace bp = boost::python;

///namespace containing specializations and casting-functions for pybindings
namespace pyOMKeyHash {
  CompactOMKeyHashServicePtr
  CompactOMKeyHashService_From_pyOMKeyList(const bp::list& l) {
    std::set<OMKey> omkeys;
    for (int i=0; i<bp::len(l); i++) {
      omkeys.insert(bp::extract<OMKey>(l[i]));
      //std::cout << bp::extract<OMKey>(l[i]) << std::endl;
    }
    return boost::make_shared<CompactOMKeyHashService>(omkeys);
  };
  
  bp::list
  ExtractOMKeys_From_pyI3Geo(const I3GeometryConstPtr& geo) {
    bp::list pylist;
    BOOST_FOREACH(const OMKey& omkey , ExtractOMKeys(geo)) {
      pylist.append(omkey);
    }
    return pylist;
  };
}

///namespace containin pybinings for OMKeyHash library
void register_OMKeyHash()
{
  bp::def("ExtractOMKeys",
    &pyOMKeyHash::ExtractOMKeys_From_pyI3Geo,
    bp::args("I3Geometry"),
    "Extract the set of OMKeys from the Geometry");
  
  //=== class OMKeyMap_HitFacility
  bp::class_<CompactOMKeyHashService, CompactOMKeyHashServicePtr>("CompactOMKeyHashService", bp::no_init)     
    .def("__init__",
      bp::make_constructor(&pyOMKeyHash::CompactOMKeyHashService_From_pyOMKeyList) ) //,
//       bp::args("omkeylist"),
//       "Create a Hasher indexing only a certain list of OMKeys")
    .def("OMKeyFromHash",  
      &CompactOMKeyHashService::OMKeyFromHash,
      (bp::arg("hash")),
      "Convert a Hash back to an OMKey")
    .def("HashFromOMKey",
      &CompactOMKeyHashService::HashFromOMKey,
      (bp::arg("omkey")),
      "Convert a OMKey into a Hash")
    .def("HoldsOMKey",
      &CompactOMKeyHashService::HoldsOMKey,
      (bp::arg("omkey")),
      "Hasher holds this OMKey")
    .def("HoldsHash",
      &CompactOMKeyHashService::HoldsHash,
      (bp::args("hash")),
      "Hasher holds this hash")
    .def("HashSize",
      &CompactOMKeyHashService::HashSize,
      "This is the maximal hash hold")
  ;
    
  bp::implicitly_convertible<CompactOMKeyHashServicePtr, CompactOMKeyHashServiceConstPtr>();
}
