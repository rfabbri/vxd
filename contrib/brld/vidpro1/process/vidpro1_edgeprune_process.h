// This is vidpro1_edgeprune_process_h_
#ifndef vidpro1_edgeprune_process_h_
#define vidpro1_edgeprune_process_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Prune vtol or vsol contours (e.g. edge contours)
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 05/13/04
//
// \verbatim
//  Modifications
//  Apr 23 2007 Ricardo Fabbri - added support for vsol input 
// \endverbatim
//--------------------------------------------------------------------------------
#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage.h>
#include <vidpro1/storage/vidpro1_vsol2D_storage_sptr.h>

#include <vidpro1/storage/vidpro1_vtol_storage.h>
#include <vidpro1/storage/vidpro1_vtol_storage_sptr.h>

#include <vcl_vector.h>

//: Process that takes vtol or vsol storage class as input and outputs a vsol2D storage class.
//  Edge curves/chains with length shorter than a given threshold can be pruned during conversion if required.
//  This process can be used for vtol -> vsol conversion, or for duplicating vsol curve storages. In case the pruning option is
//  unchecked in the dialog, the input is merely copied to the output.
//
class vidpro1_edgeprune_process : public bpro1_process
{
public:
  vidpro1_edgeprune_process();
  ~vidpro1_edgeprune_process() {}

  //: Clone the process
  virtual bpro1_process* clone() const;

  vcl_string name() {
    return "Prune Edges";
  }

  vcl_vector< vcl_string > get_input_type();
  vcl_vector< vcl_string > get_output_type();

  int input_frames() {
    return 1;
  }
  int output_frames() {
    return 1;
  }

  bool execute();
  bool finish() {
    return true;
  }

protected:
};

#endif // vidpro1_edgeprune_process_h_
