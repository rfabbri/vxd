// This is brl/bpro1/bpro1_process.h
#ifndef bpro1_process_h_
#define bpro1_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief The bpro1 process base class
//
//  A process is an object that wraps an algorithm to create
//  standardized interface.
// \author
//   J.L. Mundy
//
// \verbatim
//  Modifications:
//   J.L. Mundy    October   9, 2002  Initial version.
//   Amir Tamrakar October  30, 2003  Redesigned for Brown Eyes
//   Matt Leotta   October  18, 2004  Added some documentation
//   Matt Leotta   December 15, 2004  Migrated from vidpro
//
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_vector.h>
#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>

#include <bpro1/bpro1_parameters_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>

//: This abstract class is the base class for process objects
//
// The process manager uses polymorphism to manage and execute
// classes derived from this one.  The process manager will call
// your execute function once for every frame of the video.  The 
// process manager will call your finish function once after all
// calls to execute are finished.
//
// To make a working video process you must overload the following:
// - clone
// - name
// - input_frames
// - output_frames
// - get_input_type
// - get_output_type
// - execute
// - finish
//
// The clone and name function are trivial.  The clone function
// creates a copy of your process.  The following should be 
// sufficient in almost all cases (in some cases you may need
// to write an explicit copy constructor).
// \verbatim
// bpro1_process* bpro1_my_process::clone() const
// {
//   return new bpro1_my_process(*this);
// }
// \endverbatim
// The name function should simply return a string containing 
// a unique name.  Think of this as a title for your process.
// It must be unique for identification, but it may also 
// appear in menus to allow a user to select your process.
//
// The input_frames and output_frames functions should return
// the number of previous frames from which your process needs 
// to access and store data respectively. 
//
// The get_input_type and get_output_type functions should return
// a vector of type identifier strings that the process requires 
// as input and will produce as output.  The type identifier 
// strings must match those return by a bpro1_storage class.
//
// The execute function should be a wrapper around your algorithm.
// Before execute is called, the bpro1_process_manager will
// populate the member data structure input_data_ with smart
// pointers to the storages classes to use for input.  These
// classes are determined by the input_frames and get_input_type
// functions.  Storage classes are taken from the bpro1_repository
// if the appropriate data is available.  If not, these pointers
// will be null.  The execute function must cast these base class
// smart pointers into the appropriate storage types and then
// extract the needed data.  This data is passed to the algorithm
// to be run, and the results must be repackaged into storage
// classes.  The new storage classes should be added to the
// output_data_ vector, and the bpro1_process_manager will then
// add the output back into the bpro1_repository after the 
// execute call returns.
//
// The finish function is similar to the execute function 
// except that it is called once after a series of calls
// to execute and acts globally on the video.  The input_data_
// vector contains storage classes of the types defined
// by get_input_type, but indexing into this vector is now
// based on the global frame number (not relative).  In addition,
// data is provided from all frames that execute was called
// on.  Any output data produced by the execute calls is appended
// to the input_data_ vector after the original input.  The 
// purpose of a the finish function is to perform any global 
// calculations on the video or to provide "clean-up" for the 
// prior calls to execute. Again, any new storage classes added
// to output_data_ are submitted to the bpro1_repository when 
// the finish function returns.   
//
// Typically, the constructor is used to define the parameters
// for a process. See bpro1_parameters. 
class bpro1_process : public vbl_ref_count
{
 public:

  //: Clone the process
  virtual bpro1_process* clone() const = 0;
  
  //: Clear the input vector and then resize the empty vector
  virtual void clear_input(int resize = -1);
  //: Clear the output vector and then resize the empty vector
  virtual void clear_output(int resize = -1);
  
  //: Set this vector of inputs at the given frame
  void set_input( const vcl_vector< bpro1_storage_sptr >&, unsigned frame=0);
  //: Append the storage class to the input vector at the given frame
  void add_input( const bpro1_storage_sptr& i, unsigned frame=0);

  //: Return the vector of output storage class at the given frame
  vcl_vector< bpro1_storage_sptr >  const & get_output(unsigned frame=0);
  //: Return the vector of input storage class at the given frame
  vcl_vector< bpro1_storage_sptr >  const & get_input(unsigned frame=0);

  //: Return a smart pointer to the parameters
  bpro1_parameters_sptr parameters();
  //: Set the parameters
  void set_parameters(const bpro1_parameters_sptr& params);

  //: Set the vector of names of the storage classes to be used as input
  void set_input_names(const vcl_vector<vcl_string>& names)
  { input_names_ = names;}

  //: Set the vector of names of the storage classes to be used as output
  void set_output_names(const vcl_vector<vcl_string>& names)
  { output_names_ = names;}

  //: Return the vector of names of the storage classes to be used as input
  vcl_vector< vcl_string > input_names() const
  { return input_names_;}

  //: Return the vector of names of the storage classes to be used as output
  vcl_vector< vcl_string > output_names() const
  { return output_names_;}

  //: The name of the video process
  virtual vcl_string name() = 0;

  //: Returns the number of frames needed for input
  // (prior to and including the current frame)
  virtual int input_frames() = 0;
  //: Returns the number of frames used for output
  // (prior to and including the current frame)
  virtual int output_frames() = 0;

 //: to facilitate use of future frames.

  //: Returns a vector of strings describing the required storage class input types
  virtual vcl_vector< vcl_string > get_input_type() = 0;
  //: Returns a vector of strings describing the storage class output types produced
  virtual vcl_vector< vcl_string > get_output_type() = 0;

  //: Returns a vector of strings with suggested names for output classes
  // returns an empty vector by default
  virtual vcl_vector< vcl_string > suggest_output_names();

  //: Execute the process on using the storage classes already set.
  // \return false if any error occurs.
  virtual bool execute() = 0;
  //: Perform any clean up or final computation after computing on all frames
  virtual bool finish() = 0;
  

  //: overload this function if your process need to access future data
  virtual bool non_causal() const { return false; }
 protected:
  
  vcl_vector< vcl_vector< bpro1_storage_sptr > >  input_data_;
  vcl_vector< vcl_vector< bpro1_storage_sptr > >  output_data_;

  //: Copy Constructor
  bpro1_process(const bpro1_process& other);
  bpro1_process();
  virtual ~bpro1_process();

 private:
  //: The parameters of this process
  bpro1_parameters_sptr parameters_;
  
  //: Names of the storage classes used as input
  vcl_vector< vcl_string > input_names_;
  //: Names of the storage classes used as output
  vcl_vector< vcl_string > output_names_;

  
};

#endif // bpro1_process_h_
