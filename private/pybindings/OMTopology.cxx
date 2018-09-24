/**
 * copyright  (C) 2012
 * the IceCube Collaboration
 * $Id: $
 *
 * @file OMTopology.cxx
 * @date $Date: 2012-12-20$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include <string>

#include "icetray/python/stream_to_string.hpp"
#include "icetray/python/dataclass_suite.hpp"
#include "dataclasses/ostream_overloads.hpp"

#include "ToolZ/OMTopology.h"
#include "ToolZ/Interfaces.h"

#include <boost/python/operators.hpp>
#include <boost/operators.hpp>
#include <boost/python/list.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include "helper.h"

namespace bp = boost::python;

///namespace containing specializations and casting-functions for pybindings
namespace pyOMTopology {
  
  struct OMTopology_pyWrapper {
    static
    OMTopologyPtr initWrapper (const bp::list& pyl) {
      return boost::make_shared<OMTopology>(PyListToStdList<std::string>(pyl));
    };
    
    static bool GetIceTop(const OMTopology& omt) {return omt.GetFlag("IceTop");};
    static void SetIceTop(OMTopology& omt, const bool val) {omt.SetFlag("IceTop", val);};
    static bool GetInIce(const OMTopology& omt) {return omt.GetFlag("InIce");};
    static void SetInIce(OMTopology& omt, const bool val) {omt.SetFlag("InIce", val);};
    static bool GetIceCube(const OMTopology& omt) {return omt.GetFlag("IceCube");};
    static void SetIceCube(OMTopology& omt, const bool val) {omt.SetFlag("IceCube", val);};
    static bool GetDeepCore(const OMTopology& omt) {return omt.GetFlag("DeepCore");};
    static void SetDeepCore(OMTopology& omt, const bool val) {omt.SetFlag("DeepCore", val);};
    static bool GetGen2(const OMTopology& omt) {return omt.GetFlag("Gen2");};
    static void SetGen2(OMTopology& omt, const bool val) {omt.SetFlag("Gen2", val);};
    static bool GetPingu(const OMTopology& omt) {return omt.GetFlag("Pingu");};
    static void SetPingu(OMTopology& omt, const bool val) {omt.SetFlag("Pingu", val);};
    static bool GetDeepCoreFidutial(const OMTopology& omt) {return omt.GetFlag("DeepCoreFidutial");};
    static void SetDeepCoreFidutial(OMTopology& omt, const bool val) {omt.SetFlag("DeepCoreFidutial", val);};
    static bool GetDeepCoreCap(const OMTopology& omt) {return omt.GetFlag("DeepCoreCap");};
    static void SetDeepCoreCap(OMTopology& omt, const bool val) {omt.SetFlag("DeepCoreCap", val);};
    
    static bp::list GetFlags(const OMTopology& omt) {return StdListToPyList<std::string>(omt.GetFlags());};
  };
    
  bp::list pyOMTopologyMap_Comparator_AllCommon(const OMTopologyMap_Comparator& omcomp) {
    return StdVectorToPyList( omcomp.AllCommon() );
  };
  
  #if SERIALIZATION_ENABLED
  struct OMTopologyMap_pyWrapper {
    static
    void 
    WriteToFile(const OMTopologyMapPtr& omtm, const std::string& filename)
      { fileinterfaces::WriteToFile<OMTopologyMap>(omtm.get(), filename); };

    static
    OMTopologyMapPtr
    ReadFromFile(const std::string& filename)
      { OMTopologyMap* omtopomap;
        fileinterfaces::ReadFromFile<OMTopologyMap>(omtopomap, filename);
        return OMTopologyMapPtr(omtopomap);
      };
  };
  #endif //SERIALIZATION_ENABLED  
};

///namespace containing pybindings for OMKeyHash library
void register_OMTopology()
{
  using namespace pyOMTopology;
  
  bp::class_<OMTopology, OMTopologyPtr>("OMTopology")
    .def("__init__",
         make_constructor(&OMTopology_pyWrapper::initWrapper),
         "Initialize directly specifying all flags in a list")
    .add_property("IceTop",
                  &OMTopology_pyWrapper::GetIceTop,
                  &OMTopology_pyWrapper::SetIceTop)
    .add_property("InIce",
                  &OMTopology_pyWrapper::GetIceTop,
                  &OMTopology_pyWrapper::SetIceTop)
    .add_property("IceCube",
                  &OMTopology_pyWrapper::GetIceCube,
                  &OMTopology_pyWrapper::SetIceCube)
    .add_property("DeepCore",
                  &OMTopology_pyWrapper::GetDeepCore,
                  &OMTopology_pyWrapper::SetDeepCore)
    .add_property("Gen2",
                  &OMTopology_pyWrapper::GetGen2,
                  &OMTopology_pyWrapper::SetGen2)
    .add_property("Pingu",
                  &OMTopology_pyWrapper::GetPingu,
                  &OMTopology_pyWrapper::SetPingu)
    .add_property("DeepCoreFidutial",
                  &OMTopology_pyWrapper::GetDeepCoreFidutial,
                  &OMTopology_pyWrapper::SetDeepCoreFidutial)
    .add_property("DeepCoreCap",
                  &OMTopology_pyWrapper::GetDeepCoreCap,
                  &OMTopology_pyWrapper::SetDeepCoreCap) 
    .def("commonFlags", &OMTopology::Common, bp::args("OMTopology"), "have common flags")
    .def("getFlags", &OMTopology_pyWrapper::GetFlags, "get all flags that are set by their name")
    .def(str(bp::self))
    .def(bp::dataclass_suite<OMTopology>())
    ;
  bp::implicitly_convertible<OMTopologyPtr, OMTopologyConstPtr>();
  
    
  bp::class_<OMTopologyMap, OMTopologyMapPtr>("OMTopologyMap")
    .def(bp::dataclass_suite<OMTopologyMap>())
    #if SERIALIZATION_ENABLED
    .def("to_file",
      OMTopologyMap_pyWrapper::WriteToFile,
      bp::args("filename"),
      "Write to file")
    .def("from_file",
      OMTopologyMap_pyWrapper::ReadFromFile,
      bp::args("filename"),
      "Read from file")
    #endif //SERIALIZATION_ENABLED
    ;
    
  register_pointer_conversions<OMTopologyMap>();
  
  //=== class OMTopologyMap_Comparator
  bp::class_<OMTopologyMap_Comparator, OMTopologyMap_ComparatorPtr>("OMTopology_Comparator",
    bp::init<const OMTopology&, const OMTopologyMap&>(
      bp::args("OMTopology_Flags","OMTopologyMap"),
      "Create a comparision operator of OMKeys to the specified flag set"))
    .def("__call__", 
      &OMTopologyMap_Comparator::operator(),
      bp::args("OMKey"),
      "Has this OMKey common flags")
    .def("AllCommon",
      &pyOMTopology::pyOMTopologyMap_Comparator_AllCommon,
      "return all OMKeys which have these common flags")
    ;
}
