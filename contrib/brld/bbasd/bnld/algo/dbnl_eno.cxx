#include "dbnl_eno.h"
#include <vcl_cstdio.h>
#include <vcl_cassert.h>
#include <vcl_limits.h>
#include <vcl_algorithm.h>  

//:
// \file
// \author Ricardo Fabbri

#if !VCL_STATIC_CONST_INIT_FLOAT_NO_DEFN
const double  dbnl_eno::near_zero_value
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(1.0e-8);
const double  dbnl_eno::interval_tolerance
      VCL_STATIC_CONST_INIT_FLOAT_DEFN(1.0e-8);
#endif

//:
// \return true on success
bool dbnl_eno_1d::
interpolate(double const *data, unsigned len, ptrdiff_t stride)
{
   /* if don't have enough data to interpolate intervals, nothing to
      do here */
#ifndef NDEBUG
   if (len < DBNL_ENO_DATA_LENGTH) {
      // better err treatment TODO
      printf("data gridline too short to interpolate.\n");
      printf("in: enointerv_compute_interpolants\n");
      return false;
   }
#endif


   double nhood[DBNL_ENO_DATA_LENGTH]; /* storage for copying
                                           local neighborhood */

   vcl_vector<dbnl_eno_interp>::iterator interp_ptr;

   stride_ = stride;
   interp_.resize(len*stride -1);
   interp_ptr = interp_.begin();

   /* intialize loop index before calculations */
   unsigned ii = 0;

   /* A macro for improved readability */
#define ENOStoreDataList4(l,f0,f1,f2,f3) \
    do{(l)[0] = (f0); (l)[1] = (f1); (l)[2] = (f2); (l)[3] = (f3);}while(0)


   /* do the first border value by hand */
   ENOStoreDataList4(nhood,
                     border_value_, data[0],
                     data[stride * 1], data[stride * 2]);
   interp_ptr->set_bounds(ii, ii + 1);
   interp_ptr->interpolate(nhood);
   data += stride;
   interp_ptr += stride;

   for (ii = 1; ii +2 < len; ++ii) {

      ENOStoreDataList4(nhood,
                        data[stride * -1], data[0],
                        data[stride * 1], data[stride * 2]);
      interp_ptr->set_bounds(ii, ii + 1);
      interp_ptr->interpolate(nhood);

      /* increment to next element */
      data += stride;
      interp_ptr += stride;
   }

   /* do the last border value by hand */
   ENOStoreDataList4(nhood,
                     data[stride * -1], data[0],
                     data[stride * 1], border_value_);
   interp_ptr->set_bounds(ii, ii + 1);
   interp_ptr->interpolate(nhood);

#undef ENOStoreDataList4

   abscissas_.set_size(len);
   for (ii=0; ii< len; ++ii)
      abscissas_[ii] = ii;

   return true;
}

//:
// stride does not apply to abcissa vector x
// \return true on success
bool dbnl_eno_1d::
interpolate(double const *data, double const *x, unsigned len, ptrdiff_t stride)
{
   /* if don't have enough data to interpolate intervals, nothing to
      do here */
#ifndef NDEBUG
   if (len < DBNL_ENO_DATA_LENGTH) {
      // better err treatment TODO
      printf("data gridline too short to interpolate.\n");
      printf("in: enointerv_compute_interpolants\n");
      return false;
   }
#endif

   double nhood[DBNL_ENO_DATA_LENGTH]; /* storage for copying
                                           local neighborhood */

   vcl_vector<dbnl_eno_interp>::iterator interp_ptr;

   stride_ = stride;
   interp_.resize(len*stride -1);
   interp_ptr = interp_.begin();

   /* intialize loop index before calculations */
   unsigned ii = 0;

   /* A macro for improved readability */
#define ENOStoreDataList4(l,f0,f1,f2,f3) \
    do{(l)[0] = (f0); (l)[1] = (f1); (l)[2] = (f2); (l)[3] = (f3);}while(0)


   interp_ptr->set_bounds(x[0], x[1]);
   interp_ptr->interpolate(data[0], data[stride*1], data[stride*2],x[0],x[1],x[2]);

   data += stride;
   interp_ptr += stride;

   for (ii = 1; ii +2 < len; ++ii) {

      ENOStoreDataList4(nhood,
                        data[stride * -1], data[0],
                        data[stride * 1], data[stride * 2]);

      interp_ptr->set_bounds(x[ii], x[ii + 1]);
      interp_ptr->interpolate(nhood,x+ii-1);

      /* increment to next element */
      data += stride;
      interp_ptr += stride;
   }

   interp_ptr->set_bounds(x[ii], x[ii + 1]);
   interp_ptr->interpolate(*data,data[stride*1],data[-1*stride],x[ii],x[ii+1],x[ii-1]);

#undef ENOStoreDataList4

   abscissas_.set_size(len);
   abscissas_.copy_in(x);

   return true;
}

void dbnl_eno_1d::
print(vcl_ostream& strm) const
{

   strm << "==== Eno 1D ====" << vcl_endl
        << "len: " << size() << vcl_endl;

   unsigned i;
   for (i=0; i < size(); ++i)
      interp_.at(i).print(strm);
}

double dbnl_eno_1d::
sample(double x) const
{
   unsigned i=interval_index(x);
   return interp_[i].sample(x);
}

bool dbnl_eno_1d::
sample(unsigned size, vnl_vector<double> &f, vnl_vector<double> &xx) const
{
   f.set_size(size);
   xx.set_size(size);

   double pace= ( x(this->size()) - x(0) ) / (size-1);

   for (unsigned i=0; i<size; i++) {
      xx[i] = x(0) + i*pace;
      f[i] = sample(xx(i));
   }

   return true;
}

//: Finds index of the interval containing a given abscissa, using
// binary search.
// If s correspond to one of the endpoints/samples, we return the index of the
// interval to the "left" of it.
unsigned dbnl_eno_1d::
interval_index(double x) const
{
   // binary search for s in vector of arclens
   const vnl_vector<double>::const_iterator
      p = vcl_lower_bound(abscissas_.begin(), abscissas_.end(), x);

   unsigned i = p - abscissas_.begin();

   return (i == 0)? i : i-1;
}
