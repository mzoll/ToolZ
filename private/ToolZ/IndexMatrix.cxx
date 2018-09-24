/**
 * \file IndexMatrix.cxx
 *
 * copyright (c) 2011
 * the IceCube Collaboration
 * $Id: IceHiveHelpers.cxx 144968 2016-04-20 22:52:24Z mzoll $
 *
 * @date $Date: 2013-10-22$
 * @author mzoll <marcel.zoll@fysik.su.se>
 *
 */

#include "ToolZ/IndexMatrix.h"
  
// #if SERIALIZATION_ENABLED
//   typedef boost::dynamic_bitset<> DynamicBitSet;
//   I3_SPLIT_SERIALIZABLE(DynamicBitSet);
// #endif //SERIALIZATION_ENABLED

//================== CLASS  (A)SymmetricIndexMatrix_Double ==================

indexmatrix::AsymmetricIndexMatrix_Double::AsymmetricIndexMatrix_Double (const unsigned biSize) :
  AsymmetricIndexMatrix<double, std::vector<double> > (biSize) 
{
  BOOST_FOREACH(double& field, internal_)
    field = NAN;
};

indexmatrix::SymmetricIndexMatrix_Double::SymmetricIndexMatrix_Double (const unsigned biSize) :
  SymmetricIndexMatrix<double, std::vector<double> > (biSize) 
{
  BOOST_FOREACH(double& field, internal_)
    field = NAN;
};


//================== CLASS  (A)SymmetricIndexMatrix_Bool ==================

indexmatrix::SymmetricIndexMatrix_Bool::SymmetricIndexMatrix_Bool (
  const unsigned biSize,
  const bool setall) 
:
  SymmetricIndexMatrix<bool, boost::dynamic_bitset<> >(biSize)  
{
  if (setall) {
    for (boost::dynamic_bitset<>::size_type i=0; i<internal_.size(); ++i)
      internal_[i]=1;
  }
};

indexmatrix::SymmetricIndexMatrix_Bool::SymmetricIndexMatrix_Bool (
  const unsigned biSize,
  const boost::function<bool (const uint64_t, const uint64_t)> predicate)
: SymmetricIndexMatrix<bool, boost::dynamic_bitset<> >(biSize)
{
  for (boost::dynamic_bitset<>::size_type index=0; index<internal_.size(); ++index) {
    std::pair<unsigned, unsigned> one_two = UniIndex_To_BiIndex(index); 
    //log_debug_stream("EVAL "<<index<<" with "<<one_two.first<<","<<one_two.second<<" as "<<predicate(one_two.first, one_two.second));
    internal_[index] = predicate(one_two.first, one_two.second);
  }
};

indexmatrix::SymmetricIndexMatrix_Bool&
indexmatrix::SymmetricIndexMatrix_Bool::operator|= (const SymmetricIndexMatrix_Bool& rhs) {
  assert (rhs.biSize_ == biSize_);
  this->internal_ |= rhs.internal_;
  return *this;
}

indexmatrix::SymmetricIndexMatrix_Bool&
indexmatrix::SymmetricIndexMatrix_Bool::operator&= (const SymmetricIndexMatrix_Bool& rhs) {
  assert (rhs.biSize_ == biSize_);
  this->internal_ &= rhs.internal_;
  return *this;
}

indexmatrix::AsymmetricIndexMatrix_Bool::AsymmetricIndexMatrix_Bool (
  const unsigned biSize,
  const bool setall) 
: AsymmetricIndexMatrix<bool, boost::dynamic_bitset<> >(biSize)
{
  if (setall) {
    for (boost::dynamic_bitset<>::size_type i=0; i<internal_.size(); ++i)
      internal_[i]=1;
  }
};

indexmatrix::AsymmetricIndexMatrix_Bool::AsymmetricIndexMatrix_Bool (
  const unsigned biSize,
  const boost::function<bool (const uint64_t, const uint64_t)> predicate)
: AsymmetricIndexMatrix<bool, boost::dynamic_bitset<> >(biSize)
{
  for (boost::dynamic_bitset<>::size_type index=0; index<internal_.size(); ++index) {
    std::pair<uint64_t, uint64_t> one_two = UniIndex_To_BiIndex(index);
    //log_debug_stream("EVAL "<<index<<" with "<<one_two.first<<","<<one_two.second<<" as "<<predicate(one_two.first, one_two.second));
    internal_[index] = predicate(one_two.first, one_two.second);
  }
};

indexmatrix::AsymmetricIndexMatrix_Bool&
indexmatrix::AsymmetricIndexMatrix_Bool::operator|= (const AsymmetricIndexMatrix_Bool& rhs) {
  assert (rhs.biSize_ == biSize_);
  this->internal_ |= rhs.internal_;
  return *this;
}

indexmatrix::AsymmetricIndexMatrix_Bool&
indexmatrix::AsymmetricIndexMatrix_Bool::operator&= (const AsymmetricIndexMatrix_Bool& rhs) {
  assert (rhs.biSize_ == biSize_);
  this->internal_ &= rhs.internal_;
  return *this;
}    

#if SERIALIZATION_ENABLED
  I3_SERIALIZABLE(indexmatrix::AsymmetricIndexMatrix_Double);
  I3_SERIALIZABLE(indexmatrix::SymmetricIndexMatrix_Double);
  I3_SERIALIZABLE(indexmatrix::AsymmetricIndexMatrix_Bool);
  I3_SERIALIZABLE(indexmatrix::SymmetricIndexMatrix_Bool);
#endif
