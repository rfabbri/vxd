#include <bpro1/bpro1_storage_sptr.h>
#include <vcl_string.h>
#include <vcl_vector.h>
#include <vsl/vsl_map_io.h>
#include <vsl/vsl_vector_io.hxx>
 
typedef vcl_map< vcl_string, vcl_vector< bpro1_storage_sptr > > storage_map;

VSL_VECTOR_IO_INSTANTIATE(storage_map);
