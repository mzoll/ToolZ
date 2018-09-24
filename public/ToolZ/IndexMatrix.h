/**
 * \file IndexMatrix.h
 *
 * (c) 2012 the IceCube Collaboration
 *
 * $Id: IndexMatrix.h 144968 2016-04-20 22:52:24Z mzoll $
 * \version $Revision: 144968 $
 * \date $Date: 2016-04-21 00:52:24 +0200 (tor, 21 apr 2016) $
 * \author Marcel Zoll <marcel.zoll@fysik.su.se>
 *
 * Matrices which are 2 dimensional
 */

#ifndef INDEXMATRIX_H
#define INDEXMATRIX_H


#include "icetray/OMKey.h"
#include "dataclasses/I3Map.h"
#include "dataclasses/I3TimeWindow.h"
#include "dataclasses/physics/I3TriggerHierarchy.h"
#include "dataclasses/geometry/I3Geometry.h"
//#include "ToolZ/OMKeyHash.h"

#include <boost/foreach.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/function.hpp>

//serialization
#include "ToolZ/__SERIALIZATION.h"
static const unsigned indexmatrix_version_ = 0;

#if SERIALIZATION_ENABLED
  #if FUTURE
    #include <boost/dynamic_bitset/serialization.hpp> //FUTURE
    #include <boost/serialization/dynamic_bitset.hpp> //FUTURE
  #else
//NOTE Include serialization for boost::dynamic_bitset
  namespace SERIALIZATION_NS_BASE { namespace serialization {
  template <typename Ar, typename Block, typename Alloc>
      void save(Ar& ar, ::boost::dynamic_bitset<Block, Alloc> const& bs, unsigned version) {
          size_t num_bits = bs.size();
          std::vector<Block> blocks(bs.num_blocks());
          to_block_range(bs, blocks.begin());
          
          ar & SERIALIZATION_NS::make_nvp("num_bits", num_bits); 
          ar & SERIALIZATION_NS::make_nvp("blocks", blocks);
      };

  template <typename Ar, typename Block, typename Alloc>
      void load(Ar& ar, ::boost::dynamic_bitset<Block, Alloc>& bs, unsigned version) {
          size_t num_bits;
          std::vector<Block> blocks;
          ar & SERIALIZATION_NS::make_nvp("num_bits", num_bits);
          ar & SERIALIZATION_NS::make_nvp("blocks", blocks);

          bs.resize(num_bits);
          from_block_range(blocks.begin(), blocks.end(), bs);
          bs.resize(num_bits);
      };

  template <typename Ar, typename Block, typename Alloc>
      void serialize(Ar& ar, ::boost::dynamic_bitset<Block, Alloc>& bs, unsigned version) {
          split_free(ar, bs, version);
      };
  }; }; //namespace
  
  typedef boost::dynamic_bitset<> DynamicBitSet;
  SERIALIZATION_CLASS_VERSION(DynamicBitSet, indexmatrix_version_);
  #endif //FUTURE
#endif //SERIALIZATION_ENABLED


/**
 * two-dimentional maps
 */
namespace indexmatrix{
  /**
   * @brief a two-dimentional map holding entries of type 'Base'
   * 'internal' representation is a linear array of some sort,
   * which can be evaluated in any field and typecast to 'Base'
   * @template Base a basic, or complex datatype
   * @template internal a vectorized array, which supports operator[]
   */
  template <typename Base, class Internal>
  class IndexMatrix {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const IndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, IndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned version);
  #endif //SERIALIZATION_ENABLED
  protected: //properties
    ///the size of the indexable range
    const size_t biSize_;
    ///thingy holding information
    Internal internal_;
  
  protected: //constructors
    /// hidden constructor
    /// \param biSize of the matrix in one dimention
    /// \param mapSize size of the internal container that needs to be hold
    IndexMatrix (const unsigned biSize,
                 const unsigned mapSize);
    /// copy constructor
//     IndexMatrix (const IndexMatrix& other);
    /// destructor
    virtual
    ~IndexMatrix();
    
