// This is dbnl_eno_zerox_label.h
#ifndef dbnl_eno_zerox_label_h
#define dbnl_eno_zerox_label_h



#include <vcl_limits.h>
#include <vcl_vector.h>
#include <dbnl/algo/dbnl_eno_zerox.h>

//:
//\file
//\brief ENO zero-crossing labeling code
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 10/26/2005 03:38:02 PM EDT
//

//: Assign labels to zero-crossings of an ENO1d interpolation, from given labels
// at each data sample. This is based on an old code by Tek and others, and
// should be useful in some applications.
//
// It currently only works for 2nd-order ENO interpolations WITHOUT shocks. The
// reason for this is historical: the original code didn't deal with shocks, and
// we don't want to attempt doing it for now.
//
// Note that this class should be used side-by-side with a corresponding
// dbnl_eno_zerox_vector. This class will not store the number of zero crossings
// or any other info already in other classes that should be used in conjunction
// with this one.
//
class dbnl_eno_zerox_label {
public:

  static const unsigned unlabeled 
     VCL_STATIC_CONST_INIT_INT_DECL((unsigned) -1);

  //: max number of zeros per interval supported by this class
  static const unsigned maxzeros
     VCL_STATIC_CONST_INIT_INT_DECL(4);

  //: constructor does all the work and stores result in this object
  dbnl_eno_zerox_label(
      const double *data,
      const unsigned *label,
      const dbnl_eno_zerox_vector &zc);

  ~dbnl_eno_zerox_label() {}

  //: return labels for current interval in a 2-element array 
  // For example, if zl is an object of this class, then
  // zl[i][k] = label of k-th zero-crossing of i-th interval.
  vcl_vector<unsigned>::const_iterator operator [] (unsigned i) const { return label_.begin() + maxzeros*i;}
;
  //: number of intervals
  unsigned size() const {return label_.size()/maxzeros;}

  void print (vcl_ostream&) const { vcl_cout << "Not yet coded\n";}

  void print(dbnl_eno_1d &eno, dbnl_eno_zerox_vector &zc, vcl_ostream& strm) const;

protected:
  //: The labeling of each zero-crossing is implemented by 1D vector
  //representing a 2D array, where the columns correspond to intervals, 
  // and rows to labels. This 2D array is stored in the vector "label_"
  // column-wise.
  vcl_vector<unsigned> label_;

  // Internally used auxiliary function
  void label_one_zerox_in_interval(
      unsigned pos,
      const dbnl_eno_zero_crossing &, 
      const double *data, 
      const unsigned *data_label);

  // Internally used auxiliary function
  void label_two_zerox_in_interval(
      unsigned pos,
      const dbnl_eno_zero_crossing &zc_i, 
      const double *data, 
      const unsigned *data_label);

  unsigned & LABEL_(unsigned i, unsigned k) { return label_[maxzeros*i + k];}
  const unsigned & LABEL_(unsigned i, unsigned k) const { return label_[maxzeros*i + k];}
};


#endif // dbnl_eno_zerox_label_h

