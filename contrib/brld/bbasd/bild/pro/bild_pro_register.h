// This is basic/bild/pro/bild_pro_register.h
#ifndef bild_pro_register_h_
#define bild_pro_register_h_
//:
// \file
// \brief Include this file in your executable to force registration of this library
// \author Based on original code by  Matt Leotta, (@lems.brown.edu)
// \date 1/3/05

#include <bprod1/bprod_storage.h>
#include <vil/vil_image_resource_sptr.h>

//: A global instance of this object to force the constructor
bprod_storage_type<vil_image_resource_sptr> bild_pro_reg_img_stg;


#endif // bild_pro_register_h_
