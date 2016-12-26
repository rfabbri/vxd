// This is basic/dbil/pro/dbil_pro_register.h
#ifndef dbil_pro_register_h_
#define dbil_pro_register_h_
//:
// \file
// \brief Include this file in your executable to force registration of this library
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 1/3/05

#include <dbpro1/dbpro_storage.h>
#include <vil/vil_image_resource_sptr.h>

//: A global instance of this object to force the constructor
dbpro_storage_type<vil_image_resource_sptr> dbil_pro_reg_img_stg;


#endif // dbil_pro_register_h_
