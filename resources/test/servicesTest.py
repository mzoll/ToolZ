#!/usr/bin/env python 

"""
Testing the all modules of ToolZ
"""

from icecube import icetray, dataclasses, dataio
from icecube import ToolZ

#get a dummy IC86Geometry
geo = ToolZ.IC86_Geometry()
omkeys = geo.omgeo.keys()

#hash this geometry
hashedGeo = HashedGeometry(geo.omgeo)
#probe access
omkey0 = hashedGeo.hashService.OMKeyFromHash(0)
omkey1 = hashedGeo.hashService.OMKeyFromHash(1)
pos0 = hashedGeo.posService.GetPosition(0)
dist01 = hashedGeo.distService(0,1)