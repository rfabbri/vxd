#include "bmcsd_discrete_corresp.h"
#include <vsl/vsl_vector_io.h>
#include <vsl/vsl_list_io.h>
#include <vcl_algorithm.h>
#include <vcl_set.h>
#include <dborl/dborl_evaluation.h>
#include <mw/bmcsd_util.h>

bool  bmcsd_discrete_corresp::
add_unique(const bmcsd_attributed_object &e, unsigned i, 
    vcl_list<bmcsd_attributed_object>::iterator *itr
    )
{
  vcl_list<bmcsd_attributed_object>::iterator 
    result = find(corresp_[i].begin(), corresp_[i].end(), e); 

  if (result != corresp_[i].end()) {
    *itr = result;
    return false;
  }

  if (i >= corresp_.size()) {
    vcl_cout << "bmcsd_discrete_corresp:: resizing list\n";
    corresp_.resize(i+1);
  }

  corresp_[i].push_back(e);
  *itr = --corresp_[i].end();

  return true;
}

vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_match_attribute &c)
{

  c.print_summary(s);

  return s;
}

//: Print an ascii summary to the stream
void bmcsd_match_attribute::
print_summary(vcl_ostream &os) const
{
  os << "[" << is_a() << ": " << 
     "is_null: " << null_ <<  "  cost: " << cost_
     << "]";
}

//: Return a platform independent string identifying the class
vcl_string bmcsd_match_attribute::
is_a() const 
{return "bmcsd_match_attribute";}

vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_attributed_object &c)
{
  c.print_summary(s);
  return s;
}

vcl_ostream&  operator<<(vcl_ostream& s, const bmcsd_discrete_corresp &c)
{
  s << "===========  bmcsd_discrete_corresp ===========\n";
  s << "# of Domain objects = num_objs0 + 1 = " << c.corresp_.size() << vcl_endl;

  unsigned  max_domain_elts=20, count_nonzero=0;
  unsigned  i;
  unsigned  total_n_corresp=0;
  for (i=0; i < c.corresp_.size(); ++i) {
    if (i < max_domain_elts) {
      s << "obj_id: " << i << " #corresp: " << c.corresp_[i].size() << vcl_endl;

      vcl_list< bmcsd_attributed_object >::const_iterator  itr;
      const unsigned  max_elts=3; 
      unsigned  nn=1;
      for (itr = c.corresp_[i].begin(); itr != c.corresp_[i].end(); ++itr, ++nn) {
        if (nn > max_elts)
          break;
        s << "         "; 
        itr->print_summary(s);
        s << vcl_endl;
      }
    }
    if (!c.corresp_[i].empty())
      ++count_nonzero;
    total_n_corresp += c.corresp_[i].size();
  }


  unsigned n_finite=0;
  for (i=0; i < c.corresp_.size(); ++i) {
      vcl_list< bmcsd_attributed_object >::const_iterator  itr;
      for (itr = c.corresp_[i].begin(); itr != c.corresp_[i].end(); ++itr) {
        if (vnl_math_isfinite(itr->cost_))
          n_finite++;
      }
  }

  if (i >= max_domain_elts)
    vcl_cout << "...\n";

  s << "Total domain objects with some correspondents: " << count_nonzero << vcl_endl;
  s << "Total number of corresps: " << total_n_corresp << vcl_endl;
  s << "Avg. number of corresps per domain object: " << (float)total_n_corresp/(float)c.corresp_.size() << vcl_endl;
  s << "Number of corresps with finite cost: " << n_finite << vcl_endl;
  s << "============================================\n";

  return s;
}


// I/O ------------------------------------------------------------------

//: Binary save self to stream.
void 
bmcsd_match_attribute::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os,null_);
  vsl_b_write(os,cost_);
}

//: Binary load self from stream.
void 
bmcsd_match_attribute::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1:
      vsl_b_read(is, null_);
      vsl_b_read(is, cost_);

    break;

    default:
        vcl_cerr << "I/O ERROR: bmcsd_match_attribute::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Return IO version number;
short 
bmcsd_match_attribute::
version() const
{
  return 1;
}


//: Binary save self to stream.
void 
bmcsd_attributed_object::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os,obj_);
  vsl_b_write(os,null_);
  vsl_b_write(os,cost_);
}

//: Binary load self from stream.
void 
bmcsd_attributed_object::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1:
      vsl_b_read(is, obj_);
      vsl_b_read(is, null_);
      vsl_b_read(is, cost_);

    break;

    default:
        vcl_cerr << "I/O ERROR: bmcsd_attributed_object::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Return IO version number;
short 
bmcsd_attributed_object::
version() const
{
  return 1;
}

