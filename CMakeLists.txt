SET(PROJECT_NAME ToolZ)

#=== SERIALIZATION SUPPORT ==
#0 No Serialization support
#1 BOOST::SERIALIZATION support
#2 ICECUBE::SERIALIZATION support
#SET(SERIALIZATION_SUPPORT 2 CACHE STRING "0:No serialization, 1:Boost, 2:IceCube")

if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/serialization")
  colormsg(GREEN "+-- ICECUBE::serialization support found")
  SET(SERIALIZATION_SUPPORT 2)
elseif(BOOST_FOUND)
  colormsg(GREEN "+-- BOOST::serialization support found")
  SET(SERIALIZATION_SUPPORT 1)
else()
  colormsg(YELLOW "+-- No serialization support found")
  SET(SERIALIZATION_SUPPORT 0)
endif()

# SET(SERIALIZATION_SUPPORT 2) #//DANGER OVERIDE

if (SERIALIZATION_SUPPORT EQUAL 0)
  colormsg(YELLOW "+-- no support for object serialization configured")
  colormsg(RED    "+--   SERIALIZATION functionality will be disabled")
elseif (SERIALIZATION_SUPPORT EQUAL 1)
  colormsg(GREEN  "+-- BOOST::serialization support configured")
  colormsg(YELLOW  "+--   library might conflict with ICECUBE::serialization")
elseif (SERIALIZATION_SUPPORT EQUAL 2)
  colormsg(GREEN "+-- ICECUBE::serialization support configured")
else ()
  message(FATAL_ERROR "wrongly configured internal flag 'SERIALIZATION_SUPPORT'")
endif()

if(CMAKE_CXX_STANDARD)
  if (CMAKE_CXX_STANDARD EQUAL 11)
    colormsg(GREEN "+-- Supporting C++11")
  else()
    colormsg(GREEN "+-- Supporting C++${CMAKE_CXX_STANDARD}")
  endif()
else()
  if(SERIALIZATION_SUPPORT EQUAL 2)
    SET (CMAKE_CXX_STANDARD 11)
    colormsg(YELLOW "+-- OVERRIDE:local set 'CMAKE_CXX_STANDARD' to 11")
  else()
    SET (CMAKE_CXX_STANDARD 98)
    colormsg(YELLOW "+-- OVERRIDE:local set 'CMAKE_CXX_STANDARD' to 98")
  endif()
endif()

i3_project(${PROJECT_NAME}
  DOCS_DIR resources/docs
  PYTHON_DIR python
)

SET(LIB_${PROJECT_NAME}_HEADERFILES
  public/ToolZ/__SERIALIZATION.h
  
  public/ToolZ/SetHelpers.h
  
  public/ToolZ/GCDinfo.h
  
  public/ToolZ/OMKeyHash.h
  public/ToolZ/Hitclasses.h
  public/ToolZ/HitSorting.h
  public/ToolZ/HitFacility.h
  public/ToolZ/HitSorting_FirstHit.h
  public/ToolZ/HitFacility_FirstHit.h
  public/ToolZ/I3RUsageTimer.h
  public/ToolZ/IndexMatrix.h
  public/ToolZ/PositionService.h
  public/ToolZ/DistanceService.h
  public/ToolZ/HashedGeometry.h
  public/ToolZ/OMTopology.h
  public/ToolZ/IC86Topology.h
  
  public/ToolZ/PartialCOG.h
  
  public/ToolZ/ResponseMapHelpers.h
  public/ToolZ/TriggerHierarchyHelpers.h
  public/ToolZ/MiscHelpers.h

  public/ToolZ/Interfaces.h
  
  public/ToolZ/FrameRegister.h
  public/ToolZ/I3RegisterModule.h
)

