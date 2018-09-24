/**
 * \file IndexMatrixTest.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Test for two dimensional matrices
 */

#include <I3Test.h>

#include "ToolZ/IndexMatrix.h"

#include <boost/make_shared.hpp>

#include "TestHelpers.h"

using namespace indexmatrix;
using namespace std;

TEST_GROUP(IndexMatrix);

TEST (SymmetricIndexMatrix_Bool_SetAndGet) {
  SymmetricIndexMatrix_Bool b(2);
  
  ENSURE(! b.Get(0,0), "Inital state is correctly False");
  
  ENSURE(! b.Get(1,1), "the maximum index is accessible as requested");
  
  b.Set(0,0,true);
  b.Set(1,0,true);
  b.Set(1,1,true);
  ENSURE(b.Get(0,0), "Setting/Getting works correctly");
  ENSURE(b.Get(1,0), "Setting/Getting works correctly"); //its symmetric, remember
  ENSURE(b.Get(0,1), "Setting/Getting works correctly"); //its symmetric, remember
  ENSURE(b.Get(1,1), "Setting/Getting works correctly");
};

TEST (AsymmetricIndexMatrix_Bool_SetAndGet) {
  AsymmetricIndexMatrix_Bool b(2);
  
  ENSURE(! b.Get(0,0), "Inital state is correctly False");
  
  ENSURE(! b.Get(1,1), "the maximum index is accessible as requested");
  
  b.Set(0,0,true);
  b.Set(1,0,true);
  b.Set(1,1,true);
  ENSURE(b.Get(0,0), "Setting/Getting works correctly");
  ENSURE(b.Get(1,0), "Setting/Getting works correctly");
  ENSURE(!b.Get(0,1), "Setting/Getting works correctly");
  ENSURE(b.Get(1,1), "Setting/Getting works correctly");
};

TEST (AsymmetricIndexMatrix_Bool_Predicate) {
  static const size_t size_limit= 4;
    //make a struct that evaluates true if both OMKeys are even OM numbers
  struct SetEven {
    bool operator() (const uint64_t index_A, const uint64_t index_B) const 
      {return (!(index_A%2) && !(index_B%2));};
  };
  SetEven se;

  AsymmetricIndexMatrix_Bool aimb(size_limit, se);
  
  for (uint64_t index_A=0; index_A<size_limit; index_A++) {
    for (uint64_t index_B=0; index_B<size_limit; index_B++) {
      ENSURE_EQUAL(aimb.Get(index_A, index_B), se(index_A, index_B));
    }
  }
};

TEST (SymmetricIndexMatrix_Bool_Predicate) {
  static const size_t size_limit= 4;
    //make a struct that evaluates true if both OMKeys are even OM numbers
  struct SetEven {
    bool operator() (const uint64_t index_A, const uint64_t index_B) const 
      {return (!(index_A%2) && !(index_B%2));};
  };
  SetEven se;
  
  SymmetricIndexMatrix_Bool simb(size_limit, se);
  
  for (uint64_t index_A=0; index_A<size_limit; index_A++) {
    for (uint64_t index_B=0; index_B<size_limit; index_B++) {
      ENSURE_EQUAL(simb.Get(index_A, index_B), se(index_A, index_B));
    }
  }
};

TEST (SymmetricIndexMatrix_Double_SetAndGet) {
  SymmetricIndexMatrix_Double d(2);
  
  ENSURE(isnan(d.Get(0,0)), "Inital state is correctly False");
  
  ENSURE(isnan(d.Get(1,1)), "the maximum index is accessible as requested");
  
  d.Set(0,0, 1.);
  d.Set(1,1, 2.);
  d.Set(0,1, 3.);
  d.Set(1,0, 4.);
  ENSURE_EQUAL(d.Get(0,0), 1., "Setting/Getting works correctly");
  ENSURE_EQUAL(d.Get(1,1), 2., "Setting/Getting works correctly");
  ENSURE_EQUAL(d.Get(0,1), 4., "Setting/Getting works correctly"); //its symmetric, remember
  ENSURE_EQUAL(d.Get(1,0), 4., "Setting/Getting works correctly"); //its symmetric, remember
};

