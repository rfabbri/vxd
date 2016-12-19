// This is bvis1_util.h
#ifndef bvis1_util_h
#define bvis1_util_h
//:
//\file
//\brief Various Utilities
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 05/14/2008 03:19:50 AM EDT
//
#include <vcl_string.h>
#include <vcl_iostream.h>

struct bvis1_util {

  //: Removes prefixes such as file:// before the filename.
  static void handle_file_name_prefix(vcl_string &file_name)
  {
    if (file_name.size() >= 7) {
      vcl_string p(file_name);
      vcl_cout << "Original file name: " << p << vcl_endl;
      vcl_string file_prefix("file://");
    
      file_name.clear();
      bool has_prefix = true;
      unsigned i;
      for (i=0; i < 7 && i < p.size(); ++i) {
        if (p[i] != file_prefix[i]) {
          has_prefix=false;
          break;
        }
      }
    
      if (has_prefix && (i < p.size() || i == 7)) {
        // has 'file://'   now copy:
        for (unsigned k=7; k < p.size(); ++k) {
          file_name.push_back(p[k]);
        }
      } else 
        file_name = p;
    
      vcl_cout << "Final file name: " << file_name << vcl_endl;
    }
  }

};


#endif // bvis1_util_h

