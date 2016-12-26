// This is basic/dbpro/dbpro_observer_sptr.h
#ifndef dbpro_observer_sptr_h
#define dbpro_observer_sptr_h

//:
// \file

class dbpro_observer;
#include <vbl/vbl_smart_ptr.h>

struct dbpro_observer_sptr : public vbl_smart_ptr<dbpro_observer> {
  typedef vbl_smart_ptr<dbpro_observer> base;

  dbpro_observer_sptr() {}
  dbpro_observer_sptr(dbpro_observer* p): base(p) {}
  void vertical_cast(dbpro_observer_sptr const& that) { *this = that; }
  void vertical_cast(dbpro_observer* t) { *this = t; }
};

// Stop doxygen documenting the B class
#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <class T, class B = dbpro_observer_sptr>
struct dbpro_observer_sptr_t : public B {
  dbpro_observer_sptr_t(): B() {}
  dbpro_observer_sptr_t(T* p): B(p) {}
  dbpro_observer_sptr_t(dbpro_observer_sptr_t<T> const& r): B(r) {}
  void operator=(dbpro_observer_sptr_t<T> const& r) { B::operator=(r); }
  T* operator->() const { return (T*)this->as_pointer(); }
  T& operator*() const { return *((T*)this->as_pointer()); }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#endif // dbpro_observer_sptr_h
