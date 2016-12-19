#include <bpro1/bpro1_storage_sptr.h>
#include <vbl/io/vbl_io_smart_ptr.h>
#include <vcl_string.h>
#include <vsl/vsl_map_io.hxx>
 
VSL_MAP_IO_INSTANTIATE(vcl_string, bpro1_storage_sptr, vcl_less<vcl_string>);
