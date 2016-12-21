// This is basic/dbgl/dbgl_eulerspiral_base.h
#ifndef dbgl_eulerspiral_base_h_
#define dbgl_eulerspiral_base_h_
//:
// \file
// \brief A base to derive Euler Spiral and Arc classes
// \author Ozge Can Ozcanli (ozge@lems.brown.edu)
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
#include <dbgl/dbgl_param_curve.h>

class dbgl_eulerspiral_base : public dbgl_param_curve
{
 public:

  //: default constructor
  dbgl_eulerspiral_base() : dbgl_param_curve() {}

  static const vcl_type_info& type_id()
  { return typeid(dbgl_eulerspiral_base); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_eulerspiral_base) == type)!=0 ||
            this->dbgl_param_curve::is_type(type);
  }

  dbgl_param_curve *clone() const = 0;

};

//: Write "<dbgl_param_curve of type euler spiral> to stream"
// \relates dbgl_param_curve
//vcl_ostream&  operator<<(vcl_ostream& s, dbgl_eulerspiral_base const& c);

//: Read parameters from stream
// \relates dbvgl_param_curve
//vcl_istream&  operator>>(vcl_istream& s, dbgl_eulerspiral_base const& c);

#endif // dbgl_eulerspiral_base_h_