  private: //internal methods
    //forward declaration
    virtual
    unsigned BiIndex_To_UniIndex(
      const unsigned indexA,
      const unsigned indexB) const = 0;
      
    virtual
    std::pair<unsigned, unsigned> UniIndex_To_BiIndex(
      const unsigned index) const = 0;

  public: //methods
    /** @brief get the value for field
     * @param indexA this one
     * @param indexB and this one
     */
    Base Get (const unsigned indexA,
              const unsigned indexB) const;
    /** @brief set the value for field
     * @param indexA this one
     * @param indexB and this one
     * @param value to this value
     */
    void Set (const unsigned indexA,
              const unsigned indexB,
              const Base value);
    /// get the size of the indexable range, which is [0, bisize-1]
    size_t GetBiSize() const;
  };


  //===================== CLASS SymmetricIndexMatrix =========================
  
  ///dynamic symmetric BiIndexed -map; input is anything [0..x][0..x]; only upper half and diagonal is filled
  ///NOTE beware, there is no explicit check if you leave the indexable range, that is your responsibility
  template <typename Base, class Internal>
  class SymmetricIndexMatrix : public IndexMatrix<Base, Internal> {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const SymmetricIndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, SymmetricIndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  protected: // hidden methods
    ///convert from Bi-indexed representation to a linear index
    unsigned BiIndex_To_UniIndex(
      const unsigned indexA,
      const unsigned indexB) const;
    ///convert from linear index to Bi-indexed representation
    /// @return a pair of indexA and indexB, where indexA<=indexB
    std::pair<unsigned, unsigned> UniIndex_To_BiIndex(const unsigned index) const;
  public:
    /// constructor
    /// @param biSize that is the range of the biIndex
    SymmetricIndexMatrix<Base, Internal> (const unsigned biSize);
    
//     SymmetricIndexMatrix<Base, Internal> (const SymmetricIndexMatrix<Base, Internal>& other) :
//       IndexMatrix<Base, Internal>(other)
//       {};
  };
  
  
  //===================== CLASS AsymmetricIndexMatrix =========================
  
  ///dynamic asymmetric BiIndexed map; input is anything [0..x][0..x]; all fields are filled
  ///NOTE beware, there is no explicit check if you leave the indexable range, that is your responsibile
  template <typename Base, class Internal>
  class AsymmetricIndexMatrix : public IndexMatrix<Base, Internal> {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const AsymmetricIndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, AsymmetricIndexMatrix * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  protected: // hidden methods
    ///convert from Bi-indexed representation to a linear index
    unsigned BiIndex_To_UniIndex(
      const unsigned indexA,
      const unsigned indexB) const;
    ///convert from linear index to Bi-indexed representation
    /// @return a pair of indexA and indexB
    std::pair<unsigned, unsigned> UniIndex_To_BiIndex(const unsigned index) const;  
      
  public:
    /// constructor
    /// @param biSize that is the range of the biIndex
    AsymmetricIndexMatrix<Base, Internal>(const unsigned biSize);
    
//     AsymmetricIndexMatrix<Base, Internal>(const AsymmetricIndexMatrix<Base, Internal>& other) :
//       IndexMatrix<Base, Internal>(other)
//       {};
      
    ///construct from a asymetric matrix from a symmetric matrix
    AsymmetricIndexMatrix<Base, Internal>(const SymmetricIndexMatrix<Base, Internal>& other);
  };
  

  //================== CLASS  (A)SymmetricIndexMatrix_Double ==================
  
  /// specialized class of AsymmetricIndexMatrix for Double
  class AsymmetricIndexMatrix_Double : public AsymmetricIndexMatrix<double, std::vector<double> > {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const AsymmetricIndexMatrix_Double * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, AsymmetricIndexMatrix_Double * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  public:
    //need constructor that initializes the fields to dafault value : NAN
    AsymmetricIndexMatrix_Double (const unsigned biSize);
  };
  
  /// specialized class of SymmetricIndexMatrix for Double
  class SymmetricIndexMatrix_Double : public SymmetricIndexMatrix<double, std::vector<double> > {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
    Archive & ar, const SymmetricIndexMatrix_Double * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
    Archive & ar, SymmetricIndexMatrix_Double * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  public:
    //need constructor that initializes the fields to dafault value : NAN
    SymmetricIndexMatrix_Double (const unsigned biSize);
  };
  
  
  //================== CLASS  (A)SymmetricIndexMatrix_Bool ==================

  /// specializzed class SymmetricIndexMatrix for Bool
  class SymmetricIndexMatrix_Bool : public SymmetricIndexMatrix<bool, boost::dynamic_bitset<> > {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const SymmetricIndexMatrix_Bool * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, SymmetricIndexMatrix_Bool * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  public:
    //constructor explicit
    SymmetricIndexMatrix_Bool (const unsigned biSize, const bool setall = false);
    //constructor predicate
    SymmetricIndexMatrix_Bool (const unsigned biSize, const boost::function<bool (const uint64_t, const uint64_t)> predicate);
    ///emplace with bitwise or
    SymmetricIndexMatrix_Bool& operator|=(const SymmetricIndexMatrix_Bool& rhs);
    ///emplace with bitwise and
    SymmetricIndexMatrix_Bool& operator&=(const SymmetricIndexMatrix_Bool& rhs);
  };
    
  /// specializzed class AsymmetricIndexMatrix for Bool
  class AsymmetricIndexMatrix_Bool : public AsymmetricIndexMatrix<bool, boost::dynamic_bitset<> > {
  #if SERIALIZATION_ENABLED
    friend class SERIALIZATION_NS::access;
    
    template<class Archive>
    friend void SERIALIZATION_NS::save_construct_data(
      Archive & ar, const AsymmetricIndexMatrix_Bool * t, const unsigned int file_version);
    
    template<class Archive>
    friend void SERIALIZATION_NS::load_construct_data(
      Archive & ar, AsymmetricIndexMatrix_Bool * t, const unsigned int file_version);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
  #endif //SERIALIZATION_ENABLED
  public:
    //constructor explicit
    AsymmetricIndexMatrix_Bool (const unsigned biSize, const bool setall = false);
    //constructor predicate
    AsymmetricIndexMatrix_Bool (const unsigned biSize, const boost::function<bool (const uint64_t, const uint64_t)> predicate);
    ///emplace with bitwise or
    AsymmetricIndexMatrix_Bool& operator|=(const AsymmetricIndexMatrix_Bool& rhs);
    ///emplace with bitwise and
    AsymmetricIndexMatrix_Bool& operator&=(const AsymmetricIndexMatrix_Bool& rhs);    
  };
  
  typedef boost::shared_ptr<AsymmetricIndexMatrix_Double> AsymmetricIndexMatrix_DoublePtr;
  typedef boost::shared_ptr<const AsymmetricIndexMatrix_Double> AsymmetricIndexMatrix_DoubleConstPtr;
  typedef boost::shared_ptr<SymmetricIndexMatrix_Double> SymmetricIndexMatrix_DoublePtr;
  typedef boost::shared_ptr<const SymmetricIndexMatrix_Double> SymmetricIndexMatrix_DoubleConstPtr;
  typedef boost::shared_ptr<AsymmetricIndexMatrix_Bool> AsymmetricIndexMatrix_BoolPtr;
  typedef boost::shared_ptr<const AsymmetricIndexMatrix_Bool> AsymmetricIndexMatrix_BoolConstPtr;
  typedef boost::shared_ptr<SymmetricIndexMatrix_Bool> SymmetricIndexMatrix_BoolPtr;
  typedef boost::shared_ptr<const SymmetricIndexMatrix_Bool> SymmetricIndexMatrix_BoolConstPtr;
}; //indexmatrix


