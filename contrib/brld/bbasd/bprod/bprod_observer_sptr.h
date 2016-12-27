// This is basic/bprod/bprod_observer_sptr.h
#ifndef bprod_observer_sptr_h
#define bprod_observer_sptr_h

//:
// \file

class bprod_observer;
#include <vbl/vbl_smart_ptr.h>

struct bprod_observer_sptr : public vbl_smart_ptr<bprod_observer> {
  typedef vbl_smart_ptr<bprod_observer> base;

  bprod_observer_sptr() {}
  bprod_observer_sptr(bprod_observer* p): base(p) {}
  void vertical_cast(bprod_observer_sptr const& that) { *this = that; }
  void vertical_cast(bprod_observer* t) { *this = t; }
};

// Stop doxygen documenting the B class
#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <class T, class B = bprod_observer_sptr>
struct bprod_observer_sptr_t : public B {
  bprod_observer_sptr_t(): B() {}
  bprod_observer_sptr_t(T* p): B(p) {}
  bprod_observer_sptr_t(bprod_observer_sptr_t<T> const& r): B(r) {}
  void operator=(bprod_observer_sptr_t<T> const& r) { B::operator=(r); }
  T* operator->() const { return (T*)this->as_pointer(); }
  T& operator*() const { return *((T*)this->as_pointer()); }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#endif // bprod_observer_sptr_h
