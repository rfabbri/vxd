// This is bmcsd_discrete_corresp.h
#ifndef bmcsd_discrete_corresp_h
#define bmcsd_discrete_corresp_h
//:
//\file
//\brief Data structure representing correspondence between two sets of objects
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 04/25/2005 06:41:03 PM EDT
// 
// Modifications:
// Wed Sep 30 02:12:23 EDT 2009   Ricardo Fabbri  Renamed things: s/point/object/g
//

#include <vcl_cassert.h>
#include <vcl_vector.h>
#include <vcl_list.h>
#include <vcl_algorithm.h>
#include <vsl/vsl_binary_io.h>

struct bmcsd_match_attribute {
public:
   bmcsd_match_attribute(bool isnull=false, double cost=0)
    :
     null_(isnull),
     cost_(cost)
   {
   }

   ~bmcsd_match_attribute() {}

  double cost() const { return cost_; }

  //: Equality test
  inline bool operator==(bmcsd_match_attribute const &that) const
  { 
    return (null_ == true && that.null_ == true) || (cost_ == that.cost_ && null_ == that.null_);
  }

  //: Cost comparison test
  inline bool operator<(bmcsd_match_attribute const &that) const
  { 
    return cost_ < that.cost_;
  }

  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const;

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

  vcl_string is_a() const;

  friend vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_match_attribute &c);

public:
   bool null_;
   double cost_;
   // add more stuff here such as enum representing reasons for this corresp.
   // Symbolic reasons to store for isnull_=true:
   //   - SELF_OCCLUSION
   //   - OUT_OF_VIEW
   //   - FEATURE_DETECTION_ERROR
};

//: Binary save vnl_my_class to stream.
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_match_attribute & v)
{
  v.b_write(os);
}

//: Binary load vnl_my_class from stream.
inline void vsl_b_read(vsl_b_istream &is, bmcsd_match_attribute & v)
{
  v.b_read(is);
}

inline void vsl_print_summary(vcl_ostream &os, const bmcsd_match_attribute &p)
{
  p.print_summary(os);
}

//: This is to be used within the bmcsd_discrete_corresp class;
struct bmcsd_attributed_object : public bmcsd_match_attribute {
public:
  bmcsd_attributed_object() { }

  bmcsd_attributed_object(unsigned obj_idx, bool isnull=false, double cost=0)
   :
    bmcsd_match_attribute(isnull, cost),
    obj_(obj_idx)
  {
  }

  ~bmcsd_attributed_object() {}
  
  //: the id of the stored object
  unsigned id() const { return obj_; }

  //: the id of the stored object
  unsigned obj() const { return obj_; }

  //: Equality test
  inline bool operator==(bmcsd_attributed_object const &that) const
  { 
    return (null_ == true && that.null_ == true) || (obj_ == that.obj_  && cost_ == that.cost_ && null_ == that.null_);
  }

  //: Cost comparison test
  inline bool operator<(bmcsd_attributed_object const &that) const
  { 
    return cost_ < that.cost_;
  }


  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const;
  
  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

  friend vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_attributed_object &c);

  vcl_string is_a() const;

public:
   unsigned obj_; //:< objects are represented by some arbitrary numbering
};

class bmcsd_attributed_object_eq : public vcl_unary_function<bmcsd_attributed_object_eq, bool> {
  unsigned p_;
public:
  explicit bmcsd_attributed_object_eq(const unsigned pp) : p_(pp) { }; 
  bool operator() (const bmcsd_attributed_object &e) const { return e.obj_ == p_; }
};

//: Binary save vnl_my_class to stream.
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_attributed_object & v)
{
  v.b_write(os);
}

//: Binary load vnl_my_class from stream.
inline void vsl_b_read(vsl_b_istream &is, bmcsd_attributed_object & v)
{
  v.b_read(is);
}

inline void vsl_print_summary(vcl_ostream &os, const bmcsd_attributed_object &p)
{
  p.print_summary(os);
}


//: We support many-to-one, one-to-many, and even zero-to-many and many-to-zero
// Access is directly on datastructure, since its kinda low-level
//
// The object sets are indexed in the same way as in the initialization object
// vectors.
//
// The representation for an object is an id (unsigned int). Therefore, objects
// can be anything having an id. Each object can be a single point, or a SIFT
// point, or a curve fragment. Anything goes.
//
struct bmcsd_discrete_corresp {
public:
  typedef vcl_list< bmcsd_attributed_object > one_corresp_list;
  typedef vcl_list< bmcsd_attributed_object >::iterator one_corresp_list_iter;
  typedef vcl_list< bmcsd_attributed_object >::const_iterator one_corresp_list_const_iter;
  typedef vcl_vector < vcl_list< bmcsd_attributed_object > > corresp_data;