#if SERIALIZATION_ENABLED
  SERIALIZATION_CLASS_VERSION(indexmatrix::AsymmetricIndexMatrix_Double, indexmatrix_version_);
  SERIALIZATION_CLASS_VERSION(indexmatrix::SymmetricIndexMatrix_Double, indexmatrix_version_);
  SERIALIZATION_CLASS_VERSION(indexmatrix::AsymmetricIndexMatrix_Bool, indexmatrix_version_);
  SERIALIZATION_CLASS_VERSION(indexmatrix::SymmetricIndexMatrix_Bool, indexmatrix_version_);
#endif //SERIALIZATION_ENABLED

  
//==============================================================================
//========================== IMPLEMENTATION ====================================
//==============================================================================

#if SERIALIZATION_ENABLED
template <typename Base, class Internal>
template <class Archive>
void indexmatrix::IndexMatrix<Base, Internal>::serialize(Archive & ar, const unsigned version) 
{ ar & SERIALIZATION_NS::make_nvp("Internal", internal_); };

namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive, typename Base, class Internal>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::IndexMatrix<Base,Internal> * t, const unsigned int file_version)
  { ar << SERIALIZATION_NS::make_nvp("biSize", t->biSize_); };

  template<class Archive, typename Base, class Internal>
  inline void load_construct_data(
    Archive & ar, indexmatrix::IndexMatrix<Base,Internal> * t, const unsigned int file_version)
  {
    unsigned biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::IndexMatrix<Base, Internal>(biSize);
  };
}}; // namespace ...
#endif //SERIALIZATION_ENABLED

