// This is bbasd/bgld/bgld_eulerspiral_base.h
#ifndef bgld_eulerspiral_base_h_
#define bgld_eulerspiral_base_h_
//:
// \file
// \brief A base to derive Euler Spiral and Arc classes
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
#include <bgld/bgld_param_curve.h>

class bgld_eulerspiral_base : public bgld_param_curve
{
 public:

  //: default constructor
  bgld_eulerspiral_base() : bgld_param_curve() {}

  static const vcl_type_info& type_id()
  { return typeid(bgld_eulerspiral_base); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(bgld_eulerspiral_base) == type)!=0 ||
            this->bgld_param_curve::is_type(type);
  }

  bgld_param_curve *clone() const = 0;

};

//: Write "<bgld_param_curve of type euler spiral> to stream"
// \relates bgld_param_curve
//vcl_ostream&  operator<<(vcl_ostream& s, bgld_eulerspiral_base const& c);

//: Read parameters from stream
// \relates dbvgl_param_curve
//vcl_istream&  operator>>(vcl_istream& s, bgld_eulerspiral_base const& c);

#endif // bgld_eulerspiral_base_h_
