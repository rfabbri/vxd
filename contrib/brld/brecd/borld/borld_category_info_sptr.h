// This is brcv/rec/dborl/dborl_category_info_sptr.h
#ifndef dborl_category_info_sptr_h_
#define dborl_category_info_sptr_h_
//:
// \file
// \brief Smart pointer to a dborl_category_info
// \author Ozge Can Ozcanli, (ozge@lems.brown.edu)
// \date 10/15/07
//

#include <vbl/vbl_smart_ptr.h>

class dborl_category_info;
typedef vbl_smart_ptr<dborl_category_info> dborl_category_info_sptr;
class dborl_category_info_set;
typedef vbl_smart_ptr<dborl_category_info_set> dborl_category_info_set_sptr;

#endif // dborl_category_info_sptr_h_
