/**
 * \file OMKeyHash.cxx
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: OMKeyHash.cxx 124115 2014-10-03 16:42:30Z mzoll $
 * \version $Revision: 124115 $
 * \date $Date: 2014-10-03 18:42:30 +0200 (fre, 03 okt 2014) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 */

#include "ToolZ/OMKeyHash.h"

#include <boost/foreach.hpp>

#include "ToolZ/SetHelpers.h"

std::set<OMKey>
ExtractOMKeys(
  const I3GeometryConstPtr& geo) 
{
  std::set<OMKey> omkey_set;
  BOOST_FOREACH(const I3OMGeoMap::value_type &omkeygeo, geo->omgeo) {
    omkey_set.insert(omkey_set.end(),omkeygeo.first);
  }
  return omkey_set;
};

//============== CLASS CompactOMKeyHashService ===============

CompactOMKeyHashService::CompactOMKeyHashService() {};

bool CompactOMKeyHashService::VerifyAgainst(const std::set<OMKey>& omkeys) const {
  BOOST_FOREACH(const OMKey& omkey, omkeys) {
    if (! hashmap_.count(omkey)) {
      log_error("Provided OMKey set contains more DOMs than this hasher is currently encoding"); 
      return false;
    }
  }
  log_debug_stream("sets are congruent");
  return true;
};
  
CompactOMKeyHashService::CompactOMKeyHashService(const std::set<OMKey> omkeys)
: omkeyvec_(omkeys.begin(), omkeys.end()),
  hashmap_(HashMap_From_OMKeyVec(omkeyvec_))
{};

std::map<OMKey, CompactHash>
CompactOMKeyHashService::HashMap_From_OMKeyVec(const std::vector<OMKey>& omkeyvec) {
  std::map<OMKey, CompactHash> hashmap;
  CompactHash hash_iter = 0;
  BOOST_FOREACH(const OMKey &omkey, omkeyvec) {
    hashmap.insert(std::make_pair(omkeyvec.at(hash_iter), hash_iter));
    hash_iter++;
  }
  return hashmap;
};

#if SERIALIZATION_ENABLED
  I3_SERIALIZABLE(CompactOMKeyHashService);
#endif

