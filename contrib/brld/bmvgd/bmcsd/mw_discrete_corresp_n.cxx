#include "mw_discrete_corresp_n.h"
#include <mw/mw_util.h>
#include <vbl/io/vbl_io_sparse_array_base.h>
#include <vsl/vsl_vector_io.h>

mw_discrete_corresp_n::
mw_discrete_corresp_n (const vcl_vector<unsigned> &npts)
  :
  n_(npts)
{
}

/* XXX work in progres...
mw_discrete_corresp_n::
  mw_discrete_corresp_n (const mw_discrete_corresp_3 &c3)
{
  n_.push_back(c3.n0());
  n_.push_back(c3.n1());
  n_.push_back(c3.n2());

  mw_ntuplet tup(3);

  vbl_sparse_array_3d<mw_match_attribute>::const_iterator p;
  for (p = c3.l_.begin(); p != c3.l_.end(); ++p) {
    tup[0] = p->first.first; 
    tup[1] = p->first.second; 
    tup[2] = p->first.third;
    l_.put(tup,mw_match_attribute())
  }
}
*/

void mw_discrete_corresp_n::
set_size(const vcl_vector<unsigned> &npts) 
{  
  n_ = npts;
  if (is_hashed()) {
    vcl_cerr << "Not supported: reuse structure after it is hashed\n";
    abort();
  }
}

vcl_ostream&  operator<<(vcl_ostream& s, const mw_discrete_corresp_n &c)
{
  s << "===========  mw_discrete_corresp_n ===========\n";
  s << "nviews: " << c.n_.size() << vcl_endl;
  s << "# of domain points: " << c.n_ << vcl_endl;

  unsigned  total_n_corresp= c.l_.count_nonempty();


//  unsigned  max_domain_elts=5;
//  unsigned  max_domain_elts=100;
  unsigned  max_domain_elts=20;

  unsigned n_finite=0, count =1;
  vbl_sparse_array_base<mw_match_attribute,mw_ntuplet>::const_iterator p;
  for (p = c.l_.begin(); p != c.l_.end(); ++p,++count) {
    if (count <= max_domain_elts) {
      s   << (*p).first
          << ": " << (*p).second << '\n';
    }

    if (vnl_math_isfinite(p->second.cost_))
      n_finite++;
  }

  if (count > max_domain_elts)
    vcl_cout << "...\n";

  s << "Total number of corresps: " << total_n_corresp << vcl_endl;
//  s << "Avg. number of corresps per domain point: " << (float)total_n_corresp/(float)c.corresp_.size() << vcl_endl;
  s << "Number of corresps with finite cost: " << n_finite << vcl_endl;
  s << "============================================\n";

  return s;
}

bool mw_discrete_corresp_n::
equal(const vbl_sparse_array_base<mw_match_attribute, mw_ntuplet> &l1, const vbl_sparse_array_base<mw_match_attribute, mw_ntuplet> &l2)
{
  //same number of non zero elements?
  if (l1.count_nonempty() != l2.count_nonempty())
    return false;
  else {
    //check every key/data pair, require same order too.
    vbl_sparse_array_base<mw_match_attribute,mw_ntuplet>::const_iterator s = l2.begin();
    vbl_sparse_array_base<mw_match_attribute,mw_ntuplet>::const_iterator r;
    //N.B. relies on sensible == operator for mw_match_attribute
    for (r = l1.begin(); r != l1.end(); ++r){
      if (!((*s).first == (*r).first) || !((*s).second == (*r).second))
        return false;
      s++;
    }
  }

  return true;
}

/*
void mw_discrete_corresp_n::
number_of_correct_triplets(unsigned &n_correct, unsigned &n_valid, const mw_discrete_corresp_n *gt) const
{

  vcl_vector<bool> p0s, p1s, p2s;

  gt->participating_points(p0s,p1s,p2s);


  n_valid   = 0;
  n_correct = 0;


  vbl_sparse_array_3d<mw_match_attribute>::const_iterator p;
  for (p = l_.begin(); p != l_.end(); ++p) {
    // if the triplet is finite 
    // and the first point has a ground-truth triplet
    // NOTE: ground-truth is generally assymetric for me. This means that, for
    // real data, I only define all possible triplets involving points of image 1. 
    //
    if (vnl_math_isfinite(p->second.cost_) && p0s[p->first.first]) {
      ++n_valid;
      if (gt->l_.fullp(p->first.first, p->first.second, p->first.third))
        ++n_correct;
    }
  }
}
*/


