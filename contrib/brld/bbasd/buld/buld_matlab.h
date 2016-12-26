// This is bbasd/buld/buld_matlab.h

//:
// \file
// \brief
// \author Based on original code by  Firat Kalaycilar (@lems.brown.edu)
// \date Mar 24, 2010
// Implementation is incomplete. Required features will be added as needed.

#ifndef buld_matlab_H_
#define buld_matlab_H_

#include <engine.h>
#include <vcl_string.h>
#include <vcl_vector.h>

typedef Engine buld_matlab_engine;

/*typedef Matrix buld_matlab_double_matrix;

typedef NDArray buld_matlab_double_array;

typedef uint8NDArray buld_matlab_uint8_array;
typedef uint16NDArray buld_matlab_uint16_array;
typedef uint32NDArray buld_matlab_uint32_array;
typedef uint64NDArray buld_matlab_uint64_array;

typedef int8NDArray buld_matlab_int8_array;
typedef int16NDArray buld_matlab_int16_array;
typedef int32NDArray buld_matlab_int32_array;
typedef int64NDArray buld_matlab_int64_array;

typedef Octave_map buld_matlab_struct;
typedef Cell buld_matlab_cell;

typedef octave_value_list buld_matlab_argument_list;
typedef octave_value buld_matlab_value;*/

class buld_matlab_wrapper
{
private:
    void add_path(const vcl_string& path);
    void rm_path(const vcl_string& path);
    void activate();

    buld_matlab_engine* engine_;
    bool is_activated_;
public:
    buld_matlab_wrapper();
    ~buld_matlab_wrapper();
    void run(const vcl_string& path, const vcl_string& name, vcl_vector<mxArray*>& inargs, vcl_vector<mxArray*>& outargs);
};

/*buld_matlab_double_array buld_matlab_get_empty_double_array(vcl_vector<int>& sizes);
buld_matlab_uint8_array buld_matlab_get_empty_uint8_array(vcl_vector<int>& sizes);
buld_matlab_uint16_array buld_matlab_get_empty_uint16_array(vcl_vector<int>& sizes);
buld_matlab_uint32_array buld_matlab_get_empty_uint32_array(vcl_vector<int>& sizes);*/

extern buld_matlab_wrapper buld_matlab;

#endif /* buld_matlab_H_ */
