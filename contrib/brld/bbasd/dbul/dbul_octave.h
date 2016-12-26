// This is basic/dbul/dbul_octave.h

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 16, 2010
// Implementation is incomplete. Required features will be added as needed.

#ifndef DBUL_OCTAVE_H_
#define DBUL_OCTAVE_H_

#include <octave/oct.h>
#include <octave/oct-map.h>
#include <vcl_string.h>
#include <vcl_vector.h>
#include <bxml/bxml_document.h>

#define dbul_octave_value_to_double(x) (x.scalar_value())
#define dbul_octave_value_to_string(x) (x.string_value())
#define dbul_octave_value_to_octave_double_matrix(x) (x.matrix_value())
#define dbul_octave_value_to_octave_double_array(x) (x.array_value())
#define dbul_octave_value_to_octave_uint8_array(x) (x.uint8_array_value())

typedef Matrix dbul_octave_double_matrix;

typedef NDArray dbul_octave_double_array;

typedef uint8NDArray dbul_octave_uint8_array;
typedef uint16NDArray dbul_octave_uint16_array;
typedef uint32NDArray dbul_octave_uint32_array;
typedef uint64NDArray dbul_octave_uint64_array;

typedef int8NDArray dbul_octave_int8_array;
typedef int16NDArray dbul_octave_int16_array;
typedef int32NDArray dbul_octave_int32_array;
typedef int64NDArray dbul_octave_int64_array;

typedef Octave_map dbul_octave_struct;
typedef Cell dbul_octave_cell;

typedef octave_value_list dbul_octave_argument_list;
typedef octave_value dbul_octave_value;

class dbul_octave_wrapper
{
private:
    void add_path(const vcl_string& path);
    void rm_path(const vcl_string& path);
    void activate();

    bool is_activated_;
public:
    dbul_octave_wrapper();
    ~dbul_octave_wrapper();
    void run(const vcl_string& path, const vcl_string& name, dbul_octave_argument_list& inargs, dbul_octave_argument_list& outargs);
};

dbul_octave_double_array dbul_octave_get_empty_double_array(vcl_vector<int>& sizes);
dbul_octave_uint8_array dbul_octave_get_empty_uint8_array(vcl_vector<int>& sizes);
dbul_octave_uint16_array dbul_octave_get_empty_uint16_array(vcl_vector<int>& sizes);
dbul_octave_uint32_array dbul_octave_get_empty_uint32_array(vcl_vector<int>& sizes);

bool dbul_octave_convert_xml_to_octave_value(const bxml_data_sptr& root_xml, dbul_octave_value& ret);

extern dbul_octave_wrapper dbul_octave;

#endif /* DBUL_OCTAVE_H_ */