#if 0
void mw_discrete_corresp_n::
compare_and_print( const mw_discrete_corresp_n *gt) const
{
  assert(this->n0() == gt->n0());
  assert(this->n1() == gt->n1());
  assert(this->n2() == gt->n2());

  vcl_cout << "\n========== CORRESPONDENCE STATISTICS ==========\n"; 


  // # of correct triplets within all finite-valued triplets

//  unsigned  total_n_corresp= l_.count_nonempty();

  unsigned n_correct, n_valid;
  number_of_correct_triplets(n_correct, n_valid, gt);

  vcl_cout << "# of correct triplets: \t\n"
    << 100*(float)n_correct/(float)(n_valid) << "% ("  << n_correct << " out of " << n_valid << ")." << vcl_endl;

  /*
   * TODO: number of right triplets in ground truth that are not present in the
   * present correspondence structure.
   * 
   *
  unsigned n_w_gt;
  number_of_pts1_with_gt_among_any_candidates(n_w_gt,gt);

  vcl_cout << "# of points in image 1 having ground-truth among any candidates:\n\t" 
    << 100.0*(float)n_w_gt/(float)(size()) << "% ("  << n_w_gt << " out of " << size() << ")." << vcl_endl;

  vcl_cout << "Top 5 matches covers " << (float)(100*vcl_min(5*n_gt,total_nmatches_gt))/(float)total_nmatches_gt << "\% of the total candidate matches (" << vcl_min(5*n_gt,total_nmatches_gt) << " out of " << total_nmatches_gt << ").\n";


  unsigned nh_higher, nh_valid;
  percentage_of_matches_above_truth(nh_higher,nh_valid,gt);

  vcl_cout << "# Matches with cost higher than correct match:\n\t" 
    << 100.0*(float)nh_higher/(float)nh_valid<< "% ("  << nh_higher << " out of " << nh_valid << ")." << vcl_endl;
    */

}

//: Return all point id's that participate in some finite triplet.
//
// p0s[i] == true if i is part of some triplet (i,a,b) with finite cost
// p1s[i] == true if i is part of some triplet (a,b,i) with finite cost
// p2s[i] == true if i is part of some triplet (a,b,i) with finite cost
void mw_discrete_corresp_n::
participating_points(vcl_vector<bool> &p0s,vcl_vector<bool> &p1s,vcl_vector<bool> &p2s) const
{

  // resize
  p0s.resize(n0_,false);
  p1s.resize(n1_,false);
  p2s.resize(n2_,false);

  vbl_sparse_array_3d<mw_match_attribute>::const_iterator p;
  for (p = l_.begin(); p != l_.end(); ++p) {
    if (vnl_math_isfinite(p->second.cost_)) {
      p0s[p->first.first] = true;
      p1s[p->first.second] = true;
      p2s[p->first.third] = true;
    }
  }
}
#endif

// I/O ------------------------------------------------------------------

//: Return IO version number;
short 
mw_discrete_corresp_n::
version() const
{
  return 1;
}

//: Print an ascii summary to the stream
void 
mw_discrete_corresp_n::
print_summary(vcl_ostream &os) const
{
  os << *this;
}

//: Return a platform independent string identifying the class
vcl_string mw_discrete_corresp_n::
is_a() const 
{ 
  return "mw_discrete_corresp_n";
}

//: Binary save self to stream.
void 
mw_discrete_corresp_n::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os, n_); 

  vsl_b_write<mw_match_attribute, mw_ntuplet>(os, l_);
}

//: Binary load self from stream.
void 
mw_discrete_corresp_n::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1:
      vsl_b_read(is, n_);
      vsl_b_read<mw_match_attribute, mw_ntuplet>(is, l_);
    break;

    default:
        vcl_cerr << "I/O ERROR: mw_discrete_corresp_n::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

/*
void 
mw_discrete_corresp_n::
hash()
{
  if (is_hashed()) {
    vcl_cerr << "Re-hash operation not yet supported\n";
    abort();
  }

  hash_[0].resize(n0_);
  hash_[1].resize(n1_);
  hash_[2].resize(n2_);

  vbl_sparse_array_3d<mw_match_attribute>::const_iterator p;
  for (p = l_.begin(); p != l_.end(); ++p) {
    if (vnl_math_isfinite(p->second.cost_)) {
      unsigned const i = p->first.first;
      unsigned const j = p->first.second;
      unsigned const k = p->first.third;
      hash_[0][i].insert(triplet_uuu(i,j,k));
      hash_[1][j].insert(triplet_uuu(i,j,k));
      hash_[2][k].insert(triplet_uuu(i,j,k));
    }
  }
}
*/
