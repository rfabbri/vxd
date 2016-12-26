// This is bbasd/buld/buld_handle_files_embedded_in_vxl_source.h

//:
// \file
// \brief
// \author Based on original code by  Firat Kalaycilar (@lems.brown.edu)
// \date Feb 09, 2010

#ifndef BULD_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_
#define BULD_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_

void buld_default_extract_embedded_file(char* array, int length, const char* out_dir, char* file_name);
void buld_default_delete_extracted_file(const char* out_dir);
bool buld_is_file_extraction_in_progress(const char* out_dir);
void buld_create_file_extraction_lock(const char* out_dir);
void buld_remove_file_extraction_lock(const char* out_dir);

#endif /* BULD_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_ */