template <typename Base, class Internal>
indexmatrix::IndexMatrix<Base, Internal>::IndexMatrix (const unsigned biSize, const unsigned mapSize) :
  biSize_(biSize),
  internal_(mapSize) 
{};

// template <typename Base, class Internal>
// indexmatrix::IndexMatrix<Base, Internal>::IndexMatrix (const indexmatrix::IndexMatrix<Base, Internal>& other):
//   biSize_(other.biSize_),
//   internal_(other.internal_)
// {};

template <typename Base, class Internal>
indexmatrix::IndexMatrix<Base, Internal>::~IndexMatrix ()
{};

template <typename Base, class Internal>
Base 
indexmatrix::IndexMatrix<Base, Internal>::Get (const unsigned indexA, const unsigned indexB) const
  {return internal_[BiIndex_To_UniIndex(indexA, indexB)];};

template <typename Base, class Internal>
void 
indexmatrix::IndexMatrix<Base, Internal>::Set (const unsigned indexA, const unsigned indexB, const Base value)
  {internal_[BiIndex_To_UniIndex(indexA, indexB)]=value;};
  
template <typename Base, class Internal>
size_t
indexmatrix::IndexMatrix<Base, Internal>::GetBiSize () const
  {return biSize_;};
  

//===================== CLASS AsymmetricIndexMatrix =========================
  
#if SERIALIZATION_ENABLED 
template <typename Base, class Internal>
template <class Archive>
void indexmatrix::AsymmetricIndexMatrix<Base, Internal>::serialize(Archive & ar, const unsigned version) {
  typedef IndexMatrix<Base, Internal> IM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( IM );
};

namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive, typename Base, class Internal>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::AsymmetricIndexMatrix<Base,Internal> * t, const unsigned int file_version)
  { ar << SERIALIZATION_NS::make_nvp("biSize", t->biSize_); };

  template<class Archive, typename Base, class Internal>
  inline void load_construct_data(
    Archive & ar, indexmatrix::AsymmetricIndexMatrix<Base,Internal> * t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::AsymmetricIndexMatrix<Base, Internal>(biSize);
  };
}}; // namespace ...
#endif

template <typename Base, class Internal>
indexmatrix::AsymmetricIndexMatrix<Base, Internal>::AsymmetricIndexMatrix(const unsigned biSize) :
  IndexMatrix<Base, Internal>(biSize, biSize*biSize)
{};  
  
