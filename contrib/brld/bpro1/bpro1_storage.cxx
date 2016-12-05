// This is brl/bpro1/bpro1_storage.cxx

//:
// \file

#include "bpro1_storage.h"
#include <iostream>


//: Destructor
bpro1_storage::~bpro1_storage()
{
}


//: Constructor
bpro1_storage::bpro1_storage()
 : name_("defaultname"), frame_(-3)
{
}


//: Constructor
bpro1_storage::bpro1_storage(const vcl_string& name)
 : name_(name)
{
}

//: Set the name of this storage class instance
void 
bpro1_storage::set_name(const vcl_string& name)
{
  name_ = name;
}


//: Returns the name of this storage class instance
vcl_string
bpro1_storage::name() const
{
  return name_;
}

  
//: Mark a new storage class for storage at all frames
bool 
bpro1_storage::mark_global()
{
  if(frame_ <= -2){
    frame_ = -2;
    return true;
  }
  return false;
}
  

//: Returns the frame number associated with this instance
int
bpro1_storage::frame() const
{
  return frame_;
}


//: Binary save self to stream.
void 
bpro1_storage::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os, this->frame_);
  vsl_b_write(os, this->name_);
}


//: Binary load self from stream.
void 
bpro1_storage::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:
    vsl_b_read(is, this->frame_);
    vsl_b_read(is, this->name_);
    break;

  default:
    vcl_cerr << "I/O ERROR: bpro1_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}


//: Return IO version number;
short 
bpro1_storage::version() const
{
  return 1;
}


//==============================================
//: Allows derived class to be loaded by base-class pointer.
//  A loader object exists which is invoked by calls
//  of the form "vsl_b_read(os,base_ptr);".  This loads derived class
//  objects from the stream, places them on the heap and
//  returns a base class pointer.
//  In order to work the loader object requires
//  an instance of each derived class that might be
//  found.  This function gives the model class to
//  the appropriate loader.
void vsl_add_to_binary_loader(const bpro1_storage& b)
{
  vsl_binary_loader<bpro1_storage>::instance().add(b);
}


//==============================================
//: Stream summary output for base class pointer
void vsl_print_summary(vcl_ostream& os,const bpro1_storage* s)
{
  if (s)
    os << "bpro1_storage{type:"<<s->type()<<"}";
  else
    os << "No bpro1_storage defined.\n";
}


