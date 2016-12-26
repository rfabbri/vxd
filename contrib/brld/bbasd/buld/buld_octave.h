// This is bbasd/buld/buld_octave.h

//:
// \file
// \brief
// \author Based on original code by  Firat Kalaycilar (@lems.brown.edu)
// \date Feb 16, 2010
// Implementation is incomplete. Required features will be added as needed.

#ifndef BULD_OCTAVE_H_
#define BULD_OCTAVE_H_

#include <octave/oct.h>
#include <octave/oct-map.h>
#include <vcl_string.h>
#include <vcl_vector.h>
#include <bxml/bxml_document.h>

#define buld_octave_value_to_double(x) (x.scalar_value())
#define buld_octave_value_to_string(x) (x.string_value())
#define buld_octave_value_to_octave_double_matrix(x) (x.matrix_value())
#define buld_octave_value_to_octave_double_array(x) (x.array_value())
#define buld_octave_value_to_octave_uint8_array(x) (x.uint8_array_value())

typedef Matrix buld_octave_double_matrix;

typedef NDArray buld_octave_double_array;

typedef uint8NDArray buld_octave_uint8_array;
typedef uint16NDArray buld_octave_uint16_array;
typedef uint32NDArray buld_octave_uint32_array;
typedef uint64NDArray buld_octave_uint64_array;

typedef int8NDArray buld_octave_int8_array;
typedef int16NDArray buld_octave_int16_array;
typedef int32NDArray buld_octave_int32_array;
typedef int64NDArray buld_octave_int64_array;

typedef Octave_map buld_octave_struct;
typedef Cell buld_octave_cell;

typedef octave_value_list buld_octave_argument_list;
typedef octave_value buld_octave_value;

class buld_octave_wrapper
{
private:
    void add_path(const vcl_string& path);
    void rm_path(const vcl_string& path);
    void activate();

    bool is_activated_;
public:
    buld_octave_wrapper();
    ~buld_octave_wrapper();
    void run(const vcl_string& path, const vcl_string& name, buld_octave_argument_list& inargs, buld_octave_argument_list& outargs);
};

buld_octave_double_array buld_octave_get_empty_double_array(vcl_vector<int>& sizes);
buld_octave_uint8_array buld_octave_get_empty_uint8_array(vcl_vector<int>& sizes);
buld_octave_uint16_array buld_octave_get_empty_uint16_array(vcl_vector<int>& sizes);
buld_octave_uint32_array buld_octave_get_empty_uint32_array(vcl_vector<int>& sizes);

bool buld_octave_convert_xml_to_octave_value(const bxml_data_sptr& root_xml, buld_octave_value& ret);

extern buld_octave_wrapper buld_octave;

#endif /* BULD_OCTAVE_H_ */
