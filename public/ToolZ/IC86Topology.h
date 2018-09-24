/**
 * \file IC86Topology.h
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: IC86Topology.h 153730 2017-03-06 12:06:51Z mzoll $
 * \version $Revision: 153730 $
 * \date $Date: 2017-03-06 13:06:51 +0100 (Mon, 06 Mar 2017) $
 * \author mzoll <marcel.zoll@icecube.wisc.edu>
 *
 * Some topological functions useful for the IC86 detector configuration
 */

#ifndef IC86TOPOLOGY_H
#define IC86TOPOLOGY_H

static const unsigned ic86topology_version_ = 0;

#include "icetray/OMKey.h"

#include "dataclasses/geometry/I3Geometry.h"

#include "ToolZ/OMTopology.h"

///Some topological functions useful for the IC86 detector configuration
struct IC86Topology{
  /// Is this DOM in IceTop?
  static
  bool IsIceTop(const OMKey& omkey);
  
  /// Is this DOM in the Ice?
  static
  bool IsInIce(const OMKey& omkey);
  
  /// Is this a DOM in the regular IceCube array configuration?
  static
  bool IsIceCube(const OMKey& omkey);
  
  /// Is this a DOM in the DeepCore array configuration?
  static
  bool IsDeepCore(const OMKey& omkey);
  
  /// Is this a DOM in the bottom part of the DeepCore array configuration?
  static
  bool IsDeepCoreBottom(const OMKey& omkey);
  
  /// Is this DOM in the top part of the DeepCore array configuration?
  static
  bool IsDeepCoreTop(const OMKey& omkey);
  
  /// Is this DOM in the denser populated DeepCore region?
  static
  bool IsDeepCoreFidutial(const OMKey& omkey);
  
  /// Is this DOM in the denser populated DeepCore-Veto region?
  static
  bool IsDeepCoreCap(const OMKey& omkey);
  
  /// Is this DOM in any dense populated region?
  static
  bool IsDeepCoreDense(const OMKey& omkey);
  
  /** Generate a TestGeometry
      construct an artificially ideal IceCube geometry, that is:
    1. exactly centered at string 36
    2. the major axis of the hexagonal grid follows the x-axis
    3. all strings are at their exact position in the hexagonal structure
    4. The charcteristic inter string spacing is 125 meters
    5. the detector center is 1948 meter below the ice-surface
    6. OM30 on regular IC-strings rests in the detector XY plane
    7. DOMs are spaced 17m in IC, 10/7m in DC
    8. IceTop tanks are located exactly on top of the strings at the surface
    */
  static
  I3Geometry Build_IC86_Geometry();
  
  /// Build a OMTopology for IC86
  static
  OMTopologyMap Build_IC86_OMKeyTopologyMap();
};

#endif //IC86TOPOLOGY_H
