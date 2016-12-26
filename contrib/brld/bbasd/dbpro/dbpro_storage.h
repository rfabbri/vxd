// This is basic/dbpro/dbpro_storage.h
#ifndef dbpro_storage_h_
#define dbpro_storage_h_

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
#include "dbpro_fwd.h"
#include "dbpro_storage_sptr.h"

template< class T > class dbpro_storage_type;

//: This abstract class is the base class for storage objects
class dbpro_storage : public vbl_ref_count
{
 public:
  //: Constructor
  dbpro_storage() {}

  //: Destructor
  virtual ~dbpro_storage() {}

  //: Return a signal (DBPRO_VALID for valid data)
  virtual dbpro_signal info() const = 0;

  //: Return the type of the stored data
  virtual const vcl_type_info& type_id() const = 0;

  //: Returns true if a parameter exists with \p name and type \p T
  template<class T>
  const T& data() const
  {
    return static_cast<const dbpro_storage_type<T>*>(this)->data();
  }

 private:
  dbpro_storage(const dbpro_storage& other);

};


//=============================================================================


//: A Templated parameter class
template< class T >
class dbpro_storage_type : public dbpro_storage
{
 public:
  //: Default Constructor
  dbpro_storage_type<T>() {}

  //: Constructor
  dbpro_storage_type<T>(const T& data)
   : data_(data) {}

  //: Return a message flag
  dbpro_signal info() const { return DBPRO_VALID; }

  //: Return a const reference to the data
  const T& data() const { return data_; }

  //: Return the type of the stored data
  const vcl_type_info& type_id() const { return typeid(T); }

 private:
  //: The stored data
  T data_;
};


//=============================================================================

typedef dbpro_storage_type<dbpro_signal> dbpro_storage_signal;

//: A storage class that contains only a message
template <>
class dbpro_storage_type<dbpro_signal> : public dbpro_storage
{
 public:
  //: Default Constructor
  dbpro_storage_type<dbpro_signal>(dbpro_signal info=DBPRO_INVALID) : info_(info) {}

  //: Return a message flag
  dbpro_signal info() const { return info_; }

  //: Return the type of the stored data
  const vcl_type_info& type_id() const { return typeid(dbpro_signal); }

  private:
  //: The message
  dbpro_signal info_;
};


#endif // dbpro_storage_h_
