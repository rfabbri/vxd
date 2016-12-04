// This is bbld_curve_subsets.h
#ifndef bbld_subsequence_h
#define bbld_subsequence_h
//:
//\file
//\brief Utilities to represent sets of sub-sequences, similar to boost's ranges
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 09/02/2009 08:10:03 AM PDT
//

#include <vcl_vector.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <assert.h>

//: This represents a subsequence of a sequence whose elements can be addressed
// by indexes (id's).
//
// The subsequence is represented by original_sequence[ini] to
// original_sequence[end-1];
//
// The concept behind this class is that of an open range with an id of the
// "object" on which the range is represented.
//
// \remarks This is similar (but a lot simpler) than Boost's range and
// associated utilities. Each subsequence also provides an id that can be used
// to refer to the parent sequence.
//
class bbld_subsequence {
public:
  bbld_subsequence(unsigned ini, unsigned end) { set(ini, end); }
  bbld_subsequence(unsigned ini, unsigned end, unsigned orig_id) 
  { set(ini, end); set_orig_id(orig_id);}
  bbld_subsequence(unsigned ini, unsigned end, unsigned orig_id, unsigned int_id)
  { set(ini, end); set_orig_id(orig_id); set_int_id(int_id); }

  bbld_subsequence() {}

  void set(unsigned ini, unsigned end) 
    {assert (ini < end); ini_ = ini; end_ = end; }

  void set_orig_id(unsigned i) { orig_id_ = i; }
  void set_int_id(unsigned i) { int_id_ = i; }

  unsigned size() const { return end_ - ini_; }
  unsigned ini() const { return ini_; } 
  unsigned end() const { return end_; } 
  unsigned orig_id() const { return orig_id_; } 
  unsigned int_id() const { return int_id_; }
  bool in_range(unsigned i) const { return i >= ini() && i < end(); }

private:
  //: id of the initial element on the original sequence
  unsigned ini_; 
  //: id of the end element on the original sequence
  unsigned end_;
  //: id of the parent sequence within a random access container of sequences
  unsigned orig_id_;
  //: Anil: id of an intermediate sequence, to be used when needed
  unsigned int_id_;
};

//: Given a sequence of sequences, represent a subsequence of subsequences.
// For example, given a  vcl_vector<vcl_vector<point> > v,
// then this class represents another vcl_vector<vcl_vector<point> > w, such
// that all vectors in w are contained in v, and all vectors w[i] is a subset of
// some v[k].
//
class bbld_subsequence_set {
public:

  //: reserve a number of subsequences
  void reserve(unsigned n) { ss_.reserve(n); }

  //: \param[in] i : index of the new subsequence into this set of subsequences
  //  \param[in] original_id : index of the parent sequence into the original
  //  set of sequences
  void set_subsequence(unsigned i, const bbld_subsequence &s) 
  { ss_[i] = s;}

  void push_back(const bbld_subsequence &s) 
  { ss_.push_back(s);}

  const bbld_subsequence &subsequence(unsigned i) const 
  { assert (i < num_subsequences()); return ss_[i]; }

  const bbld_subsequence &operator [] (unsigned i) const 
  { assert (i < num_subsequences()); return ss_[i]; }

  unsigned num_subsequences() const { return ss_.size(); }

  //: the sum of all subsequence's sizes
  unsigned long total_size() const { 
    unsigned long t = 0;
    for (unsigned i=0; i < num_subsequences(); ++i)
      t += subsequence(i).size();
    return t;
  }

  void trim_memory() { vcl_vector<bbld_subsequence >(ss_).swap(ss_); }

private:
  //: s_[i] = (id, ini, end), where id is an index into v as in the above
  // example, and v[id][ini], v[id][ini+1], ..., v[id][end] is the subsequence,
  vcl_vector<bbld_subsequence > ss_;
};

//: param[in] a: a subsequence set from the original set O to an intermediary one I
//  param[in] b: a subsequence set from an intermediate set I to a final one F
//  param[out] b: a subsequence set from the original set O to the final one F
inline void compose_subsequences(const bbld_subsequence_set &a, bbld_subsequence_set *b)
{
  for (unsigned i=0; i < b->num_subsequences(); ++i) {
    const bbld_subsequence &b_subseq = b->subsequence(i);
    const bbld_subsequence &a_subseq = a.subsequence(b_subseq.orig_id());

    bbld_subsequence ss(a_subseq.ini() + b_subseq.ini(), 
                      a_subseq.ini() + b_subseq.end());

    ss.set_orig_id(a_subseq.orig_id());
    ss.set_int_id(b_subseq.orig_id());

    b->set_subsequence(i, ss);
  }
}

//: Copy the elements themselves from the original sequence into a new
// container. T has to behave like a vcl_vector and represents a sequence.
template <typename T> void 
consolidate_subsequences(
    const vcl_vector<T> &seq,
    bbld_subsequence_set s_ids,
    vcl_vector<T> *ss_ptr
    )
{
  assert (&seq != ss_ptr);
  vcl_vector<T> &ss = *ss_ptr;

  ss.resize(s_ids.num_subsequences());
  for (unsigned i=0; i < s_ids.num_subsequences(); ++i) {
    const bbld_subsequence &ss_id = s_ids.subsequence(i);

    ss[i].reserve(ss_id.size());
    for (unsigned p = ss_id.ini(); p < ss_id.end(); ++p)
      ss[i].push_back(seq[ss_id.orig_id()][p]);
  }
}

//: Specialization of consolidate for vsol polylines
inline void 
consolidate_subsequences(
    const vcl_vector<vsol_polyline_2d_sptr> &seq,
    bbld_subsequence_set s_ids,
    vcl_vector<vsol_polyline_2d_sptr> *ss_ptr
    )
{
  assert (&seq != ss_ptr);
  vcl_vector<vsol_polyline_2d_sptr> &ss = *ss_ptr;

  ss.reserve(s_ids.num_subsequences());
  for (unsigned i=0; i < s_ids.num_subsequences(); ++i) {
    vcl_vector<vsol_point_2d_sptr> subcurve;

    const bbld_subsequence &ss_id = s_ids.subsequence(i);

    subcurve.reserve(ss_id.size());
    for (unsigned p = ss_id.ini(); p < ss_id.end(); ++p)
      subcurve.push_back(seq[i]->vertex(p));
    ss.push_back(new vsol_polyline_2d(subcurve));
  }
}

//: Partitions a subsequence \p orig into a set of contiguous subsequences \p ss
// such that, for each resulting subsequence, the predicate is either true or
// false.
//
// \param[in] cond : a predicate of the type: bool cond(unsigned i), where i
// would represent an element in the range of the subsequence \p orig
//
// \param[out] ss : the set of subsequences of orig, but with ss[i].orig_id ==
// orig.orig_id();
template <typename Predicate_T> void
bbld_contiguous_partition(
    const bbld_subsequence &orig, const Predicate_T &cond, bbld_subsequence_set *ss_ptr)
{
  bbld_subsequence_set &ss = *ss_ptr;
  bool prev_cond = cond(orig.ini());
  unsigned ini = orig.ini();
  unsigned n_elements = orig.size();

  ss.reserve(n_elements);
  unsigned end = orig.ini() + 1;
  for (;  end != orig.end();  ++end) {
    if (cond(end) != prev_cond) {
      ss.push_back(bbld_subsequence(ini, end, orig.orig_id()));
      ini = end; 
      prev_cond = cond(end);
    }
  }
  ss.push_back(bbld_subsequence(ini, end, orig.orig_id()));
  ss.trim_memory();
}

#endif // bbld_subsequence_h
