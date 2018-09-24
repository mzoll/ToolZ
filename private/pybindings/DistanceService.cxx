/**
 * \file DistanceService.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: IceHiveHelpers.cxx 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Pybindings to the DistanceService
 */

#include <boost/python.hpp>

#include "ToolZ/OMKeyHash.h"
#include "ToolZ/DistanceService.h"

#include <boost/make_shared.hpp>

namespace bp = boost::python;

namespace pyDistanceService {
}


//Do the pybindings
void register_DistanceService() {
  //================= DistanceService ===========================
  bp::class_<DistanceService>(
    "DistanceService", bp::init<PositionServiceConstPtr>(bp::args("posService"), "make DistanceService for already hashed Position" ))
    .def("GetDistance",
      (double (DistanceService::*)(const CompactHash, const CompactHash) const)&DistanceService::GetDistance,
      bp::args("compacthash", "compacthash"),
      "Get the Distance between these DOMs")
    .def("GetDistance",
      (double (DistanceService::*)(const OMKey&, const OMKey&) const)&DistanceService::GetDistance,
      bp::args("omkey", "omkey"),
      "Get the Distance between these DOMs")
    .def("HashAllDistances",
      &DistanceService::HashAllDistances,
      "Hash all Distances")
    ;
};

