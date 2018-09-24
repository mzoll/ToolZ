/**
 * copyright  (C) 2012
 * the IceCube Collaboration
 * $Id: $
 *
 * @file HashedGeometry.cxx
 * @date $Date: 2012-12-20$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include <string>

#include "icetray/python/stream_to_string.hpp"
#include "dataclasses/ostream_overloads.hpp"

#include "ToolZ/HashedGeometry.h"

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include "icetray/python/dataclass_suite.hpp"

namespace bp = boost::python;

///namespace containing specializations and casting-functions for pybindings
namespace pyHashedGeometry {
  HashedGeometryConstPtr
  HashedGeometryConstPtr_From_pyOMKeyList(
    const I3OMGeoMap& omgeo,
    const bp::list& l) 
  {
    std::set<OMKey> omkeys;
    for (int i=0; i<bp::len(l); i++) {
      omkeys.insert(bp::extract<OMKey>(l[i]));
      //std::cout << bp::extract<OMKey>(l[i]) << std::endl;
    }
    return boost::make_shared<const HashedGeometry>(omgeo, omkeys);
  };
}

///namespace containin pybinings for OMKeyHash library
void register_HashedGeometry()
{
  //=== class HashedGeometry
  bp::class_<HashedGeometry, HashedGeometryPtr>("HashedGeometry", bp::init<const I3OMGeoMap&>(bp::args("omgeo"), "hash all OMs inthis Geometry"))
    .def("__init__",
      bp::make_constructor(&pyHashedGeometry::HashedGeometryConstPtr_From_pyOMKeyList) )
    .def_readonly("hashService",
      &HashedGeometry::GetHashService)
    .def_readonly("posService",
      &HashedGeometry::GetPosService)
    .def_readonly("distService",
      &HashedGeometry::GetDistService)
  ;
    
  bp::implicitly_convertible<CompactOMKeyHashServicePtr, CompactOMKeyHashServiceConstPtr>();
}
