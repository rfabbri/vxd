// This is basic/bprod/bprod_storage.h
#ifndef bprod_storage_h_
#define bprod_storage_h_

//:
// \file
// \brief The storage class
// \author Matt Leotta
// \date 5/26/06
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_typeinfo.h>
#include <vcl_cassert.h>
#include <vcl_iostream.h>
#include <vbl/vbl_ref_count.h>
#include "bprod_fwd.h"
#include "bprod_storage_sptr.h"

template< class T > class bprod_storage_type;

//: This abstract class is the base class for storage objects
class bprod_storage : public vbl_ref_count
{
 public:
  //: Constructor
  bprod_storage() {}

  //: Destructor
  virtual ~bprod_storage() {}

  //: Return a signal (BPROD_VALID for valid data)
  virtual bprod_signal info() const = 0;

  //: Return the type of the stored data
  virtual const vcl_type_info& type_id() const = 0;

  //: Returns true if a parameter exists with \p name and type \p T
  template<class T>
  const T& data() const
  {
    return static_cast<const bprod_storage_type<T>*>(this)->data();
  }

 private:
  bprod_storage(const bprod_storage& other);

};


//=============================================================================


//: A Templated parameter class
template< class T >
class bprod_storage_type : public bprod_storage
{
 public:
  //: Default Constructor
  bprod_storage_type<T>() {}

  //: Constructor
  bprod_storage_type<T>(const T& data)
   : data_(data) {}

  //: Return a message flag
  bprod_signal info() const { return BPROD_VALID; }

  //: Return a const reference to the data
  const T& data() const { return data_; }

  //: Return the type of the stored data
  const vcl_type_info& type_id() const { return typeid(T); }

 private:
  //: The stored data
  T data_;
};


//=============================================================================

typedef bprod_storage_type<bprod_signal> bprod_storage_signal;

//: A storage class that contains only a message
template <>
class bprod_storage_type<bprod_signal> : public bprod_storage
{
 public:
  //: Default Constructor
  bprod_storage_type<bprod_signal>(bprod_signal info=BPROD_INVALID) : info_(info) {}

  //: Return a message flag
  bprod_signal info() const { return info_; }

  //: Return the type of the stored data
  const vcl_type_info& type_id() const { return typeid(bprod_signal); }

  private:
  //: The message
  bprod_signal info_;
};


#endif // bprod_storage_h_