//: Print an ascii summary to the stream
void 
bmcsd_attributed_object::
print_summary(vcl_ostream &os) const
{
  os << "[" << is_a() << ": " << 
     "is_null: " << null_ << "  object_id: " << obj_ << "  cost: " << cost_
     << "]";
}

//: Return a platform independent string identifying the class
vcl_string 
bmcsd_attributed_object::
is_a() const 
{return "bmcsd_attributed_object";}


//: Binary save self to stream.
void 
bmcsd_discrete_corresp::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os, cksum_);
  unsigned n = n0();
  vsl_b_write(os, n);
  n = n1();
  vsl_b_write(os, n);
  for (unsigned i=0; i < corresp_.size(); ++i) {
    vsl_b_write<bmcsd_attributed_object>(os, corresp_[i]);
  }
}

//: Binary load self from stream.
void 
bmcsd_discrete_corresp::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1: {
      vcl_cerr << "Warning: this is an old correspondence file...\n"
       << "...it is incompatible with some newer tools\n";
      unsigned size;
      vsl_b_read(is, size);
      corresp_.resize(size);
      for (unsigned i=0; i < size; ++i)
        vsl_b_read<bmcsd_attributed_object>(is, corresp_[i]);
    } break;

    case 2: {
      unsigned n0, n1;
      vsl_b_read(is, n0);
      vsl_b_read(is, n1);
      assert (n0 != 0 && n1 != 0);
      set_size(n0-1, n1-1);
      for (unsigned i=0; i < n0; ++i)
        vsl_b_read<bmcsd_attributed_object>(is, corresp_[i]);
    } break;
    case 3: {
      unsigned long cksum;
      vsl_b_read(is, cksum);
      set_checksum(cksum);
      unsigned n0, n1;
      vsl_b_read(is, n0);
      vsl_b_read(is, n1);
      assert (n0 != 0 && n1 != 0);
      set_size(n0-1, n1-1);
      for (unsigned i=0; i < n0; ++i)
        vsl_b_read<bmcsd_attributed_object>(is, corresp_[i]);
    } break;

    default:
        vcl_cerr << "I/O ERROR: bmcsd_discrete_corresp::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Return IO version number;
short 
bmcsd_discrete_corresp::
version() const
{
  return 3;
}

//: Print an ascii summary to the stream
void 
bmcsd_discrete_corresp::
print_summary(vcl_ostream &os) const
{
  os << *this;
}

//: Return a platform independent string identifying the class
vcl_string 
bmcsd_discrete_corresp::
is_a() const 
{return "bmcsd_discrete_corresp";}

//: remove every element whose cost is above threshold
//\todo test
void bmcsd_discrete_corresp::
threshold_by_cost(double max_cost)
{
  for (unsigned i=0; i < corresp_.size(); ++i) {
    vcl_list< bmcsd_attributed_object >::iterator  itr;
    for (itr = corresp_[i].begin(); itr != corresp_[i].end();) {
      if (itr->cost_ > max_cost)
        itr = corresp_[i].erase(itr);
      else
        ++itr;
    }
  }
}

//: remove every element whose cost is below or equal to threshold
void bmcsd_discrete_corresp::
threshold_by_cost_lteq(double min_cost)
{
  for (unsigned i=0; i < corresp_.size(); ++i) {
    vcl_list< bmcsd_attributed_object >::iterator  itr;
    for (itr = corresp_[i].begin(); itr != corresp_[i].end();) {
      if (itr->cost_ <= min_cost)
        itr = corresp_[i].erase(itr);
      else
        ++itr;
    }
  }
}

void bmcsd_discrete_corresp::
sort()
{
  for (unsigned i=0; i < corresp_.size(); ++i) {
    corresp_[i].sort();
  }
}

