// This is dbul/dbul_dir_file.cxx

#include <vcl_cassert.h>
#include <vcl_algorithm.h>
#include <dbul/dbul_dir_file.h>
#include <vcl_fstream.h>

#define FILENAME_HUGE_INT 1000

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the directory "abc\def"
vcl_string dbul_get_dir (const vcl_string& filename) 
{
    int end;
    end=filename.find_last_of('\\');
    return filename.substr (0, end);
}

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the file prefix "prefix"
vcl_string dbul_get_prefix (const vcl_string& filename) 
{
    int start=filename.find_last_of ('\\');
    int end1 = filename.find_first_of ('-');
    int end2 = filename.find_first_of ('.');
    if (end1 == -1)
        end1 = FILENAME_HUGE_INT;
    if (end2 == -1)
        end2 = FILENAME_HUGE_INT;
    assert (end1 != FILENAME_HUGE_INT || end2 != FILENAME_HUGE_INT);
    int end = (end1 < end2) ? end1 : end2;
    return filename.substr (start+1, end-start-1);
}

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the dir-file prefix "abc\def\prefix"
vcl_string dbul_get_dir_prefix (const vcl_string& filename) 
{
    int end1 = filename.find_first_of ('-');
    int end2 = filename.find_first_of ('.');
    if (end1 == -1)
        end1 = FILENAME_HUGE_INT;
    if (end2 == -1)
        end2 = FILENAME_HUGE_INT;

    if (end1 == FILENAME_HUGE_INT && end2 == FILENAME_HUGE_INT) // ||
        return filename; //error

    int end = (end1 < end2) ? end1 : end2;
    return filename.substr (0, end);
}

//: For the input "abc\def\prefix_ghi_jkl-mno.txt"
//  return the dir-file prefix "abc\def\prefix"
vcl_string dbul_get_dir_prefix2 (const vcl_string& filename) 
{
    int end1 = filename.find_first_of ('_');
    int end2 = filename.find_first_of ('.');
    if (end1 == -1)
        end1 = FILENAME_HUGE_INT;
    if (end2 == -1)
        end2 = FILENAME_HUGE_INT;

    if (end1 == FILENAME_HUGE_INT && end2 == FILENAME_HUGE_INT) // ||
        return filename; //error

    int end = (end1 < end2) ? end1 : end2;
    return filename.substr (0, end);
}

//Return true for the case of filename contains the parent directory ('..') 
//and there is no suffix, such as "../../abc\def\prefix-ghi-jkl-mno".
bool is_par_dir (const vcl_string& str)
{
    bool b_par_dir = true;
    for (unsigned int i=0; i<str.size(); i++) {
        //check if any character not '.', '\\', or '/'.
        if (str[i] != '.' && str[i] != '\\' && str[i] != '/')
            b_par_dir = false;
    }

    return b_par_dir;
}

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the file prefix "abc\def\prefix-ghi-jkl-mno"
vcl_string dbul_get_dir_file (const vcl_string& filename)
{  
    int end=filename.find_last_of ('.');

    //Handle the case of filename contains the parent directory ('..') with no suffix.
    vcl_string str = filename.substr (0, end);
    if (is_par_dir (str))
        return filename;
    else
        return filename.substr (0, end);
}

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the file name "prefix-ghi-jkl-mno.txt"
vcl_string dbul_get_file (const vcl_string& filename)
{
    int start1 = filename.find_last_of ('\\');
    int start2 = filename.find_last_of ('/');
    int start = vcl_max (start1, start2);
    int end=filename.find_last_of ('\0');
    return filename.substr (start+1, end);
}

//: For the input "abc\def\prefix-ghi-jkl-mno.txt"
//  return the file suffix ".txt"
//  For the input "..\..\abc\def\prefix-ghi-jkl-mno", return "".
vcl_string dbul_get_suffix (const vcl_string& filename)
{
    int start,end;
    start=filename.find_last_of ('.');
    end=filename.find_last_of ('\0');

    if (start == end)
        return "\0";
    else {
        //Handle the case of filename contains the parent directory ('..') with no suffix.
        vcl_string str = filename.substr (0, start);
        if (is_par_dir (str))
            return "\0";
        else
            return filename.substr (start, end);
    }
}

vcl_string dbul_get_str_prior (const vcl_string& str, const vcl_string& sub)
{
    int end = str.find (sub); //find_last_of
    return str.substr (0, end);
}

vcl_string remove_commas (vcl_string& filename)
{
    int  start = filename.find_first_of( '.' ) ;
    filename.replace( start, 1, "," );
    return filename;
}

bool dbul_copy_file(const vcl_string& inf, const vcl_string& outf)
{
    char * buffer;
    long size;

    vcl_ifstream infile (inf.c_str(), vcl_ifstream::binary);
    vcl_ofstream outfile (outf.c_str(), vcl_ofstream::binary);
    if(!infile.is_open() || !outfile.is_open())
    {
        return false;
    }

    // get size of file
    infile.seekg(0, vcl_ifstream::end);
    size = infile.tellg();
    infile.seekg(0);

    // allocate memory for file content
    buffer = new char[size];

    // read content of infile
    infile.read(buffer, size);

    // write to outfile
    outfile.write(buffer, size);

    // release dynamically-allocated memory
    delete[] buffer;

    outfile.close();
    infile.close();

    return true;
}

