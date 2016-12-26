// This is mw_discrete_corresp_n.h
#ifndef mw_discrete_corresp_n_h
#define mw_discrete_corresp_n_h
//:
//\file
//\brief Class for storing n-view correspondences.
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 
//

#include <vbl/vbl_sparse_array_base.h>
#include <vcl_set.h>
#include <vcl_algorithm.h>
#include <vcl_vector.h>
#include <mw/mw_discrete_corresp.h>

typedef vcl_vector<unsigned> mw_ntuplet;

//: A class for storing and querying n-view correspondences.
//
// \todo In the future, we might move stuff to a base class and this might be a
// specialization.
class mw_discrete_corresp_n {
public:
  mw_discrete_corresp_n (const vcl_vector<unsigned> &npts);
  
  /* XXX work in progress...
  mw_discrete_corresp_n (const mw_discrete_corresp_3 &c3);
  */

  mw_discrete_corresp_n() {}

  void set_size(const vcl_vector<unsigned> &npts);

  ~mw_discrete_corresp_n() {}

  const vcl_vector<unsigned> &n() const { return n_; }

  //: Hash/organize the finite-cost triplets for fast query. Drawback: uses more memory.
  // OBS: user has to rehash after insertions/removals are done.
  void hash() { abort();}

  //  void
  //  participating_points(vcl_vector<bool> &p0s,vcl_vector<bool> &p1s,vcl_vector<bool> &p2s) const;

  //  void 
  //  number_of_correct_triplets(
  //    unsigned &n_correct, unsigned &n_valid, const mw_discrete_corresp_n *gt) const;

  unsigned size() const {return l_.count_nonempty();}

  bool is_empty() const {return size() == 0; }

  bool is_hashed() const {return false;}//{return hash_[0].size() != 0;}

  //: return all n-tuplets containing point "point_id" of image "img_id"
  //
  //  TODO const vcl_set<mw_ntuplets> &ntuplets(unsigned img_id, unsigned point_id) const
  //    { assert(is_hashed()); return hash_[img_id][point_id]; }

  //: return all triplets containing point with id "p1" of image "img1" and
  // point id "p2" of image "img2"
  //  void triplets(unsigned img1, unsigned p1, unsigned img2, unsigned p2, vcl_set<triplet_uuu> &t) const
  //    { 
  //      assert(is_hashed());
  //      const vcl_set<triplet_uuu> &s1 = triplets(img1,p1);
  //      const vcl_set<triplet_uuu> &s2 = triplets(img2,p2);

  //      vcl_set_intersection(s1.begin(),s1.end(), s2.begin(), s2.end(), inserter(t, t.begin()));
  //    }

  //  mw_match_attribute & operator() (unsigned i, unsigned j, unsigned k)
  //    {return l_(i,j,k);}

  mw_match_attribute & operator() (const mw_ntuplet &tpl)
    {return l_(tpl);}

  /*
  //  void sort();

  // Functions to be moved to algo ----------------------------------------

  vcl_list<mw_attributed_point>::const_iterator 
  find_right_corresp_mincost(unsigned p1_idx, const mw_discrete_corresp *gt) const;

  void 
  percentage_of_matches_above_truth(
  unsigned &n, unsigned &n_valid, const mw_discrete_corresp *gt) const;

  void
  number_of_pts1_with_gt_among_any_candidates(
  unsigned &n_w_gt, const mw_discrete_corresp *gt) const;

  bool 
  is_gt_among_top5(unsigned p1_idx, const mw_discrete_corresp *gt) const;

  //: \todo functional access

  */

  //: Equality test
  inline bool operator==(mw_discrete_corresp_n const &that) const
  { return equal(l_, that.l_) && n_ == that.n_; }

  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const;

  //: Return a platform independent string identifying the class
  vcl_string is_a() const;

  /*
  void 
  compare_and_print( const mw_discrete_corresp_n *gt) const;
  */

  //  friend class mw_discrete_corresp_algo;
  friend vcl_ostream&  operator<<(vcl_ostream& s, const mw_discrete_corresp_n &c);
  
  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

public:

  //: Corresp stored as sparse array for now. However, this sparse array doesn't
  // seem very good if you want to traverse only non-null elements row-wise,
  // column-wise, depth-wise etc.
  vbl_sparse_array_base<mw_match_attribute, mw_ntuplet> l_;

  //: n_[i] == number of points in view i
  vcl_vector<unsigned> n_;

  //: hash_[0][i] == set of triplets (i,*,*)
  //  hash_[1][i] == set of triplets (*,i,*)
  //  hash_[2][i] == set of triplets (*,*,i)
//  vcl_vector<vcl_vector<vcl_set<mw_ntuplet> > > hash_;
protected:
  static bool equal(
      const vbl_sparse_array_base<mw_match_attribute, mw_ntuplet> &l1, 
      const vbl_sparse_array_base<mw_match_attribute, mw_ntuplet> &l2);
};

//: Binary save vnl_my_class to stream.
inline void vsl_b_write(vsl_b_ostream &os, const mw_discrete_corresp_n & v)
{
  v.b_write(os);
}

//: Binary load vnl_my_class from stream.
inline void vsl_b_read(vsl_b_istream &is, mw_discrete_corresp_n & v)
{
  v.b_read(is);
}

inline vcl_ostream&  operator<<(vcl_ostream& s, const mw_ntuplet &x)
{
  s << "(" ;
  for (unsigned i=0; i < x.size(); ++i) {
    s << x[i];
    if (i+1 == x.size())
      s << ")";
    else
      s << ",";
  }
  return s;
}

#endif // mw_discrete_corresp_n_h
