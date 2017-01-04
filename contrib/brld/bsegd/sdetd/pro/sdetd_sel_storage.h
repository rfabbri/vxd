// This is brcv/seg/dbdet/pro/dbdet_sel_storage.h
#ifndef dbdet_sel_storage_h_
#define dbdet_sel_storage_h_

//:
// \file
// \brief Storage class for various edge linking data structures
//
//  It contains the following things:
//      a) An edge map containing a set of edgels bucketed into neighborhoods.
//
//      b) The link graph structure
//
//      c) The data structure to hold the final linked image contours.
//
// \author Amir Tamrakar
// \date 03/28/06
//
//
// \verbatim
//  Modifications
//
//   Amir Tamrakar  Nov 07,  Instead of storing the linking algorithm which contained 
//                           all the relevant data structures, I've decided to store the
//                           data structures in this storage class until a better data structure
//                           to organize these elements can be designed. 
//                           This will, at the least, clean up the linking algorithm which 
//                           has become quite messy and unmanageable.
//
// \endverbatim

#include <bpro1/bpro1_storage.h>
#include "dbdet_sel_storage_sptr.h"

#include <vcl_vector.h>
#include <vbl/vbl_array_2d.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector_fixed.h>

#include <dbdet/edge/dbdet_edgemap_sptr.h>
#include <dbdet/sel/dbdet_curvelet_map.h>
#include <dbdet/sel/dbdet_edgel_link_graph.h>
#include <dbdet/sel/dbdet_curve_fragment_graph.h>

//: Storage class for various edge linking data structures
//  One idiosyncracy of this design: Since the various structures are
//  instantiated in this class, the storage class will need to be instantiated
//  before any algorithm can be called
class dbdet_sel_storage : public bpro1_storage 
{
public:

  //: Constructor
  dbdet_sel_storage() : EM_(0) {}

  //: Destructor
  virtual ~dbdet_sel_storage(){}

  //: Returns the type string "sel"
  virtual vcl_string type() const { return "sel"; }

  //: Return IO version number;
  short version() const;

  //: Create a copy of the object on the heap.
  // The caller is responsible for deletion
  virtual bpro1_storage* clone() const;
  
  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbdet_sel_storage"; }

  // Data Access Functions
  dbdet_edgemap_sptr EM() { return EM_; }
  dbdet_curvelet_map& CM() {return CM_; }
  dbdet_edgel_link_graph& ELG() { return ELG_; }
  dbdet_curve_fragment_graph&  CFG() { return CFG_; }
  vbl_array_2d<bool>& EULM() { return EULM_; }
  vcl_list<dbdet_edgel_chain_list>& c_groups() {return c_groups_; }
  vnl_matrix<double> color_mat(){return color_mat_;}
  vnl_vector_fixed<float, 3> prune_color(){return prune_color_;}
  dbdet_edgel_chain_list& prune_frags(){return prune_frags_;}

  void set_EM(dbdet_edgemap_sptr EM) { EM_ = EM; }
  void set_color_mat(vnl_matrix<double> color_mat) {color_mat_=color_mat;}
  void set_prune_color(float r, float g, float b) {prune_color_[0] =r; prune_color_[1]=g; prune_color_[2]=b;}
  dbdet_edgel_chain_list& all_groups_frags();
  //void set_CFG(dbdet_curve_fragment_graph CFG) {CFG_ = CFG;} //By Yuliang Guo

private:

  dbdet_edgemap_sptr EM_; ///< The edgemap (EM) (without this structure none of the other structures here are defined)
  dbdet_curvelet_map CM_; ///< The curvelet map (CM) 
  dbdet_edgel_link_graph ELG_; ///< The edge link graph (ELG)
  dbdet_curve_fragment_graph CFG_; ///< The curve fragment graph (CFG)
  vbl_array_2d<bool> EULM_; ///< The edge unlinked map (EULM) (temp structure for generic linker)
  //  Don't know if this ought to hidden away inside the generic linker class
  //  [[ The matrix of flags to keep track of the unlinked edgels (for tracing) ]]
  vcl_list<dbdet_edgel_chain_list> c_groups_;
  vnl_matrix<double> color_mat_;
  dbdet_edgel_chain_list prune_frags_;
  vnl_vector_fixed<float,3> prune_color_;
  dbdet_edgel_chain_list all_groups_frags_;
};


//: Create a smart-pointer to a dbdet_sel_storage.
struct dbdet_sel_storage_new : public dbdet_sel_storage_sptr
{
  typedef dbdet_sel_storage_sptr base;

  //: Constructor - creates a default dbdet_sel_storage_sptr.
  dbdet_sel_storage_new() : base(new dbdet_sel_storage()) { }

};


#endif //dbdet_sel_storage_h_
