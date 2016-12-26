// This is /lemsvxl/basic/dbul/dbul_random.cxx

//:
// \file
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date   Mar 8, 2010
// \brief 
//
// \verbatim
//
// \endverbatim
//

#include "dbul_random.h"

#include<vcl_cstdlib.h>
#include<vcl_ctime.h>

void dbul_random_init()
{
    static bool is_init = false;
    if(!is_init)
    {
        vcl_srand((unsigned)vcl_time(NULL));
        is_init = true;
    }
}

vcl_string dbul_get_random_alphanumeric_string(int len)
{
    dbul_random_init();
    vcl_string ret;
    for (int i = 0; i < len; ++i) {
        char randomChar = rand()%(26+26+10);
        if (randomChar < 26)
            ret += vcl_string(1, 'a' + randomChar);
        else if (randomChar < 26+26)
            ret += vcl_string(1,'A' + randomChar - 26);
        else
            ret += vcl_string(1,'0' + randomChar - 26 - 26);
    }
    return ret;
}
