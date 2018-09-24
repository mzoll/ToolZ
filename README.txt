This projects contains a bunch of libraries for common use in combination with icetray C++-projects. The libraries are all of general purpose and can be used for prototyping and convenience.
It follows a short description of the main components:

In IceCube there is a fixed, so limited number of OMKeys in the detector. It is so possible to assign each of them a Hash value, so that OMKeys can be represented simpler and more compact, especially to information-technical purposes. As this hash can be constructed progressively this opens up the possibility to identify simple positive value indexes with hashed OMKeys and so realize a simple one-to-one mapping without the need to go through Hash tables

* OMKeyHash: implements OMKeyHashes and their forth and back conversion through the CompactOMKeyHashService

Many applications require two dimensional fields for storing and accessing data in the comparison of similar objects. By construction these matrices are 2-dimensional and square, additionally they can be symmetrical. Especially the last property grants a large reduction of redundant information. Also the normal representation of this as 2dim arrays might sometimes go over the top, as the internally used container for each entry might be bigger than the essential information to stored, e.g. arrays of bools. Index matrices circumvent this problem by allowing to specify an internal storage container and outer representation of the entries, which then can be internally converted. They are therefore much more compact while their mode of access is much faster. Their use will especially become apparent for the implementation of Bool-matrices from bitsets.

* IndexMatrix: Implements 2d square matrices, symmetrical as well as asymmetrical, their conversions; special types implemented for Double and Bool matrices

The I3Geometry-object is often used only to retrieve Position information of the DOMs through a mapping of the OMKeys. This operation is slow as every retrieval requires a new map look-up, which scales by order log(n). However, once a OMKey is hashed one can construct and store DOM-positions in a strictly ordered progressive container, which can be directly accessed by the hash-value itself, making the access scale by order 1. This is of special interest in hindsight to the Hitclasses library, a bit further down.

* PositionService: stores the I3Position data from the I3Geometry and grants fast access by CompactOMKeyHashes 

A common task in the comparison of any two DOMs is computing their distance. As this is a repeatative task in longer calculations scaling with the number of to comparing objects squared, it makes sense to store the computed result for repeated later retrieval. This is of special interest in hindsight to the Hitclasses library, a bit further down.

* DistanceService: stores distances (accurate to 0.1m) between DOMs and grants access by CompactOMKeyHashes

The data in IceCube is in principle a progessive stream of hits, represented as MCPulses, RecoPulses or similar on individual DOMs.However, because of the frame<->event correspondence hits might are stored in mapped container formats, like I3RecoPulseSeriesMaps, which so neglect a progressive chronological order of Hits for the benefit of storage size by omitting redundant information. The Hit libraries help with the facilitation of reverting this process and make the data lightweight to handle, manipulate and work with.

* Hitclasses: classes for lightweight representation of Hits
  implements classes AbsHit, AbsDAQHit, Hit, HitObject, HitObjectOriginal
  
* HitSorting: functions for extracting Hitclasses from OMKeyMap-types

* HitFacility: facilitates extraction of Hits from OMKeyMap-types stored in the frame and the reverse process

* HitSorting_FirstHit: same as HitSorting, but extracts the first hits for each OMKey only, partially accumulating properties e.g. charge

* HitFacility_FirstHit: same as HitFacility, but extracting the first hit only, partially accumulating properties e.g. charge

From the series of Hits as they ocure in the Detector much information can be extracted by very simplistic indicators, like for example the Center of Gravity (CoG), the average of all time and position coordinates. However the series of hits given by some physical process is often not smooth nor equally distributed, so that hits towards the end of an event are of lesser value, as increasingly more of the arriving photons have scattered at least once; this can lead to negative pulls in the distribution of otherwise direct photons. This can be counteracted by looking at only certain fractions (quantiles) of these pulses which sit at the core of the distribution. Looking also only at certain fraction of pulses the COG of only that fraction can be derived, which effectively leads to a closer reconstruction of the particle at just that moment of time, e.g. is the movement of a track easily sampled by computing the COG of all its quantiles.

* PartialCOG: facilitates the extraction of quantiles of a series of Hits and the computation of Center of Gravity (COG), weighting is supported

Some analysis and module application require the comparison of information of the same event in different data-streams, e.g. a coincidental study of tracks reconstructed in IceTop and IceCube, where both reconstructions are stored in different subevent-streams. As all data is stored in a QPP-frame stream, where each P-frame can have different subevent-stream, a method of accessing them in meaningful order is mandatory.

* FrameRegister: Buffers up a QPP-sequence of frames, granting random access to each of them

* I3RegisterModule: a I3Module that grants random access to a buffered up sequence of frames through an internal FrameRegister

Many applications require knowledge of the time consumption of the one or the other internal call within modules. I3RUsage is the prevailing way to store and represent such information, which can be used to optimize once algorithm or estimate its performance and resource consumption.

* I3RUsage: a stop-watch bulletin

OMs most often belong to more topological than simply IceCube, so for example DeepCore and DeepCore fiducial are sub-regions including a number of DOMs. OMtopology provides a method to designate DOMs as to belong to number of different predefined topological regions. These selections can be modified and expanded. also all DOMs of a certain region can be easily selected. This implementation is far more flexible than the static function implementations like IsIceCube() or IsIceTop().

* OMTopology: store properties per DOM in a compact way

With the setup with 86 strings the IceCube detector in its DeepCore adapted design is static and final. This effectively allows therefore static idealized definitions of detector OMTopologies and Geometries. Using an ideal Geometry can for example help in module and algorithm performance diagnostics.

* IC86Topology: provides an idealized IC86 I3Geometry and OMTopology(Map)