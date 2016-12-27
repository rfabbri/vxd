// This is basic/bprod/bprod_connector_sptr.h
#ifndef bprod_connector_sptr_h
#define bprod_connector_sptr_h

//:
// \file

class bprod_connector;

#include <bprod/bprod_observer_sptr.h>

//: Smart-pointer to a bprod_connector.
typedef bprod_observer_sptr_t<bprod_connector> bprod_connector_sptr;

#endif // bprod_connector_sptr_h
