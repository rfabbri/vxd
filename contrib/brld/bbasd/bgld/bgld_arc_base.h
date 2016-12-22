// This is bbasd/bgld/bgld_arc_base.h
#ifndef bgld_arc_base_h_
#define bgld_arc_base_h_
//:
// \file
// \brief A base to derive Arc class
// \author Based on original code by  Ozge Can Ozcanli (@lems.brown.edu)
// \date 02/09/05
//
// \verbatim
// Modifications
//
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_line_2d.h>
#include <bgld/bgld_eulerspiral_base.h>

class bgld_arc_base : public bgld_eulerspiral_base
{
 public:

  //: default constructor
  bgld_arc_base() : bgld_eulerspiral_base() {}

  static const vcl_type_info& type_id()
  { return typeid(bgld_arc_base); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(bgld_arc_base) == type)!=0 ||
            this->bgld_eulerspiral_base::is_type(type);
  }

  bgld_param_curve *clone() const = 0;

};

//: Write "<bgld_param_curve of type euler spiral> to stream"
// \relates bgld_param_curve
//vcl_ostream&  operator<<(vcl_ostream& s, bgld_arc_base const& c);

//: Read parameters from stream
// \relates dbvgl_param_curve
//vcl_istream&  operator>>(vcl_istream& s, bgld_arc_base const& c);

#endif // bgld_arc_base_h_
