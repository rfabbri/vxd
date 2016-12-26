#include <vbl/io/vbl_io_sparse_array_base.hxx>
#include <bmcsd/bmcsd_discrete_corresp.h>
#include <vbl/vbl_triple.h>
#include <vbl/io/vbl_io_triple.h>

typedef vbl_triple<unsigned, unsigned, unsigned> triple_uuu;
VBL_IO_SPARSE_ARRAY_BASE_INSTANTIATE(bmcsd_match_attribute, triple_uuu);
