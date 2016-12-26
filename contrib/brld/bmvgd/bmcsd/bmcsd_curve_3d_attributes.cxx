#include "dbmcs_curve_3d_attributes.h"
#include <vsl/vsl_vector_io.h>
#include <mw/dbmcs_view_set.h>

void dbmcs_curve_3d_attributes::
b_write(vsl_b_ostream &os) const
{
  // print_summary(vcl_cout);
  vsl_b_write(os, version());
  vsl_b_write(os, *v_); // yuck..
  vsl_b_write(os, inlier_views_);
  vsl_b_write(os, total_support_);
  vsl_b_write(os, i0_);
  vsl_b_write(os, i1_);
}

void dbmcs_curve_3d_attributes::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1:
      abort();
      v_ = new dbmcs_stereo_views();
      vsl_b_read(is, *v_); // urghhhh, nasty
      vsl_b_read(is, inlier_views_);
    break;
    case 2:
      v_ = new dbmcs_stereo_views();
      vsl_b_read(is, *v_); // urghhhh, nasty
      vsl_b_read(is, inlier_views_);
      vsl_b_read(is, total_support_);
      vsl_b_read(is, i0_);
      vsl_b_read(is, i1_);
    break;

    default:
        vcl_cerr << "I/O ERROR: dbmcs_curve_2d_attributes::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: Print an ascii summary to the stream
void dbmcs_curve_3d_attributes::
print_summary(vcl_ostream &os) const
{
  os << "[" << is_a() << ": " << *v_
    << " inlier_views: ";

  for (unsigned i=0; i < inlier_views_.size(); ++i)
    os << inlier_views_[i] << " ";
  os << " i0: " << i0() << " i1: " << i1() << "total_support: " << total_support_;
  os << "]";
}