// template <typename Base, class Internal>
// indexmatrix::AsymmetricIndexMatrix<Base, Internal>::AsymmetricIndexMatrix(const SymmetricIndexMatrix<Base, Internal>& other) :
//   AsymmetricIndexMatrix<Base, Internal>(other.biSize_)
// {
//   for (unsigned index=0; index<this->biSize_; index++) {
//     std::pair<unsigned, unsigned> AB = other.UniIndex_To_BiIndex(index);
//     const Base value = other.Get(AB.first, AB.second);
//     Set(AB.first, AB.second, value);
//     Set(AB.first, AB.second, value);
//   }
// };

template <typename Base, class Internal>
unsigned indexmatrix::AsymmetricIndexMatrix<Base, Internal>::BiIndex_To_UniIndex(const unsigned indexA, const unsigned indexB) const
  {return (indexA*IndexMatrix<Base, Internal>::biSize_)+indexB;};  

template <typename Base, class Internal>
std::pair<unsigned, unsigned> 
indexmatrix::AsymmetricIndexMatrix<Base, Internal>::UniIndex_To_BiIndex(const unsigned index) const 
{
  const unsigned indexA = index / IndexMatrix<Base, Internal>::biSize_;
  const unsigned indexB = index - IndexMatrix<Base, Internal>::biSize_*indexA;
  return std::make_pair(indexA, indexB);
};
  

//===================== CLASS SymmetricIndexMatrix =========================

#if SERIALIZATION_ENABLED
namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive, typename Base, class Internal>
  inline void save_construct_data(
      Archive & ar, const indexmatrix::SymmetricIndexMatrix<Base,Internal> * t, const unsigned int file_version)
  { ar << SERIALIZATION_NS::make_nvp("biSize", t->biSize_); };

  template<class Archive, typename Base, class Internal>
  inline void load_construct_data(
    Archive & ar, indexmatrix::SymmetricIndexMatrix<Base,Internal> * t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::SymmetricIndexMatrix<Base, Internal>(biSize);
  };
}}; // namespace ...

template <typename Base, class Internal>
template <class Archive>
void indexmatrix::SymmetricIndexMatrix<Base, Internal>::serialize(Archive & ar, const unsigned version) {
  typedef IndexMatrix<Base, Internal> IM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( IM );
};
#endif //SERIALIZATION_ENABLED

template <typename Base, class Internal>
indexmatrix::SymmetricIndexMatrix<Base, Internal>::SymmetricIndexMatrix (const unsigned biSize) :
  IndexMatrix<Base, Internal>(biSize, (biSize*biSize+biSize)/2) 
{};
  
template <typename Base, class Internal>
unsigned indexmatrix::SymmetricIndexMatrix<Base, Internal>::BiIndex_To_UniIndex(
  const unsigned indexA,
  const unsigned indexB) const
{
  if (indexB > indexA)
    return BiIndex_To_UniIndex(indexB, indexA);
  return (indexA*indexA+indexA)/2+indexB; 
};

template <typename Base, class Internal>
std::pair<unsigned, unsigned> 
indexmatrix::SymmetricIndexMatrix<Base, Internal>::UniIndex_To_BiIndex(const unsigned index) const 
{
  if (index > (IndexMatrix<Base, Internal>::biSize_*IndexMatrix<Base, Internal>::biSize_+IndexMatrix<Base, Internal>::biSize_)/2 + IndexMatrix<Base, Internal>::biSize_)
    log_fatal("out of indexable range");
  
  uint64_t indexA=0;
  uint64_t offset = (indexA*indexA+indexA)/2;
  while (offset <= index) {
    indexA++;
    offset = (indexA*indexA+indexA)/2;
  }
  indexA--;
  const uint64_t indexB = index - (indexA*indexA+indexA)/2;
  return std::make_pair(indexA, indexB);
};

