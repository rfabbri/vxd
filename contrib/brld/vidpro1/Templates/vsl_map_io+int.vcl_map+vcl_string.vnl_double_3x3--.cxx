#include <vcl_string.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/io/vnl_io_matrix_fixed.h>
#include <vsl/vsl_map_io.hxx>
 
typedef vcl_map<vcl_string,vnl_double_3x3> map_type;
VSL_MAP_IO_INSTANTIATE(int, map_type, vcl_less<int>);