  bmcsd_discrete_corresp(unsigned num_objs0, unsigned num_objs1) 
     :
     corresp_(num_objs0+1),
     num_counterdomain_(num_objs1+1),
     cksum_(num_objs0+2*num_objs1) // arbitrary hash.
  { }
  bmcsd_discrete_corresp() {}
  ~bmcsd_discrete_corresp() {}

  void set_size(unsigned num_objs0, unsigned num_objs1) 
  { corresp_.resize(num_objs0+1); num_counterdomain_ = num_objs1+1; }

  //: Set a checksum for the objects in the domain and counter-domain. This is
  // to ensure this correspondence is for the objects it was intended for.
  void set_checksum(unsigned long long c) { cksum_ = c;}
  unsigned long checksum() const {return cksum_; }

  //: adds object, testing if is unique, and also resize corresp. list if needed
  bool add_unique(const bmcsd_attributed_object &e, unsigned i, 
      vcl_list<bmcsd_attributed_object>::iterator *itr);

  void threshold_by_cost(double cost);
  void threshold_by_cost_lteq(double cost);
  void sort();

  //: \param[in] maxmin : if true, keep only max cost. if false, keep only min
  // cost.
  void keep_only_extreme_cost(bool keep_max)
  {
    sort();
    for (unsigned i=0; i < n0(); ++i) {
      if (corresp_[i].empty())
        continue;
      bmcsd_attributed_object attr;
      if (keep_max)
        attr = corresp_[i].back();
      else
        attr = corresp_[i].front();
      corresp_[i].clear();
      corresp_[i].push_back(attr);
    }
  }

  //: Use lowe-style disambiguation, i.e.,
  // keep only the match having max #votes,
  // if this #votes is better than the ratio*#votes of second best.
  //
  // If there is no second best to compare, demand that the match be way beyond
  // lonely_threshold.
  //
  // Extremes behavior is as follows:
  //
  // With ratio == 0, the behavior is that of keeping the max cost.
  //
  // With ratio != 0 and: 
  //    - lonely_threshold == infinity, the behavior is to never include loners
  //    - lonely_threshold == 0, the behavior is to always include loners
  //
  // With ratio == infinity and lonely_threshold != infinity, the behavior is to
  // never include correspondences having a second best, no matter how bad the
  // second best is.
  //
  // This assumes there are no correspondences with 0 or Inf cost.
  void keep_only_unambiguous_max(double ratio=1.5, double lonely_threshold = 5.0);

  //: returns the total number of correspondences.
  unsigned num_corresps() const
  {
    unsigned sum = 0;
    for (unsigned i=0; i < n0(); ++i) {
      sum += corresp_[i].size();
    }
    return sum;
  }
  const one_corresp_list & operator[](unsigned i) const { assert (i < n0()); return corresp_[i]; }
  one_corresp_list & operator[](unsigned i) { assert (i < n0()); return corresp_[i]; }

  // todo: add as heap / sorted insert

  // Functions to be moved to algo ----------------------------------------

  vcl_list<bmcsd_attributed_object>::const_iterator 
  find_right_corresp_mincost(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const;

  void 
  percentage_of_matches_above_truth(unsigned &n, unsigned &n_valid, const bmcsd_discrete_corresp *gt) const;

  void
  number_of_pts1_with_gt_among_any_candidates(unsigned &n_w_gt, const bmcsd_discrete_corresp *gt) const;

  bool is_gt_among_top5(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const;

  //: \todo functional access

  friend vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_discrete_corresp &c);

  //: Equality test
  inline bool operator==(bmcsd_discrete_corresp const &that) const
  { return corresp_ == that.corresp_; }

  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const;

  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

  //: Return a platform independent string identifying the class
  vcl_string is_a() const;

  //: # objects in view 1; returns (unsigned) -1  in case nothing was initialized yet
  unsigned n_objects_view_0() const {return corresp_.size() - 1; }
  unsigned n_objects_view_1() const {return n1() - 1; }
  unsigned n0() const { return corresp_.size(); }
  unsigned n1() const { return num_counterdomain_;}

  unsigned size() const {return n_objects_view_0(); }
  bool is_empty() const {return corresp_.size() == 0; }

  // returns the number of objects (in image 1) having no correspondence.
  // If the dummy object has no correspondence, it also counts.
  unsigned count_empty() const { 
    return vcl_count_if(corresp_.begin(), corresp_.end(), 
                        vcl_mem_fun_ref(&one_corresp_list::empty )); 
  }

  void compare_and_print( const bmcsd_discrete_corresp *gt) const;

public:
   corresp_data corresp_;
private:
   unsigned num_counterdomain_;
   unsigned long cksum_;
};

//: Binary save vnl_my_class to stream.
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_discrete_corresp & v)
{
  v.b_write(os);
}

//: Binary load vnl_my_class from stream.
inline void vsl_b_read(vsl_b_istream &is, bmcsd_discrete_corresp & v)
{
  v.b_read(is);
}

#endif // bmcsd_discrete_corresp_h
