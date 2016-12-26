// This is bmcsd_discrete_corresp_3.h
#ifndef bmcsd_discrete_corresp_3_h
#define bmcsd_discrete_corresp_3_h
//:
//\file
//\brief Class for storing trinocular correspondences.
//\author Ricardo Fabbri (rfabbri), Brown University  (@gmail.com)
//\date 09/22/06 11:31:26 EDT
//


#include <vbl/vbl_sparse_array_3d.h>
#include <mw/bmcsd_discrete_corresp.h>
#include <vcl_set.h>
#include <vcl_algorithm.h>
#include <assert.h>

class bmcsd_discrete_corresp_n; // just for conversion constructor

typedef vbl_triple<unsigned,unsigned,unsigned> triplet_uuu;

//: 
// In the future, we might move stuff to a base class and this might be a
// specialization.
class bmcsd_discrete_corresp_3 {
public:
  bmcsd_discrete_corresp_3 (unsigned npts0, unsigned npts1, unsigned npts2);
  
  bmcsd_discrete_corresp_3 (const bmcsd_discrete_corresp_n &cn);

  bmcsd_discrete_corresp_3(): hash_(3) {}

  void set_size(unsigned npts0, unsigned npts1, unsigned npts2);

  ~bmcsd_discrete_corresp_3() {}

  unsigned n0() const { return n0_; }
  unsigned n1() const { return n1_; }
  unsigned n2() const { return n2_; }

  //: Hash/organize the finite-cost triplets for fast query. Drawback: uses more memory.
  // OBS: user has to rehash after insertions/removals are done.
  void hash();

  void
  participating_points(vcl_vector<bool> &p0s,vcl_vector<bool> &p1s,vcl_vector<bool> &p2s) const;

  void 
  number_of_correct_triplets(
      unsigned &n_correct, unsigned &n_valid, const bmcsd_discrete_corresp_3 *gt) const;

  unsigned size() const {return l_.count_nonempty();}

  bool is_empty() const {return size() == 0; }

  bool is_hashed() const {return hash_[0].size() != 0;}

  //: return all triplets containing point "point_id" of image "img_id"
  //
  const vcl_set<triplet_uuu> &triplets(unsigned img_id, unsigned point_id) const
    { assert(is_hashed()); return hash_[img_id][point_id]; }

  //: return all triplets containing point with id "p1" of image "img1" and
  // point id "p2" of image "img2"
  void triplets(
      unsigned img1, unsigned p1, unsigned img2, unsigned p2, vcl_set<triplet_uuu> &t) const
    { 
      assert(is_hashed());
      const vcl_set<triplet_uuu> &s1 = triplets(img1,p1);
      const vcl_set<triplet_uuu> &s2 = triplets(img2,p2);

      vcl_set_intersection(s1.begin(),s1.end(), s2.begin(), s2.end(), inserter(t, t.begin()));
    }

  bmcsd_match_attribute & operator() (unsigned i, unsigned j, unsigned k)
    {return l_(i,j,k);}

  const bmcsd_match_attribute & operator() (unsigned i, unsigned j, unsigned k) const
    {return l_(i,j,k);}

  /*
//  void sort();

  // Functions to be moved to algo ----------------------------------------

  vcl_list<bmcsd_attributed_point>::const_iterator 
  find_right_corresp_mincost(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const;

  void 
  percentage_of_matches_above_truth(unsigned &n, unsigned &n_valid, const bmcsd_discrete_corresp *gt) const;

  void
  number_of_pts1_with_gt_among_any_candidates(unsigned &n_w_gt, const bmcsd_discrete_corresp *gt) const;

  bool 
  is_gt_among_top5(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const;

  //: \todo functional access

  */

  //: Equality test
  inline bool operator==(bmcsd_discrete_corresp_3 const &that) const
  { return equal(l_, that.l_) && n0_ == that.n0_ && n1_ == that.n1_ && n2_ == that.n2_; }

  // I/O ------------------------------------------------------------------

  //: Binary save self to stream.
  void b_write(vsl_b_ostream &os) const;

  //: Binary load self from stream.
  void b_read(vsl_b_istream &is);

  //: Return IO version number;
  short version() const;

  //: Return a platform independent string identifying the class
  vcl_string is_a() const;

  void compare_and_print( const bmcsd_discrete_corresp_3 *gt) const;

  friend vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_discrete_corresp_3 &c);
  
  //: Print an ascii summary to the stream
  void print_summary(vcl_ostream &os) const;

public:

  //: Corresp stored as sparse array for now. However, this sparse array doesn't
  // seem very good if you want to traverse only non-null elements row-wise,
  // column-wise, depth-wise etc.
  vbl_sparse_array_3d<bmcsd_match_attribute> l_;
  unsigned n0_;
  unsigned n1_;
  unsigned n2_;

  //: hash_[0][i] == set of triplets (i,*,*)
  //  hash_[1][i] == set of triplets (*,i,*)
  //  hash_[2][i] == set of triplets (*,*,i)
  vcl_vector<vcl_vector<vcl_set<triplet_uuu> > > hash_;
protected:
  static bool equal(vbl_sparse_array_3d<bmcsd_match_attribute> l1, vbl_sparse_array_3d<bmcsd_match_attribute> l2);
};

//: Binary save vnl_my_class to stream.
inline void vsl_b_write(vsl_b_ostream &os, const bmcsd_discrete_corresp_3 & v)
{
  v.b_write(os);
}

//: Binary load vnl_my_class from stream.
inline void vsl_b_read(vsl_b_istream &is, bmcsd_discrete_corresp_3 & v)
{
  v.b_read(is);
}

inline vcl_ostream&  operator<<(vcl_ostream& s, const triplet_uuu  &x)
{
  s << "(" << x.first << "," << x.second <<"," << x.third << ")";
  return s;
}

#endif // bmcsd_discrete_corresp_3_h
