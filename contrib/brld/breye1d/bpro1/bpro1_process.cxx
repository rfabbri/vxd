// This is brl/bpro1/bpro1_process.cxx

//:
// \file

#include <bpro1/bpro1_process.h>

#include <vcl_iostream.h>
#include <vcl_cassert.h>

#include <bpro1/bpro1_parameters.h>
#include <bpro1/bpro1_storage.h>


bpro1_process::bpro1_process()
{
  parameters_ = new bpro1_parameters;

}


bpro1_process::~bpro1_process()
{
}


//: Copy Constructor
bpro1_process::bpro1_process(const bpro1_process& other)
 : input_data_(other.input_data_), output_data_(other.output_data_),
   parameters_(new bpro1_parameters(other.parameters_)), // deep copy of parameters
   input_names_(other.input_names_), output_names_(other.output_names_)
{
  ///this->ref_count = 0;
}


void bpro1_process::clear_input(int size) 
{
  input_data_.clear();
  if(size < 0)
    input_data_.resize(this->input_frames());
  else
    input_data_.resize(size);
}


void bpro1_process::clear_output(int size)
{
  output_data_.clear();
  if(size < 0)
    output_data_.resize(this->output_frames());
  else
    output_data_.resize(size);
}


void bpro1_process::set_input( const vcl_vector< bpro1_storage_sptr >& i , unsigned frame)
{
  if (input_data_.size()<frame+1)
    input_data_.resize(frame+1);
  input_data_[frame] = i;
}


void bpro1_process::add_input(const bpro1_storage_sptr& i, unsigned frame)
{
  if (input_data_.size()<frame+1)
    input_data_.resize(frame+1);
  input_data_[frame].push_back(i);
}


vcl_vector< bpro1_storage_sptr > const & bpro1_process::get_output(unsigned frame)
{
  assert(output_data_.size()>frame);
  return output_data_[frame];
}


vcl_vector< bpro1_storage_sptr > const & bpro1_process::get_input(unsigned frame)
{
  assert(input_data_.size()>frame);
  return input_data_[frame];
}


bpro1_parameters_sptr bpro1_process::parameters()
{
  return parameters_;
}


void bpro1_process::set_parameters(const bpro1_parameters_sptr& params)
{
  parameters_ = params;
}

//: Returns a vector of strings with suggested names for output classes
vcl_vector< vcl_string > bpro1_process::suggest_output_names()
{
  return vcl_vector< vcl_string >();
}
