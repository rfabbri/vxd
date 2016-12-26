#include "dbmcs_view_set.h"
#include <vcl_fstream.h>
#include <dbul/dbul_parse_simple_file.h>
#include <vsl/vsl_vector_io.h>

//: returns view instances for a specific dataset.
void dbmcs_view_set::
get_capitol_subset_view_instances(dbmcs_stereo_instance_views *frames_to_match)
{
  dbmcs_stereo_instance_views &f = *frames_to_match;

  {
  dbmcs_stereo_views_sptr instance = new dbmcs_stereo_views();

  instance->set_stereo0(0);
  instance->set_stereo1(2);

  instance->reserve_num_confirmation_views(4);
  instance->add_confirmation_view(1);
  instance->add_confirmation_view(3);
  instance->add_confirmation_view(4);
  instance->add_confirmation_view(5);

  f.add_instance(instance);
  }

  {
  dbmcs_stereo_views_sptr instance = new dbmcs_stereo_views();

  instance->set_stereo0(1);
  instance->set_stereo1(3);

  instance->reserve_num_confirmation_views(4);
  instance->add_confirmation_view(0);
  instance->add_confirmation_view(2);
  instance->add_confirmation_view(4);
  instance->add_confirmation_view(5);

  f.add_instance(instance);
  }
}

bool dbmcs_view_set::
read_txt(
    const vcl_string &fname,
    dbmcs_stereo_instance_views *frames_to_match)
{
  dbmcs_stereo_instance_views &f = *frames_to_match;

  vcl_vector< vcl_vector <int> > ids;

  bool retval = dbul_parse_number_lists (fname, ids);
  if (!retval) {
    vcl_cerr << "ERROR: in parsing number lists\n";
    return false;
  }

  for (unsigned l=0; l < ids.size(); ++l) {
    if (ids[l].size() < 3) {
      vcl_cerr << 
        "ERROR in dbmcs_view_set::read_txt - need at least 2 views in line " << l << vcl_endl;
      return false; 
    }

    dbmcs_stereo_views_sptr instance = new dbmcs_stereo_views();

    instance->set_stereo0(ids[l][0]);
    instance->set_stereo1(ids[l][1]);
    instance->reserve_num_confirmation_views(ids[l].size()-2);
    for (unsigned c=2; c < ids[l].size(); ++c) {
      instance->add_confirmation_view(ids[l][c]);
    }
    f.add_instance(instance);
  }

  return true;
}

vcl_ostream&  operator<<(vcl_ostream& s, const dbmcs_stereo_instance_views& p)
{
  s << "num instances: " << p.num_instances() << vcl_endl;
  for (unsigned i=0; i < p.num_instances(); ++i)
    s << *p.instance(i) << vcl_endl;

  return s;
}

vcl_ostream&  operator<<(vcl_ostream& s, const dbmcs_stereo_views & p)
{
  s << "stereo0: " << p.stereo0() << vcl_endl;
  s << "stereo1: " << p.stereo1() << vcl_endl;
  s << "num_conf: " << p.num_confirmation_views() << vcl_endl;

  for (unsigned i=0; i < p.num_confirmation_views(); ++i) {
    s << p.confirmation_view(i) << " "; 
  }
  s << vcl_endl;

  return s;
}

void dbmcs_stereo_views::
b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os, stereo0());
  vsl_b_write(os, stereo1());
  vsl_b_write(os, confirm_);
}

void dbmcs_stereo_views::
b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch (ver)
  {
    case 1:
      unsigned s;
      vsl_b_read(is, s);
      set_stereo0(s);

      vsl_b_read(is, s);
      set_stereo1(s);

      vsl_b_read(is, confirm_);
    break;

    default:
        vcl_cerr << "I/O ERROR: dbmcs_stereo_views::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}
