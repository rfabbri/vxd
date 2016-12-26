// This is basic/dbul/dbul_handle_files_embedded_in_vxl_source.h

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 09, 2010

#ifndef DBUL_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_
#define DBUL_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_

void dbul_default_extract_embedded_file(char* array, int length, const char* out_dir, char* file_name);
void dbul_default_delete_extracted_file(const char* out_dir);
bool dbul_is_file_extraction_in_progress(const char* out_dir);
void dbul_create_file_extraction_lock(const char* out_dir);
void dbul_remove_file_extraction_lock(const char* out_dir);

#endif /* DBUL_HANDLE_FILES_EMBEDDED_IN_VXL_SOURCE_H_ */
