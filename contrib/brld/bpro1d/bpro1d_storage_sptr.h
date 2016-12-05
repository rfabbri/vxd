// This is brld/bpro1d/bpro1d_storage_sptr.h
#ifndef bpro1d_storage_sptr_h
#define bpro1d_storage_sptr_h

//:
// \file

class bpro1d_storage;

#include <vbl/vbl_smart_ptr.h>

struct bpro1d_storage_sptr : public vbl_smart_ptr<bpro1d_storage> {
  typedef vbl_smart_ptr<bpro1d_storage> base;

  bpro1d_storage_sptr() {}
  bpro1d_storage_sptr(bpro1d_storage* p): base(p) {}
  void vertical_cast(bpro1d_storage_sptr const& that) { *this = that; }
  void vertical_cast(bpro1d_storage* t) { *this = t; }
};


// Stop doxygen documenting the B class
#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <class T, class B = bpro1d_storage_sptr>
struct bpro1d_storage_sptr_t : public B {
  bpro1d_storage_sptr_t(): B() {}
  bpro1d_storage_sptr_t(T* p): B(p) {}
  bpro1d_storage_sptr_t(bpro1d_storage_sptr_t<T> const& r): B(r) {}
  void operator=(bpro1d_storage_sptr_t<T> const& r) { B::operator=(r); }
  T* operator->() const { return (T*)this->as_pointer(); }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#endif // bpro1d_storage_sptr_h
