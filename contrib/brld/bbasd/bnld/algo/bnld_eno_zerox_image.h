// This is bnld_eno_zerox_image.h
#ifndef bnld_eno_zerox_image_h
#define bnld_eno_zerox_image_h
//:
//\file
//\brief  Zerocrossings for bnld_eno_image
////\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 10/27/2005 10:03:47 PM EDT
//

#include <bnld/algo/bnld_eno_image.h>
#include <bnld/algo/bnld_eno_zerox.h>
#include <bnld/algo/bnld_eno_zerox_label.h>

//:Computes/stores the zero crossings of row- and column-wise ENO
// interpolation of an image (bnld_eno_image).
//
class bnld_eno_zerox_image {
public: 
  bnld_eno_zerox_image(bnld_eno_image &eim);

  ~bnld_eno_zerox_image();

  const bnld_eno_zerox_vector & horiz(unsigned i) const;
  const bnld_eno_zero_crossing & horiz(unsigned i, unsigned j) const;

  const bnld_eno_zerox_vector & vert(unsigned i) const;
  const bnld_eno_zero_crossing & vert(unsigned i, unsigned j) const;



//   double sample(double x) const;
//   unsigned interval_index(double x) const;

  unsigned ni() const {return ni_;}
  unsigned nj() const {return nj_;}
  unsigned long size() const {return ni_*nj_;}

  void print(vcl_ostream& = vcl_cerr) const;

   //-----------------------------------------------------------------


   //: Compute labels
  //: Compute labels
  bool 
  assign_labels(const vil_image_view<double> &img, const vil_image_view<unsigned> &label);

  const bnld_eno_zerox_label & horiz_labels(unsigned long i) const;
  vcl_vector<unsigned>::const_iterator horiz_labels(unsigned long i, unsigned long j) const;

  const bnld_eno_zerox_label & vert_labels(unsigned long i) const;
  vcl_vector<unsigned>::const_iterator vert_labels(unsigned long i, unsigned long j) const;

  bool computed_labels() const { return vlabel_.size()!=0; }

protected:
  vcl_vector<bnld_eno_zerox_vector *> horiz_;    
  vcl_vector<bnld_eno_zerox_vector *> vert_;
  vcl_vector<bnld_eno_zerox_label *>  hlabel_;
  vcl_vector<bnld_eno_zerox_label *>  vlabel_;
  unsigned 
    ni_,  //:< number of pixels in a row (i.e. number of columns)
    nj_;  //:< number of pixels in a column (i.e. number of rows)
};

//: interpolant at (col i, row j)
inline const bnld_eno_zero_crossing & bnld_eno_zerox_image::
horiz(unsigned i, unsigned j) const
{
  return horiz_[j]->zeros(i);
}

//: 1d zerox vector at i-th row
inline const bnld_eno_zerox_vector & bnld_eno_zerox_image::
horiz(unsigned i) const
{
  return *(horiz_[i]);
} 

//: interpolant at (col i, row j)
inline const bnld_eno_zero_crossing & bnld_eno_zerox_image::
vert(unsigned i, unsigned j) const
{
  return vert_[i]->zeros(j);
}

//: 1d zerox vector at ith row
inline const bnld_eno_zerox_vector & bnld_eno_zerox_image::
vert(unsigned i) const
{
  return *(vert_[i]);
} 


// ----------------------------------------------------------------------

inline vcl_vector<unsigned>::const_iterator bnld_eno_zerox_image::
horiz_labels(unsigned long i, unsigned long j) const
{
  return (*(hlabel_[j]))[i];
}

inline const bnld_eno_zerox_label & bnld_eno_zerox_image::
horiz_labels(unsigned long i) const
{
  return *(hlabel_[i]);
}

inline vcl_vector<unsigned>::const_iterator bnld_eno_zerox_image::
vert_labels(unsigned long i, unsigned long j) const
{
  return (*(vlabel_[i]))[j];
}

inline const bnld_eno_zerox_label & bnld_eno_zerox_image::
vert_labels(unsigned long i) const
{
  return *(vlabel_[i]);
}


#endif // bnld_eno_zerox_image_h