//: \param[in] gt : ground-truth correspondences
void bmcsd_discrete_corresp::
compare_and_print( const bmcsd_discrete_corresp *gt) const
{
  assert(this->size() == gt->size());

  vcl_cout << "\n========== CORRESPONDENCE STATISTICS ==========\n"; 


  unsigned n_w_gt;
  number_of_pts1_with_gt_among_any_candidates(n_w_gt,gt);

  vcl_cout << "# of objects in image 1 having ground-truth among any candidates:\n\t" 
    << 100.0*(float)n_w_gt/(float)(size()) << "% ("  << n_w_gt << " out of " << size() << ")." << vcl_endl;

  unsigned n5=0, 
           n_top_50=0, 
           n_gt=0, 
           total_nmatches_gt=0;//:< total number of candidates with non-empty ground-truth 
  for (unsigned i=0; i < this->corresp_.size(); ++i) {
    // for each top 5 match:

    if (gt->corresp_[i].empty()) {
      continue;
    } else {
      bool found_gt_in_top_5=false,
           found_gt_in_top_half=false, has_finite=false;
      unsigned ncorr_atual = 0;
      double corresp_i_size = (double)(corresp_[i].size());
      vcl_list< bmcsd_attributed_object >::const_iterator  itr;
      for (itr = corresp_[i].begin(); itr != corresp_[i].end(); ++itr) {
        if (!vnl_math_isfinite(itr->cost_))
          continue;
        else
          has_finite = true;

        ++total_nmatches_gt;
        ++ncorr_atual;


        vcl_list<bmcsd_attributed_object>::const_iterator 
          result = find_if(gt->corresp_[i].begin(), gt->corresp_[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 

        if (result != gt->corresp_[i].end()) {
          if (vnl_math_isfinite(result->cost_)) {
            // found
            if (ncorr_atual <= 5)
              found_gt_in_top_5 = true;
            if (ncorr_atual <= ceil(corresp_i_size/2.0))
              found_gt_in_top_half = true;
          } else {
            vcl_cout << "Warning: non-finite cost in ground-truth corresp\n";
          }
        }
      }
      if (has_finite)
        n_gt++;
      if (found_gt_in_top_half)
        n_top_50++;
      if (found_gt_in_top_5)
        n5++;
    }
  }

  vcl_cout << "# objects in image 1 having correct candidates ranked among top 50% matches:\n\t" 
    << 100.0*(float)n_top_50/(float)n_gt << "% ("  << n_top_50 << " out of " << n_gt  << ")." << vcl_endl;

  vcl_cout << "# objects in image 1 having correct candidates in top 5 matches:\n\t" 
    << 100.0*(float)n5/(float)n_gt << "% ("  << n5 << " out of " << n_gt  << ")." << vcl_endl;

  vcl_cout << "Top 5 matches covers " << (float)(100*vcl_min(5*n_gt,total_nmatches_gt))/(float)total_nmatches_gt << "\% of the total candidate matches (" << vcl_min(5*n_gt,total_nmatches_gt) << " out of " << total_nmatches_gt << ").\n";


  unsigned nh_higher, nh_valid;
  percentage_of_matches_above_truth(nh_higher,nh_valid,gt);

  vcl_cout << "# Matches with cost higher than correct match:\n\t" 
    << 100.0*(float)nh_higher/(float)nh_valid<< "% ("  << nh_higher << " out of " << nh_valid << ")." << vcl_endl;

  // - percentage of objects in image 1 having correct candidates in top 5 first matches

  // - percentage of objects in image 1 having correct candidates in top 10 first
  // matches
  
  // - any other statistics?


}

//: Computes the percentage of all *wrong* epipolar candidates having energy above the
// ground-truth.
//
// More precisely, we want "How many of the *wrong* correspondences passing a
// test have cost greater than the true correspondences? By passing a test it
// means it has finite cost. 
//
// TODO: test further
void bmcsd_discrete_corresp::
percentage_of_matches_above_truth(unsigned &n, unsigned &n_valid, const bmcsd_discrete_corresp *gt) const
{
//for each object p1 in image 1

  n = n_valid = 0;
  for (unsigned p1_idx=0; p1_idx < this->corresp_.size(); ++p1_idx) {
    vcl_list<bmcsd_attributed_object>::const_iterator  result;
    result = this->find_right_corresp_mincost(p1_idx,gt);
    if (result != this->corresp_[p1_idx].end()) { // found

      //  For each wrong correspondence
      //    If its cost is greater than result->cost_, n++;

      vcl_list< bmcsd_attributed_object >::const_iterator  itr = corresp_[p1_idx].begin();
      for (; itr != corresp_[p1_idx].end(); ++itr) {
        if (vnl_math_isfinite(itr->cost_) && itr->obj_ != result->obj_) {
          ++n_valid;

          if (itr->cost_ > result->cost_)
            ++n;
        }
      }
    }
  }
}

//: \remarks if there are more than one possible ground-truths for a given
// p1, then it counts if at least one of them is within the list of candidates.
//
// If the list of candidates is empty, it counts iff corresp gt list is empty. 
//
// If the list of candidates is non-empty, and gt is empty, then we do not
// count.
//
void bmcsd_discrete_corresp::
number_of_pts1_with_gt_among_any_candidates(unsigned &n_w_gt, const bmcsd_discrete_corresp *gt) const
{
  n_w_gt = 0;
  for (unsigned p1_idx=0; p1_idx < this->size(); ++p1_idx) {
    vcl_list< bmcsd_attributed_object >::const_iterator  itr;

    // Special cases
    if (corresp_[p1_idx].empty()) {
      if (gt->corresp_[p1_idx].empty())
        ++n_w_gt;
    } else {
      if (!gt->corresp_[p1_idx].empty()) {
        bool found=false;
        for (itr = corresp_[p1_idx].begin(); itr != corresp_[p1_idx].end() && !found; ++itr) {
          vcl_list<bmcsd_attributed_object>::const_iterator 
            result = find_if(gt->corresp_[p1_idx].begin(), gt->corresp_[p1_idx].end(), bmcsd_attributed_object_eq(itr->obj_)); 

          if (result != gt->corresp_[p1_idx].end()) {
            found = true;
            ++n_w_gt;
          }
        }
      }
    }
  }
}

//: returns pointer to the right match (p2) having the lowest cost in gt
// returned iterator is equal to corresp_[p1_idx].last() in case nothing was
// found.
vcl_list<bmcsd_attributed_object>::const_iterator bmcsd_discrete_corresp::
find_right_corresp_mincost(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const
{
  // run through containing all the candidates of p1_idx
  // also appearing in the ground-truth.

  unsigned i = p1_idx;
  vcl_list<bmcsd_attributed_object>::const_iterator min_cost_itr = corresp_[i].end();
  double min_cost = vcl_numeric_limits<double>::infinity();

  if (gt->corresp_[i].empty())
    return min_cost_itr;

  vcl_list< bmcsd_attributed_object >::const_iterator  itr;
  for (itr = corresp_[i].begin(); itr != corresp_[i].end(); ++itr) {
    if (!vnl_math_isfinite(itr->cost_))
      continue;

    vcl_list<bmcsd_attributed_object>::const_iterator 
      result = find_if(gt->corresp_[i].begin(), gt->corresp_[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 

    if (result != gt->corresp_[i].end()) {
      if (vnl_math_isfinite(result->cost_)) {
        // found
        if (itr->cost_ < min_cost) {
          min_cost = itr->cost_;
          min_cost_itr = itr;
        }
      } else {
        vcl_cout << "Warning: non-finite cost in ground-truth corresp\n";
      }
    }
  }

  return min_cost_itr;
}

//: 
// purpose: to determine if there is a ground-truth correspondence among top 5
// putative correspondences of a given object in image 1.
//
// \param[in] p1_idx: index of a object in image 1
//
// \return: true or false
//
// \remarks: in the special case of an empty gt for p1_idx, return true
//
bool bmcsd_discrete_corresp::
is_gt_among_top5(unsigned p1_idx, const bmcsd_discrete_corresp *gt) const
{

  unsigned i = p1_idx;
  vcl_list<bmcsd_attributed_object>::const_iterator min_cost_itr = corresp_[i].end();

  if (gt->corresp_[i].empty())
    return true;

  unsigned ncorr_atual = 0;
  vcl_list< bmcsd_attributed_object >::const_iterator  itr;
  for (itr = corresp_[i].begin(); itr != corresp_[i].end() && ncorr_atual < 5; ++itr, ++ncorr_atual) {
    if (!vnl_math_isfinite(itr->cost_))
      continue;

    vcl_list<bmcsd_attributed_object>::const_iterator 
      result = find_if(gt->corresp_[i].begin(), gt->corresp_[i].end(), bmcsd_attributed_object_eq(itr->obj_)); 

    if (result != gt->corresp_[i].end()) {
      if (vnl_math_isfinite(result->cost_)) { 
        return true;
      } else {
        vcl_cout << "Warning: non-finite cost in ground-truth corresp\n";
      }
    }
  }

  return false;
}

void bmcsd_discrete_corresp::
keep_only_unambiguous_max(double ratio, double lonely_threshold)
{
  //  vcl_cout << "Using lowe's criterion with ratio " << ratio << vcl_endl;
  sort();
  for (unsigned i=0; i < n0(); ++i) {
    unsigned sz = corresp_[i].size();
    if ( sz == 0)
      continue;

    bmcsd_attributed_object attr = corresp_[i].back();

    if (sz == 1) {
      corresp_[i].clear();
      // Only accept a lonely dog if it is way beyond threshold
      if (attr.cost() > ratio*lonely_threshold)
        corresp_[i].push_back(attr);
      continue;
    }

    // sz >= 2

    bmcsd_attributed_object 
      attr_second = *(--(--corresp_[i].end()));

    corresp_[i].clear();

    //    vcl_cout << "second cost: " << attr_second.cost()
    //      << " first cost: "<< attr.cost() << vcl_endl;
    if (ratio*attr_second.cost() < attr.cost()) {
    //      vcl_cout << "Inserting unambiguous\n";
      corresp_[i].push_back(attr);
    }
  }
}
