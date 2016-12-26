// This is dbul_arg.h
#ifndef dbul_h
#define dbul_h
//:
//\file
//\brief Extensions to vul_arg
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 04/19/09 11:39:24 EDT
//


#include <vul/vul_arg.h>
//: Specialization of vul_arg for a list of strings. In the commandline, the
// list of vcl_string is passed as a list whose delimitor is a space. Therefore,
// each vcl_string should not have a space in it. In bash, this looks like:
//
// shellprompt$    mycmd -stringlist_parameter  "string1 string2 string3"
//   
// The quotes are necessary so that the shell interpret all string1-string3 as a
// single parameter.
//
// The strings are parsed into a vcl_vector<vcl_string>
//
// \todo generalize for an arbitrary delimiter.
//
template<> void settype(vul_arg<vcl_vector<vcl_string> > &arg);
template<> void print_value(vcl_ostream &s, vul_arg<vcl_vector<vcl_string> > const &argmt);
template<> int parse(vul_arg<vcl_vector<vcl_string> >* argmt, char ** argv);


#endif // dbul_h

