// This is basic/dbul/dbul_matlab.h

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Mar 24, 2010
// Implementation is incomplete. Required features will be added as needed.

#ifndef dbul_matlab_H_
#define dbul_matlab_H_

#include <engine.h>
#include <vcl_string.h>
#include <vcl_vector.h>

typedef Engine dbul_matlab_engine;

/*typedef Matrix dbul_matlab_double_matrix;

typedef NDArray dbul_matlab_double_array;

typedef uint8NDArray dbul_matlab_uint8_array;
typedef uint16NDArray dbul_matlab_uint16_array;
typedef uint32NDArray dbul_matlab_uint32_array;
typedef uint64NDArray dbul_matlab_uint64_array;

typedef int8NDArray dbul_matlab_int8_array;
typedef int16NDArray dbul_matlab_int16_array;
typedef int32NDArray dbul_matlab_int32_array;
typedef int64NDArray dbul_matlab_int64_array;

typedef Octave_map dbul_matlab_struct;
typedef Cell dbul_matlab_cell;

typedef octave_value_list dbul_matlab_argument_list;
typedef octave_value dbul_matlab_value;*/

class dbul_matlab_wrapper
{
private:
    void add_path(const vcl_string& path);
    void rm_path(const vcl_string& path);
    void activate();

    dbul_matlab_engine* engine_;
    bool is_activated_;
public:
    dbul_matlab_wrapper();
    ~dbul_matlab_wrapper();
    void run(const vcl_string& path, const vcl_string& name, vcl_vector<mxArray*>& inargs, vcl_vector<mxArray*>& outargs);
};

/*dbul_matlab_double_array dbul_matlab_get_empty_double_array(vcl_vector<int>& sizes);
dbul_matlab_uint8_array dbul_matlab_get_empty_uint8_array(vcl_vector<int>& sizes);
dbul_matlab_uint16_array dbul_matlab_get_empty_uint16_array(vcl_vector<int>& sizes);
dbul_matlab_uint32_array dbul_matlab_get_empty_uint32_array(vcl_vector<int>& sizes);*/

extern dbul_matlab_wrapper dbul_matlab;

#endif /* dbul_matlab_H_ */
