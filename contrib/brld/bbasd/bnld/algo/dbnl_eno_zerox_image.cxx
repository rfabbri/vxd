#include "dbnl_eno_zerox_image.h"

dbnl_eno_zerox_image::
dbnl_eno_zerox_image(dbnl_eno_image &eim)
{
  ni_ = eim.ni();
  nj_ = eim.nj();

  horiz_.resize(nj_);
  vert_.resize(ni_);

  for (unsigned j=0; j < nj_; ++j)
    horiz_[j] = new dbnl_eno_zerox_vector( eim.horiz(j) );

  for (unsigned i=0; i < ni_; ++i)
    vert_[i] = new dbnl_eno_zerox_vector( eim.vert(i) );

}


dbnl_eno_zerox_image::
~dbnl_eno_zerox_image()
{
  for (unsigned i=0; i < ni_; ++i)
    delete vert_[i];

  for (unsigned j=0; j < nj_; ++j)
    delete horiz_[j];

  for (unsigned i=0; i < vlabel_.size(); ++i)
    delete vlabel_[i];

  for (unsigned j=0; j < hlabel_.size(); ++j)
    delete hlabel_[j];
}

void 
dbnl_eno_zerox_image::
print(vcl_ostream& strm) const
{

  strm << "===== Eno Zerox Image =====\n"
       << "Rows: " << nj_ << "  Cols: " << ni_ << vcl_endl;

  strm << "Computed labels: " << ((computed_labels())? "yes" : "no") << vcl_endl;
  
}


//: Compute labels
bool dbnl_eno_zerox_image::
assign_labels(const vil_image_view<double> &img, const vil_image_view<unsigned> &label)
{

  if (!label.is_contiguous() || label.istep() != 1) {
    vcl_cerr << "dbnl_eno_zerox_image::assign_labels : only contigous row-wise (col,row) images supported\n";
    return false;
  }

  hlabel_.resize(nj_);
  vlabel_.resize(ni_);

  const unsigned *label_data = label.top_left_ptr();
  const double *img_data = img.top_left_ptr();

  for (unsigned j=0; j < nj_; ++j)
    hlabel_[j] = new dbnl_eno_zerox_label( img_data + j*ni_, label_data + j*ni_, horiz(j) );

  unsigned *buffer;    //:< an image column as contigous block
  double   *imgbuffer; //:< an image column as contigous block
  buffer    = new unsigned[nj_];
  imgbuffer = new double[nj_];

  for (unsigned i=0; i < ni_; ++i) {
    for (unsigned j=0; j < nj_; ++j) {
      buffer[j]    = label(i,j);
      imgbuffer[j] = img(i,j);
    }
    vlabel_[i] = new dbnl_eno_zerox_label(imgbuffer, buffer, vert(i));
  }

  delete [] buffer;
  delete [] imgbuffer;

  return true;
}
