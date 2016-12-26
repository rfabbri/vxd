#include "dbul_arg.h"
#include <vcl_iostream.h>


template <> void settype(vul_arg<vcl_vector<vcl_string> > &arg) { arg.type_ = "string list"; }

template <> void print_value(vcl_ostream &s, vul_arg<vcl_vector<vcl_string> > const &argmt)
{
  for (unsigned int i=0; i<argmt().size(); ++i) {
    s << argmt()[i];
    if (argmt()[i].length() == 0)
      s << "<empty>";
    if (i + 1 != argmt().size())
      s << ", " ;
  }
}

template <> int parse(vul_arg<vcl_vector<vcl_string> >* argmt, char ** argv)
{
  const char separator=' ';

  if ( !argv ||  !argv[0] ) {
    // no input
    vcl_cerr << "vul_arg_parse: Expected string, none is provided.\n";
    return -1;
  }

  if (argv[0]) {
    argmt->value_.clear();

    vcl_string str(argv[0]);
    //    vcl_cout << "Argv[0]: " << str << vcl_endl;

    vcl_string substr;
    for (unsigned i=0; i < str.length(); ++i) {
      if (str[i] == separator) {
        if (substr.length()) {
          argmt->value_.push_back(substr);
          substr.clear();
        }
      } else
        substr.push_back(str[i]);
    }

    if (substr.length())
      argmt->value_.push_back(substr);

    return 1;
  }
  else {
    vcl_cerr << __FILE__ ": no argument to string list option\n";
    return 0;
  }

  return 1;
}