SET(LIB_${PROJECT_NAME}_SOURCEFILES
  private/ToolZ/SetHelpers.cxx

  private/ToolZ/GCDinfo.cxx
  
  private/ToolZ/OMKeyHash.cxx
  private/ToolZ/Hitclasses.cxx
  private/ToolZ/HitSorting.cxx
  private/ToolZ/HitFacility.cxx
  private/ToolZ/HitSorting_FirstHit.cxx
  private/ToolZ/HitFacility_FirstHit.cxx
  private/ToolZ/I3RUsageTimer.cxx
  private/ToolZ/IndexMatrix.cxx
  private/ToolZ/PositionService.cxx
  private/ToolZ/DistanceService.cxx
  private/ToolZ/HashedGeometry.cxx
  private/ToolZ/OMTopology.cxx
  private/ToolZ/IC86Topology.cxx
  
  private/ToolZ/PartialCOG.cxx
  
  private/ToolZ/ResponseMapHelpers.cxx
  private/ToolZ/TriggerHierarchyHelpers.cxx
  private/ToolZ/MiscHelpers.cxx
  
  private/ToolZ/Interfaces.cxx

  private/ToolZ/FrameRegister.cxx
  private/ToolZ/I3RegisterModule.cxx
)

SET(LIB_${PROJECT_NAME}_PYBINDINGS
  private/pybindings/module.cxx
 
  private/pybindings/OMKeyHash.cxx
  private/pybindings/Hitclasses.cxx
  private/pybindings/HitSorting.cxx
  private/pybindings/HitFacility.cxx
  private/pybindings/PositionService.cxx
  private/pybindings/DistanceService.cxx
  private/pybindings/HashedGeometry.cxx
  private/pybindings/OMTopology.cxx
  private/pybindings/IC86Topology.cxx
  
  private/pybindings/ResponseMapHelpers.cxx
  private/pybindings/TriggerHierarchyHelpers.cxx
  private/pybindings/MiscHelpers.cxx
)

SET(${PROJECT_NAME}_UNITTESTS
  private/test/main.cxx
  private/test/TestHelpers.cxx
  
  private/test/I3RUsageTimerTest.cxx
  private/test/OMKeyHashTest.cxx
  private/test/HitclassesTest.cxx
  private/test/HitSortingTest.cxx
  private/test/HitFacilityTest.cxx
  private/test/OMTopologyTest.cxx
  private/test/IndexMatrixTest.cxx
  private/test/PositionServiceTest.cxx
  private/test/DistanceServiceTest.cxx
  private/test/HashedGeometryTest.cxx
  
  private/test/ResponseMapHelpersTest.cxx
  private/test/TriggerHierarchyHelpersTest.cxx
  private/test/MiscHelpersTest.cxx
  
  private/test/HitSorting_FirstHitTest.cxx
  private/test/PartialCOGTest.cxx
  private/test/FrameRegisterTest.cxx
)

SET(${PROJECT_NAME}_USEPROJECTS
  icetray
  dataclasses
  simclasses
  phys-services
  dataio
  serialization
)

if(SERIALIZATION_SUPPORT)
  LIST(APPEND ${PROJECT_NAME}_UNITTESTS
    private/test/SerializationTest.cxx
  )
endif()


SET(${PROJECT_NAME}_PREPROCESSORFLAGS
  -DSERIALIZATION_SUPPORT=${SERIALIZATION_SUPPORT}
  -DFUTURE=0
  -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
)

SET(${PROJECT_NAME}_COMPILERINST
 -ftemplate-depth=512
 -std=c++11
) 

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  LIST(APPEND ${PROJECT_NAME}_COMPILERINST
    -std=gnu++11
  )
endif()

add_definitions(${${PROJECT_NAME}_PREPROCESSORFLAGS} ${${PROJECT_NAME}_COMPILERINST} )

i3_add_library(${PROJECT_NAME}
  ${LIB_${PROJECT_NAME}_HEADERFILES}
  ${LIB_${PROJECT_NAME}_SOURCEFILES}
  USE_TOOLS boost python
  USE_PROJECTS ${${PROJECT_NAME}_USEPROJECTS}
)

i3_add_pybindings(${PROJECT_NAME}
  ${LIB_${PROJECT_NAME}_PYBINDINGS}
  USE_TOOLS boost python 
  USE_PROJECTS ${PROJECT_NAME} ${${PROJECT_NAME}_USEPROJECTS}
)

i3_test_executable(test
  ${${PROJECT_NAME}_UNITTESTS}
  USE_PROJECTS ${PROJECT_NAME} ${${PROJECT_NAME}_USEPROJECTS}
)

i3_test_scripts(
  resources/test/servicesTest.py
)