//===================== CLASS SymmetricIndexMatrix_Double =========================

#if SERIALIZATION_ENABLED
namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::SymmetricIndexMatrix_Double * t, const unsigned int file_version)
  {
    const size_t biSize = t->GetBiSize();
    ar << SERIALIZATION_NS::make_nvp("biSize", biSize); 
  };
  
  template<class Archive>
  inline void load_construct_data(
      Archive & ar, indexmatrix::SymmetricIndexMatrix_Double* t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::SymmetricIndexMatrix_Double(biSize);
  };
}}; // namespace ...

template <class Archive>
void indexmatrix::SymmetricIndexMatrix_Double::serialize(Archive & ar, const unsigned version) {
  typedef SymmetricIndexMatrix<double, std::vector<double> > SIM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( SIM );
};
#endif //SERIALIZATION_ENABLED 

//===================== CLASS AsymmetricIndexMatrix_Double =========================

#if SERIALIZATION_ENABLED
namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::AsymmetricIndexMatrix_Double * t, const unsigned int file_version)
  {
    const size_t biSize = t->GetBiSize();
    ar << SERIALIZATION_NS::make_nvp("biSize", biSize); 
  };
  
  template<class Archive>
  inline void load_construct_data(
    Archive & ar, indexmatrix::AsymmetricIndexMatrix_Double* t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::AsymmetricIndexMatrix_Double(biSize);
  };
}}; // namespace ...

template <class Archive>
void indexmatrix::AsymmetricIndexMatrix_Double::serialize(Archive & ar, const unsigned version) {
  typedef AsymmetricIndexMatrix<double, std::vector<double> > AIM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( AIM );
};
#endif //SERIALIZATION_ENABLED 

//===================== CLASS SymmetricIndexMatrix_Bool =========================

#if SERIALIZATION_ENABLED
namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::SymmetricIndexMatrix_Bool * t, const unsigned int file_version)
  {
    const size_t biSize = t->GetBiSize();
    ar << SERIALIZATION_NS::make_nvp("biSize", biSize); 
  };
  
  template<class Archive>
  inline void load_construct_data(
      Archive & ar, indexmatrix::SymmetricIndexMatrix_Bool* t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::SymmetricIndexMatrix_Bool(biSize);
  };
}}; // namespace ...

template <class Archive>
void indexmatrix::SymmetricIndexMatrix_Bool::serialize(Archive & ar, const unsigned version) {
  typedef SymmetricIndexMatrix<bool, boost::dynamic_bitset<> > SIM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( SIM );
};
#endif //SERIALIZATION_ENABLED

//===================== CLASS AsymmetricIndexMatrix_Bool =========================

#if SERIALIZATION_ENABLED
namespace SERIALIZATION_NS_BASE { namespace serialization {
  template<class Archive>
  inline void save_construct_data(
    Archive & ar, const indexmatrix::AsymmetricIndexMatrix_Bool * t, const unsigned int file_version)
  {
    const size_t biSize = t->GetBiSize();
    ar << SERIALIZATION_NS::make_nvp("biSize", biSize); 
  };
  
  template<class Archive>
  inline void load_construct_data(
    Archive & ar, indexmatrix::AsymmetricIndexMatrix_Bool* t, const unsigned int file_version)
  {
    size_t biSize;
    ar >> SERIALIZATION_NS::make_nvp("biSize", biSize);
    ::new(t)indexmatrix::AsymmetricIndexMatrix_Bool(biSize);
  };
}}; // namespace ...

template <class Archive>
void indexmatrix::AsymmetricIndexMatrix_Bool::serialize(Archive & ar, const unsigned version) {
  typedef AsymmetricIndexMatrix<bool, boost::dynamic_bitset<> > AIM;
  ar & SERIALIZATION_BASE_OBJECT_NVP( AIM );
};
#endif //SERIALIZATION_ENABLED 


#endif // INDEXMATRIX_H
