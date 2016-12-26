#include "dbnl_eno_image.h"



bool dbnl_eno_image::
interpolate(const vil_image_view<double> *img)
{

  // - resize horiz_ and vert_ to len
  // - alloc eno1d vectors

  if (!img->is_contiguous() || img->istep() != 1) {
    vcl_cerr << "dbnl_eno_image: only contigous row-wise (col,row) images supported\n";
    return false;
  }
  
  ni_ = img->ni();
  nj_ = img->nj();

  horiz_.resize(nj_);
  vert_.resize(ni_);

  const double *imgdata = img->top_left_ptr();
  // - for each row of data, call eno1d
  for (unsigned j=0; j < nj_; ++j) {
    horiz_[j] = new dbnl_eno_1d(border_value_);
    horiz_[j]->interpolate(imgdata + j*ni_, ni_);
  }
  
  vnl_vector<double> buffer(nj_); //:< an image column as contigous block
  for (unsigned i=0; i < ni_; ++i) {
    vert_[i] = new dbnl_eno_1d(border_value_);
    for (unsigned j=0; j<nj_; ++j)
      buffer[j] = (*img)(i,j);
    vert_[i]->interpolate(&buffer);
  }

  return true;
}


dbnl_eno_image::
~dbnl_eno_image()
{
  for (unsigned i=0; i < ni_; ++i)
    delete vert_[i];

  for (unsigned j=0; j < nj_; ++j)
    delete horiz_[j];
}

void 
dbnl_eno_image::
print(vcl_ostream& strm) const
{

  strm << "===== Eno Image =====\n"
       << "Rows: " << nj_ << "  Cols: " << ni_ << vcl_endl;
  
}
