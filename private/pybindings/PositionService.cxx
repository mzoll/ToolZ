/**
 * \file PositionService.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.cxx 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Pybindings to the PositionService
 */

#include <boost/python.hpp>

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/PositionService.h"

#include <boost/make_shared.hpp>

namespace bp = boost::python;

namespace pyPositionService {
  ///wrap away the haser to be held only internally
  PositionServiceConstPtr pyPositionService(const I3OMGeoMap& omgeo) {
    CompactOMKeyHashServiceConstPtr hasher;
    return boost::make_shared<const PositionService>(omgeo, hasher);
  }
}


//Do the pybindings
void register_PositionService() {
  //================= PositionService ===========================
  bp::class_<PositionService>(
    "PositionService", bp::no_init)
    .def("__init__", //FIXME bp::make_constructor 
      &pyPositionService::pyPositionService,
      bp::args("geo"),
      "Hashes Positions and Distances, hold internal Hasher")
    .def(bp::init<I3OMGeoMap,CompactOMKeyHashServiceConstPtr>(
      bp::args("geo", "hasher"),"Hashes Positions and Distances; hold external hasher"))
    .def("GetPosition",
      (const I3Position& (PositionService::*)(const CompactHash) const)&PositionService::GetPosition,
      bp::return_value_policy<bp::return_by_value>(),
      bp::args("compacthash"),
      "Get the Position of this OMKeyHash")
    .def("GetPosition",
      (const I3Position& (PositionService::*)(const OMKey&) const)&PositionService::GetPosition,
      bp::return_value_policy<bp::return_by_value>(),
      bp::args("compacthash"),
      "Get the Position of this OMKey")
    ;
};

