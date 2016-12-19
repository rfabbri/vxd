#include <bpro1d/bpro1d_storage_sptr.h>
#include <vcl_string.h>
#include <vcl_vector.h>
#include <vsl/vsl_vector_io.h>
#include <vsl/vsl_map_io.hxx>
 
VSL_MAP_IO_INSTANTIATE(vcl_string, vcl_vector<bpro1d_storage_sptr>, vcl_less<vcl_string>);
