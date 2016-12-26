// This is basic/dbpro/dbpro_connector_sptr.h
#ifndef dbpro_connector_sptr_h
#define dbpro_connector_sptr_h

//:
// \file

class dbpro_connector;

#include <dbpro/dbpro_observer_sptr.h>

//: Smart-pointer to a dbpro_connector.
typedef dbpro_observer_sptr_t<dbpro_connector> dbpro_connector_sptr;

#endif // dbpro_connector_sptr_h
