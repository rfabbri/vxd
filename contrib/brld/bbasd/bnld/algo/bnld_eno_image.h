// This is bbasd/bnld/bnld_eno_image.h
#ifndef bnld_eno_image_h
#define bnld_eno_image_h
//:
// \file
// \brief Essentially Non-Oscillatory 1D interpolation of an image
// \author Based on original code by  Ricardo Fabbri, LEMS, Brown University (rfabbri.github.io)
// \date Feb 2005
// \version 1.0
//
//
// \verbatim
// Modifications
// \endverbatim

#include <bnld/algo/bnld_eno.h>
#include <vil/vil_image_view.h>


//: Interpolates an image column- and row-wise. The image is any 2D array of
// doubles, where the coordinates of each sample are integers.
//
// \Remarks The implementation can be optimized while the interface is kept.
//
class bnld_eno_image {
public: 
   bnld_eno_image(double border_value = vcl_numeric_limits<double>::max()/1e10) 
      : border_value_(border_value) { }

   ~bnld_eno_image();

   bool interps_been_computed() const {return horiz_.size() != 0;}

   bool interpolate(const vil_image_view<double> *img);

   const bnld_eno_1d & horiz(unsigned long i) const;
   const bnld_eno_interp & horiz(unsigned long i, unsigned long j) const;

   const bnld_eno_1d & vert(unsigned long i) const;
   const bnld_eno_interp & vert(unsigned long i, unsigned long j) const;

//   double sample(double x) const;
//   unsigned interval_index(double x) const;

   unsigned ni() const {return ni_;}
   unsigned nj() const {return nj_;}
   unsigned long size() const {return ni_*nj_;}

   void print(vcl_ostream& = vcl_cerr) const;

protected:
   vcl_vector<bnld_eno_1d *> horiz_;    
   vcl_vector<bnld_eno_1d *> vert_;
   unsigned 
     ni_,  //:< number of pixels in a row (i.e. number of columns)
     nj_;  //:< number of pixels in a column (i.e. number of rows)

   const double border_value_; //:< value of function outside of data bounds 
};

//: interpolant at (col i, row j)
inline const bnld_eno_interp & bnld_eno_image::
horiz(unsigned long i, unsigned long j) const
{
  return horiz_[j]->interp()[i];
}

inline const bnld_eno_1d & bnld_eno_image::
horiz(unsigned long i) const
{
  return *(horiz_[i]);
} 

//: interpolant at (col i, row j)
inline const bnld_eno_interp & bnld_eno_image::
vert(unsigned long i, unsigned long j) const
{
  return vert_[j]->interp()[i];
}

inline const bnld_eno_1d & bnld_eno_image::
vert(unsigned long i) const
{
  return *(vert_[i]);
} 

#endif // bnld_eno_image_h
