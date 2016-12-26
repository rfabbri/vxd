// This is mw_sift_curve.h
#ifndef MW_SIFT_CURVE_H
#define MW_SIFT_CURVE_H
//:
//\file
//\brief A sequence of sift features along a curve
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri@gmail.com)
//\date 12/02/2008 03:45:22 PM EST
//

#include <vcl_vector.h>
#include <vcl_iomanip.h>
#include <vnl/vnl_vector_fixed.h>

#define MW_SIFT_CURVE_NUM_DIMS 128

class mw_sift_curve {
private:
  static const unsigned num_dims_=128;

public:
  // IMPORTANT: change to be consistent with whatever sift / implementation.
  typedef double t_descriptor_float; 
  typedef vcl_vector<vcl_vector<vnl_vector_fixed<t_descriptor_float, num_dims_> > > 
    descriptor_vector;


  mw_sift_curve()
  {}

  #define RF_UNINITIALIZED_HISTOGRAM_BIN 1
  mw_sift_curve(unsigned num_scales, unsigned num_samples) 
    :
      descriptors_(num_scales),
      num_samples_(num_samples),
      valid_(num_scales)
  {
    for (unsigned i=0; i < descriptors_.size(); ++i)  {
      descriptors_[i].resize(num_samples_);
      for (unsigned k=0; k < num_samples; ++k)
        descriptors_[i][k].fill(RF_UNINITIALIZED_HISTOGRAM_BIN);
      valid_[i].resize(num_samples_,false);
    }
  }
  #undef RF_UNINITIALIZED_HISTOGRAM_BIN

  ~mw_sift_curve()
  {}

  void print_all_descriptors() const
  {
    vcl_cout << "Number of dimensions: " << num_dims() << vcl_endl;
    unsigned is=0;
    unsigned max_n_prints = 300;

    for (unsigned i=0; i < num_scales(); ++i) {
      vcl_cout << vcl_setprecision(1);
      vcl_cout << "\nScale #" << i+1 << vcl_endl;
      if (!num_samples()) {
        vcl_cout << "No samples\n";
        return;
      }
      for (unsigned k=0; k < num_samples(); ++k) {
        vcl_cout << "  sample " << k+1 << "\n";
        if (is_valid(i,k)) {
          // output descriptor
          assert(num_dims());
          unsigned m=0;
          for (m=0; m+1 < num_dims(); ++m)
            vcl_cout << descriptors_[i][k][m] << ", ";
          vcl_cout << descriptors_[i][k][m] << ";" << vcl_endl;
        } else {
          vcl_cout << "EMPTY" << vcl_endl;
        }
        vcl_cout << vcl_endl;
        ++is;
        if (is > max_n_prints)
          return;
      }
    }
  }

  const descriptor_vector &v();

  unsigned num_dims() const { return num_dims_; }
  unsigned num_scales() const { return descriptors_.size(); }

  //: returns the number of curve samples per scale
  unsigned num_samples() const { return num_samples_; }

  void set_size(unsigned num_scales, unsigned num_samples) { 
    num_samples_ = num_samples;
    descriptors_.resize(num_scales); 
    valid_.resize(num_scales); 
    for (unsigned i=0; i < descriptors_.size(); ++i) {
      descriptors_[i].resize(num_samples);
      valid_[i].resize(num_samples_,false);
    }
  }

  void set_validity(unsigned scale_index, unsigned sample_index, bool validity) {
    valid_[scale_index][sample_index] = validity;
  }

  bool is_valid(unsigned scale_index, unsigned sample_index) const {
    return valid_[scale_index][sample_index];
  }

  //: Access the the feature vcl_vector.
  const vnl_vector_fixed<t_descriptor_float, num_dims_> &
  descriptor(unsigned scale_index, unsigned sample_index) const {
    return descriptors_[scale_index][sample_index]; 
  }

  //: For efficiency reasons, access directly the data block for the feature vcl_vector.
  // This is OK since the feature vector is bound to be represented this way.
  t_descriptor_float *descriptor_data_block(unsigned scale_index, unsigned sample_index) {
    return descriptors_[scale_index][sample_index].data_block(); 
  }

private:
  descriptor_vector descriptors_;
  unsigned num_samples_; //< number of curve samples per scale
  vcl_vector< vcl_vector<bool> > valid_;
};

#endif // MW_SIFT_CURVE_H

