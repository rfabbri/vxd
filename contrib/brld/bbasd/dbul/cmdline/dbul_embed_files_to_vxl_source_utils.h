// This is basic/dbul/cmdline/dbul_embed_files_to_vxl_source_utils.h

#ifndef DBUL_EMBED_FILES_TO_VXL_SOURCE_UTILS_H_
#define DBUL_EMBED_FILES_TO_VXL_SOURCE_UTILS_H_

//:
// \file
// \brief command line utilities for shock-based segmentation
//
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 09, 2010

#include<vcl_string.h>
#include<vcl_vector.h>

vcl_string getRandomVariableName(int len);

vcl_string getRandomHFileTag(int len);

void parseInputFile(vcl_string& input_file_name, vcl_vector<vcl_string>& original_files, vcl_vector<vcl_string>& variable_names, vcl_string& root_path);

int getArrayString(vcl_string filename, vcl_string& varname, vcl_string& array_string);

void create_h_file(vcl_string& filename, vcl_string& h_file_tag, vcl_string& extract_command, vcl_string& delete_command);

void create_cpp_file(vcl_string& output_cpp_file_name, vcl_string& output_h_file_name, vcl_string& root_path, vcl_vector<vcl_string>& original_files, vcl_vector<vcl_string>& variable_names, vcl_vector<int>& data_sizes, vcl_string& extract_command, vcl_string& delete_command);

#endif /* DBUL_EMBED_FILES_TO_VXL_SOURCE_UTILS_H_ */