TEST (AsymmetricIndexMatrix_Double_SetAndGet) {
  AsymmetricIndexMatrix_Double d(2);
  
  ENSURE(isnan(d.Get(0,0)), "Inital state is correctly False");
  
  ENSURE(isnan(d.Get(1,1)), "the maximum index is accessible as requested");
  
  d.Set(0,0, 1.);
  d.Set(1,1, 2.);
  d.Set(0,1, 3.);
  d.Set(1,0, 4.);
  ENSURE_EQUAL(d.Get(0,0), 1., "Setting/Getting works correctly");
  ENSURE_EQUAL(d.Get(1,1), 2., "Setting/Getting works correctly");
  ENSURE_EQUAL(d.Get(0,1), 3., "Setting/Getting works correctly");
  ENSURE_EQUAL(d.Get(1,0), 4., "Setting/Getting works correctly");
};


#if SERIALIZATION_ENABLED
static size_t maxSize = 100;

TEST(AsymmetricIndexMatrix_Double_Serialize_raw_ptr){
  AsymmetricIndexMatrix_Double* AIMD_save = new AsymmetricIndexMatrix_Double(maxSize);
  AsymmetricIndexMatrix_Double* AIMD_load = nullptr;
  
  serialize_object(AIMD_save, AIMD_load);
  delete AIMD_save;
  delete AIMD_load;
};
  
TEST(SymmetricIndexMatrix_Double_Serialize_raw_ptr){
  SymmetricIndexMatrix_Double* SIMD_save= new SymmetricIndexMatrix_Double(maxSize);
  SymmetricIndexMatrix_Double* SIMD_load = nullptr;
  
  serialize_object(SIMD_save, SIMD_load);
  delete SIMD_save;
  delete SIMD_load;
};

TEST(AsymmetricIndexMatrix_Bool_Serialize_raw_ptr){  
  AsymmetricIndexMatrix_Bool* AIMB_save= new AsymmetricIndexMatrix_Bool(maxSize);
  AsymmetricIndexMatrix_Bool* AIMB_load = nullptr;
  
  serialize_object(AIMB_save, AIMB_load);
  delete AIMB_save;
  delete AIMB_load;
};

TEST(SymmetricIndexMatrix_Bool_Serialize_raw_ptr){
  SymmetricIndexMatrix_Bool* SIMB_save= new SymmetricIndexMatrix_Bool(maxSize);
  SymmetricIndexMatrix_Bool* SIMB_load = nullptr;
  
  serialize_object(SIMB_save, SIMB_load);
  delete SIMB_save;
  delete SIMB_load;
};

TEST(AsymmetricIndexMatrix_Double_Serialize_boost_shared_ptr){
  AsymmetricIndexMatrix_DoublePtr AIMD_save= boost::make_shared<AsymmetricIndexMatrix_Double>(maxSize);
  AsymmetricIndexMatrix_DoublePtr AIMD_load;
  
  serialize_object(AIMD_save, AIMD_load);
};

TEST(SymmetricIndexMatrix_Double_Serialize_boost_shared_ptr){  
  SymmetricIndexMatrix_DoublePtr SIMD_save= boost::make_shared<SymmetricIndexMatrix_Double>(maxSize);
  SymmetricIndexMatrix_DoublePtr SIMD_load;
  
  serialize_object(SIMD_save, SIMD_load);
};

TEST(AsymmetricIndexMatrix_Bool_Serialize_boost_shared_ptr){  
  AsymmetricIndexMatrix_BoolPtr AIMB_save= boost::make_shared<AsymmetricIndexMatrix_Bool>(maxSize);
  AsymmetricIndexMatrix_BoolPtr AIMB_load;
  
  serialize_object(AIMB_save, AIMB_load);
};

TEST(SymmetricIndexMatrix_Bool_Serialize_boost_shared_ptr){    
  SymmetricIndexMatrix_BoolPtr SIMB_save = boost::make_shared<SymmetricIndexMatrix_Bool>(maxSize);
  SymmetricIndexMatrix_BoolPtr SIMB_load;
  
  serialize_object(SIMB_save, SIMB_load);
};
#endif //SERIALIZATION_ENABLED
