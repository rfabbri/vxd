// This is basic/dbgl/dbgl_arc_base.h
#ifndef dbgl_arc_base_h_
#define dbgl_arc_base_h_
//:
// \file
// \brief A base to derive Arc class
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
#include <dbgl/dbgl_eulerspiral_base.h>

class dbgl_arc_base : public dbgl_eulerspiral_base
{
 public:

  //: default constructor
  dbgl_arc_base() : dbgl_eulerspiral_base() {}

  static const vcl_type_info& type_id()
  { return typeid(dbgl_arc_base); }

  virtual bool is_type( const vcl_type_info& type ) const
  { return (typeid(dbgl_arc_base) == type)!=0 ||
            this->dbgl_eulerspiral_base::is_type(type);
  }

  dbgl_param_curve *clone() const = 0;

};

//: Write "<dbgl_param_curve of type euler spiral> to stream"
// \relates dbgl_param_curve
//vcl_ostream&  operator<<(vcl_ostream& s, dbgl_arc_base const& c);

//: Read parameters from stream
// \relates dbvgl_param_curve
//vcl_istream&  operator>>(vcl_istream& s, dbgl_arc_base const& c);

#endif // dbgl_arc_base_h_
