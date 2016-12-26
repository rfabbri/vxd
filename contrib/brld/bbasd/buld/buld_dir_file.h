// This is buld/buld_dir_file.h
#ifndef buld_dir_file_h_
#define buld_dir_file_h_
//:
// \file
// \brief Directory and file read/write utilities
// \author Based on original code by  Ming-Ching Chang
// \date   Mar 07, 2007
//
// \verbatim
// Modifications
// \endverbatim

#include <vcl_cstdio.h>
#include <vcl_string.h>

vcl_string buld_get_dir (const vcl_string& filename);
vcl_string buld_get_prefix (const vcl_string& filename);
vcl_string buld_get_dir_prefix (const vcl_string& filename);
vcl_string buld_get_dir_prefix2 (const vcl_string& filename);
vcl_string buld_get_dir_file (const vcl_string& filename);
vcl_string buld_get_file (const vcl_string& filename);
vcl_string buld_get_suffix (const vcl_string& filename);

vcl_string buld_get_str_prior (const vcl_string& str, const vcl_string& sub);

vcl_string remove_commas (vcl_string& filename);

bool buld_copy_file(const vcl_string& inf, const vcl_string& outf);

#endif

