/**
 * \file Interfaces.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: MapService.h 99900 2013-02-26 10:10:43Z mzoll $
 * \version $Revision: 99900 $
 * \date $Date: 2013-02-26 11:10:43 +0100 (Tue, 26 Feb 2013) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 * 
 * Facilitates Serialization of objects to Files, Write and Read operations
 */

#ifndef INTERFACES_H
#define INTERFACES_H

#include "icetray/I3Logging.h"
#include "ToolZ/__SERIALIZATION.h"

#include <fstream>

namespace fileinterfaces {
  // all objects are taken by pointer-reference; while not needed for serialization,
  // the deserialization need this way, as object-classes might not provide default constructors
  #if SERIALIZATION_ENABLED
  ///general writing of single object to file
  ///reading of single object from file using a reference;
  /// \param T pointer to a to save/serialize object
  /// \param filename name of the file to write to
  template<class T>
  void WriteToFile(
    const T* t,
    const std::string& filename);

  ///reading of single object from file using a reference;
  /// \param T pointer to a to restore/deserialize object
  /// \param filename name of the file to write to
  template<class T>
  void ReadFromFile(
    T*& t,
    const std::string& filename);
  #endif //SERIALIZATION_ENABLED
}; //fileinterfaces
  
//============================================================
//=========================== IMPLEMETATIONS =================  
//============================================================
  
#if SERIALIZATION_ENABLED
template<class T>
void fileinterfaces::WriteToFile(
  const T* t,
  const std::string& filename)
{
  log_info_stream("Writing to file: "<<filename);

  std::ofstream ofs(filename.c_str());
  SERIALIZATION_NS_BASE::archive::portable_binary_oarchive oa(ofs);
  oa << t;
  ofs.close();
};

template<class T>
void fileinterfaces::ReadFromFile(
  T*& t,
  const std::string& filename)
{
  log_info_stream("Reading from file: "<<filename);
  
  std::ifstream ifs(filename.c_str());
  if (!ifs.is_open())
    log_fatal_stream(filename.c_str()<< " not found");
  SERIALIZATION_NS_BASE::archive::portable_binary_iarchive ia(ifs);
  
  ia >> t;
  ifs.close();
};
#endif //SERIALIZATION_ENABLED

#endif //INTERFACES_H
