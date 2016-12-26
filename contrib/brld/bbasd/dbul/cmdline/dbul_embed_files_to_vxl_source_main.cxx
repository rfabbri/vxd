// This is basic/dbul/cmdline/dbul_embed_files_to_vxl_source_main.cxx

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 09, 2010

//  -i ~/Desktop/input.txt -h /home/firat/lemsvxl/src/contrib/firat/any2cpp/test/test.h -c /home/firat/lemsvxl/src/contrib/firat/any2cpp/test/test.cxx -e extractTestFiles -d deleteTestFiles

#include<vcl_cstdlib.h>
#include<vcl_ctime.h>
#include<vcl_iostream.h>
#include<vcl_string.h>
#include<vcl_vector.h>

#include<vul/vul_file.h>
#include<vul/vul_arg.h>

#include "dbul_embed_files_to_vxl_source_utils.h"

int main(int argn, char* args[])
{
    // Arguments
    vul_arg_info_list arg_list;
    vul_arg<vcl_string> arg_list_of_files(arg_list, "-i", "Input: path of the text file containing the list of files to be embedded.", "/path/input.txt");
    vul_arg<vcl_string> arg_output_h_file_name(arg_list, "-h", "Output: h-file", "out.h");
    vul_arg<vcl_string> arg_output_cpp_file_name(arg_list, "-c", "Output: cpp-file", "out.cpp");
    vul_arg<vcl_string> arg_extract_command(arg_list, "-e", "Name of the file extraction command", "extractFiles");
    vul_arg<vcl_string> arg_delete_command(arg_list, "-d", "Name of the file deletion command", "deleteFiles");
    vul_arg<bool > arg_print_usage_only(arg_list,"-usage", "print usage info and exit",false);
    vul_arg<bool > arg_print_help(arg_list,"-help", "print usage info and exit",false);

    vul_arg_include(arg_list);
    vul_arg_parse(argn, args);

    if (arg_print_help() || arg_print_usage_only())
    {
        vul_arg_display_usage_and_exit();
        return 0;
    }

    vcl_string input_file_name = arg_list_of_files();
    vcl_string output_h_file_name = arg_output_h_file_name();
    vcl_string output_cpp_file_name = arg_output_cpp_file_name();
    vcl_string extract_command = arg_extract_command();
    vcl_string delete_command = arg_delete_command();

    if(!vul_file_exists(input_file_name))
    {
        vcl_cerr << "Input file <" << input_file_name << "> cannot be found!" << vcl_endl;
        return 1;
    }

    vcl_string h_file_tag = getRandomHFileTag(32);

    vcl_vector<vcl_string> original_files;
    vcl_vector<vcl_string> variable_names;
    vcl_vector<int> data_sizes;
    vcl_string root_path;

    parseInputFile(input_file_name, original_files, variable_names, root_path);

    create_h_file(output_h_file_name, h_file_tag, extract_command, delete_command);
    create_cpp_file(output_cpp_file_name, output_h_file_name, root_path, original_files, variable_names, data_sizes, extract_command, delete_command);
    return 0;

}

