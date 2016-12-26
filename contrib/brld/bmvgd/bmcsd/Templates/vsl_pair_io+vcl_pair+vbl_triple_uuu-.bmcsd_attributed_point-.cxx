#include <vsl/vsl_pair_io.hxx>
#include <vbl/vbl_triple.h>
#include <vbl/io/vbl_io_triple.h>
#include <bmcsd/bmcsd_discrete_corresp.h>

typedef vbl_triple<unsigned,unsigned,unsigned> triple_uu;

VSL_PAIR_IO_INSTANTIATE(triple_uu, bmcsd_match_attribute);
