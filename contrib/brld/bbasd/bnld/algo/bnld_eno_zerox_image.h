// This is dbnl_eno_zerox_image.h
#ifndef dbnl_eno_zerox_image_h
#define dbnl_eno_zerox_image_h
//:
//\file
//\brief  Zerocrossings for dbnl_eno_image
////\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 10/27/2005 10:03:47 PM EDT
//

#include <dbnl/algo/dbnl_eno_image.h>
#include <dbnl/algo/dbnl_eno_zerox.h>
#include <dbnl/algo/dbnl_eno_zerox_label.h>

//:Computes/stores the zero crossings of row- and column-wise ENO
// interpolation of an image (dbnl_eno_image).
//
class dbnl_eno_zerox_image {
public: 
  dbnl_eno_zerox_image(dbnl_eno_image &eim);

  ~dbnl_eno_zerox_image();

  const dbnl_eno_zerox_vector & horiz(unsigned i) const;
  const dbnl_eno_zero_crossing & horiz(unsigned i, unsigned j) const;

  const dbnl_eno_zerox_vector & vert(unsigned i) const;
  const dbnl_eno_zero_crossing & vert(unsigned i, unsigned j) const;



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

  const dbnl_eno_zerox_label & horiz_labels(unsigned long i) const;
  vcl_vector<unsigned>::const_iterator horiz_labels(unsigned long i, unsigned long j) const;

  const dbnl_eno_zerox_label & vert_labels(unsigned long i) const;
  vcl_vector<unsigned>::const_iterator vert_labels(unsigned long i, unsigned long j) const;

  bool computed_labels() const { return vlabel_.size()!=0; }

protected:
  vcl_vector<dbnl_eno_zerox_vector *> horiz_;    
  vcl_vector<dbnl_eno_zerox_vector *> vert_;
  vcl_vector<dbnl_eno_zerox_label *>  hlabel_;
  vcl_vector<dbnl_eno_zerox_label *>  vlabel_;
  unsigned 
    ni_,  //:< number of pixels in a row (i.e. number of columns)
    nj_;  //:< number of pixels in a column (i.e. number of rows)
};

//: interpolant at (col i, row j)
inline const dbnl_eno_zero_crossing & dbnl_eno_zerox_image::
horiz(unsigned i, unsigned j) const
{
  return horiz_[j]->zeros(i);
}

//: 1d zerox vector at i-th row
inline const dbnl_eno_zerox_vector & dbnl_eno_zerox_image::
horiz(unsigned i) const
{
  return *(horiz_[i]);
} 

//: interpolant at (col i, row j)
inline const dbnl_eno_zero_crossing & dbnl_eno_zerox_image::
vert(unsigned i, unsigned j) const
{
  return vert_[i]->zeros(j);
}

//: 1d zerox vector at ith row
inline const dbnl_eno_zerox_vector & dbnl_eno_zerox_image::
vert(unsigned i) const
{
  return *(vert_[i]);
} 


// ----------------------------------------------------------------------

inline vcl_vector<unsigned>::const_iterator dbnl_eno_zerox_image::
horiz_labels(unsigned long i, unsigned long j) const
{
  return (*(hlabel_[j]))[i];
}

inline const dbnl_eno_zerox_label & dbnl_eno_zerox_image::
horiz_labels(unsigned long i) const
{
  return *(hlabel_[i]);
}

inline vcl_vector<unsigned>::const_iterator dbnl_eno_zerox_image::
vert_labels(unsigned long i, unsigned long j) const
{
  return (*(vlabel_[i]))[j];
}

inline const dbnl_eno_zerox_label & dbnl_eno_zerox_image::
vert_labels(unsigned long i) const
{
  return *(vlabel_[i]);
}


#endif // dbnl_eno_zerox_image_h
