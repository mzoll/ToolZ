/**
 * \file IC86Topology.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.cxx 144357 2016-04-07 11:49:02Z mzoll $
 * \version $Revision: 144357 $
 * \date $Date: 2016-04-07 13:49:02 +0200 (tor, 07 apr 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Pybindings to the IceHiveHelpers
 */

#include <boost/python.hpp>
#include <boost/make_shared.hpp>
#include <boost/python/scope.hpp>

#include "ToolZ/IC86Topology.h"

namespace bp = boost::python;


namespace pyIC86Topology {
  I3GeometryPtr pyBuild_IC86_Geometry() {
    return boost::make_shared<I3Geometry>(IC86Topology::Build_IC86_Geometry());
  };
};

void register_IC86Topology() {
  using namespace bp;
  
  //scope outer =
  class_<IC86Topology>("IC86Topology")
    .def("is_icetop", &IC86Topology::IsIceTop, (bp::arg("omkey")), "Is this in IceTop?")
    .staticmethod("is_icetop")
    .def("is_inice", &IC86Topology::IsInIce, (bp::arg("omkey")), "Is this omkey in ice?" )
    .staticmethod("is_inice")
    .def("is_icecube", &IC86Topology::IsIceCube, (bp::arg("omkey")), "Is this in regular IceCube?" )  
    .staticmethod("is_icecube")
    .def("is_deepcore", &IC86Topology::IsDeepCore, (bp::arg("omkey")), "Is this in DeepCore?" )
    .staticmethod("is_deepcore")
    .def("is_deepcorecap", &IC86Topology::IsDeepCoreCap, (bp::arg("omkey")), "Is this in the DeepCore Veto layer?" )
    .staticmethod("is_deepcorecap")
    .def("is_deepcorefidutial", &IC86Topology::IsDeepCoreFidutial, (bp::arg("omkey")), "Is this in the denser populated DeepCore region?" )
    .staticmethod("is_deepcorefidutial")
    .def("is_deepcoredense", &IC86Topology::IsDeepCoreDense, (bp::arg("omkey")), "Is this in the any dense populated region?" )
    .staticmethod("is_deepcoredense")
    .def("Geometry",
      &pyIC86Topology::pyBuild_IC86_Geometry, // &IC86Topology::Build_IC86_Geometry,
      "Build the I3Geometry of IC86 as an idealistic detector configuration ")
    .staticmethod("Geometry")
    .def("OMTopologyMap",
      &IC86Topology::Build_IC86_OMKeyTopologyMap,
      "Build the OMTopologyMap for IC86 with the most common applications")
    .staticmethod("OMTopologyMap")
    ;
};

