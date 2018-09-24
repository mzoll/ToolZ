/**
 * \file Dummy.cxx
 *
 * (c) 2013 the IceCube Collaboration
 *
 * $Id: OMKeyHashTest.cxx 151300 2016-11-03 15:27:21Z mzoll $
 * \version $Revision: 151300 $
 * \date $Date: 2016-11-03 16:27:21 +0100 (tor, 03 nov 2016) $
 * \author mzoll <marcel.zoll@fysik.su.se>
 *
 * Unit test to test the robustness of OMKey Hashing as it is implemented in IceHive/OMKeyHash.h
 */

#include <I3Test.h>

#include "icetray/OMKey.h"

#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include "icetray/I3Logging.h"
  
#include "ToolZ/__SERIALIZATION.h"  

//   #define SERIALIZATION_CLASS_VERSION(c, v) I3_CLASS_VERSION( c , v );
//   #define SERIALIZATION_BASE_OBJECT_NVP(c) I3_SERIALIZATION_BASE_OBJECT_NVP( c );
//   #define SERIALIZATION_NVP(c) I3_SERIALIZATION_NVP( c );
//   #define SERIALIZATION_ASSUME_ABSTRACT(c) I3_SERIALIZATION_ASSUME_ABSTRACT( c )

using namespace std;

TEST_GROUP(Dummy);

template <class T>
void serialize_object(
  const T* t_save,
  T*& t_load)
{
  std::stringstream ss;
  SERIALIZATION_NS_BASE::archive::portable_binary_oarchive oa(ss);
//   SERIALIZATION_NS_BASE::archive::xml_oarchive oa(ss);
  oa << t_save;
  SERIALIZATION_NS_BASE::archive::portable_binary_iarchive ia(ss);
//   SERIALIZATION_NS_BASE::archive::xml_iarchive ia(ss);
  ia >> t_load;
};

template <class T>
void serialize_object(
  const boost::shared_ptr<T> t_save,
  boost::shared_ptr<T>& t_load)
{
  std::stringstream ss;
  SERIALIZATION_NS_BASE::archive::portable_binary_oarchive oa(ss);
//   SERIALIZATION_NS_BASE::archive::xml_oarchive oa(ss);
  oa << t_save;
  SERIALIZATION_NS_BASE::archive::portable_binary_iarchive ia(ss);
//   SERIALIZATION_NS_BASE::archive::xml_iarchive ia(ss);
  ia >> t_load;
};



class BaseWrapper {
public:
  const OMKey omkey_;
public:
  BaseWrapper(const OMKey o) : omkey_(o) {};
  
  void Report() {cout<< omkey_<<endl;};
private:
  friend class SERIALIZATION_NS::access;
    
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const BaseWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, BaseWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {log_debug_stream("BaseWrapper serialize/deserialize from/to address"<<this);};
};

typedef boost::shared_ptr<BaseWrapper> BaseWrapperPtr;

I3_SERIALIZABLE(BaseWrapper);

namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const BaseWrapper * t, const unsigned int file_version)
  {
    log_debug_stream("BaseWrapper save from adress "<<t);
    ar << SERIALIZATION_NS::make_nvp("omkey", t->omkey_);
  };
  
  template<class Archive>
  inline void load_construct_data(
      Archive & ar, BaseWrapper* t, const unsigned int file_version)
  {
    log_debug_stream("BaseWrapper load to address "<<t);
    OMKey omkey;
    ar >> SERIALIZATION_NS::make_nvp("omkey", omkey);
    ::new(t)BaseWrapper(omkey);
  };
}}; // namespace ...

//================= sharedPtrWrapper ===============
class SharedPtrWrapper {
public:
  const boost::shared_ptr<BaseWrapper> held_;
public:
  SharedPtrWrapper(const BaseWrapper& bw) : held_(boost::make_shared<BaseWrapper>(bw)) {};
private:
  friend class SERIALIZATION_NS::access;
    
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const SharedPtrWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, SharedPtrWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {log_debug_stream("SharedPtrWrapper serialize/deserialize from/to address"<<this<<" == subordinate held_: "<<this->held_);};  
};

typedef boost::shared_ptr<SharedPtrWrapper> SharedWrapperPtr;



namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const SharedPtrWrapper * t, const unsigned int file_version)
  {
    log_debug_stream("SharedPtrWrapper save from adress "<<t<<" == subordinate held_: "<<t->held_);
    ar << SERIALIZATION_NS::make_nvp("held", t->held_);
  };
  
  template<class Archive>
  inline void load_construct_data(
      Archive & ar, SharedPtrWrapper* t, const unsigned int file_version)
  {
    log_debug_stream("SharedPtrWrapper load to address "<<t<<" == subordinate held_: "<<t->held_<<" ptrto: "<<&(*(t->held_)));
    BaseWrapperPtr held;
    ar >> SERIALIZATION_NS::make_nvp("held", held);
    ::new(t)SharedPtrWrapper(*held);
  };
}}; // namespace ...

I3_SERIALIZABLE(SharedPtrWrapper);

//================= RawPtrWrapper ===============
class RawPtrWrapper {
public:
  const BaseWrapper* held_;
public:
  RawPtrWrapper(const BaseWrapper& bw) : held_(&bw ) {};
private:
  friend class SERIALIZATION_NS::access;
    
  template<class Archive>
  friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const RawPtrWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, RawPtrWrapper * t, const unsigned int file_version);
  
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {log_debug_stream("RawPtrWrapper serialize/deserialize from/to address"<<this<<" == subordinate held_: "<<this->held_);};  
};

typedef boost::shared_ptr<RawPtrWrapper> RawWrapperPtr;


namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const RawPtrWrapper * t, const unsigned int file_version)
  {
    log_debug_stream("RawPtrWrapper save from adress "<<t<<" == subordinate held_: "<<t->held_);
    ar << SERIALIZATION_NS::make_nvp("held", t->held_);
  };
  
  template<class Archive>
  inline void load_construct_data(
      Archive & ar, RawPtrWrapper* t, const unsigned int file_version)
  {
    log_debug_stream("RawPtrWrapper load to address "<<t<<" == subordinate held_: "<<t->held_);
    BaseWrapper* held;
    ar >> SERIALIZATION_NS::make_nvp("held", held);
    ::new(t)RawPtrWrapper(*held);
  };
}}; // namespace ...

I3_SERIALIZABLE(RawPtrWrapper);



//============== TESTS
TEST(basewrapper_rawptr) {
  BaseWrapper* bw_save = new BaseWrapper(OMKey(1,2));
  BaseWrapper* bw_load = nullptr; //(BaseWrapper*)malloc(sizeof(BaseWrapper));
  
  log_debug_stream("Give "<<bw_save->omkey_);
  serialize_object(bw_save, bw_load);
  log_debug_stream("Have "<<bw_load->omkey_);
  ENSURE_EQUAL(bw_save->omkey_, bw_load->omkey_);
  
  delete bw_save;
  delete bw_load;
};

TEST(basewrapper_sharedptr) {
  BaseWrapperPtr bw_save = boost::make_shared<BaseWrapper>(OMKey(1,2));
  BaseWrapperPtr bw_load;
  
  log_debug_stream("Give "<<bw_save->omkey_);
  serialize_object(bw_save, bw_load);
  log_debug_stream("Have "<<bw_load->omkey_);
  ENSURE_EQUAL(bw_save->omkey_, bw_load->omkey_);
};


TEST(sharedptrwrapper) {
  SharedPtrWrapper* spw_save = new SharedPtrWrapper(BaseWrapper(OMKey(1,2)));
  SharedPtrWrapper* spw_load = nullptr; //(SharedPtrWrapper*)malloc(sizeof(SharedPtrWrapper));
  
  log_debug_stream("Give "<<spw_save->held_->omkey_);
  serialize_object(spw_save, spw_load);
  log_debug_stream("Have "<<spw_load->held_->omkey_);
  ENSURE_EQUAL(spw_save->held_->omkey_,spw_load->held_->omkey_);
  
  delete spw_save;
  delete spw_load;
};


TEST(rawptrwrapper) {
  RawPtrWrapper* rpw_save = new RawPtrWrapper(BaseWrapper(OMKey(1,2)));
  RawPtrWrapper* rpw_load = nullptr; //(RawPtrWrapper*)malloc(sizeof(RawPtrWrapper));
  
  log_debug_stream("Give "<<rpw_save->held_->omkey_);
  serialize_object(rpw_save, rpw_load);
  log_debug_stream("Have "<<rpw_load->held_->omkey_);
  ENSURE_EQUAL(rpw_save->held_->omkey_, rpw_load->held_->omkey_);
  
  delete rpw_save;
  delete rpw_load;
};
