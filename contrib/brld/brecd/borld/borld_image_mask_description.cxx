//:
// \file
// \brief 
// \author     Ozge Can Ozcanli (ozge@lems.brown.edu)
// \date        10/03/07
#include "dborl_image_mask_description.h"

#include <vcl_iostream.h>

unsigned dborl_image_mask_description::version()
{
  return 0;
}

void dborl_image_mask_description::b_read()
{
  vcl_cout << "IMPLEMENT: dborl_image_mask_description::b_read()\n";
}

void dborl_image_mask_description::b_write()
{
  vcl_cout << "IMPLEMENT: dborl_image_mask_description::b_write()\n";
}

//: nothing to write to the xml file --> just empty implementation to prevent abstract class instantiation error (error C2259)
void dborl_image_mask_description::write_xml(vcl_ostream& os)
{
}
