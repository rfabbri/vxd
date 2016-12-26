// This is basic/dbul/dbul_octave.cxx

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 16, 2010


#include "dbul_octave.h"

#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

#include <vcl_cctype.h>
#include <vcl_sstream.h>
#include <vcl_cstdio.h>
#include <vcl_map.h>


#define DBUL_GET_EMPTY_ARRAY(T, get_method) \
        int ndims = sizes.size(); \
        dbul_octave_argument_list function_arguments;\
        for(int i = 0; i < ndims; i++)\
        {\
            function_arguments(i) = sizes[i];\
        }\
        function_arguments(ndims) = T;\
        dbul_octave_argument_list result;\
        dbul_octave.run("", "zeros", function_arguments, result);\
        return result(0).get_method();


dbul_octave_wrapper dbul_octave;

dbul_octave_wrapper::dbul_octave_wrapper()
{
    is_activated_ = false;
}

dbul_octave_wrapper::~dbul_octave_wrapper()
{
    if(is_activated_)
    {
        do_octave_atexit ();
    }
}

void dbul_octave_wrapper::activate()
{
    const char * argvv [] = {"" , "--silent"};
    octave_main (2, (char **) argvv, true);
    dbul_octave_argument_list function_arguments;
    function_arguments(0) = "off";
    feval("warning", function_arguments, 1);
    is_activated_ = true;
}

void dbul_octave_wrapper::add_path(const vcl_string& path)
{
    dbul_octave_argument_list function_arguments;
    function_arguments(0) = path.c_str();
    feval("addpath", function_arguments, 1);
}
void dbul_octave_wrapper::rm_path(const vcl_string& path)
{
    dbul_octave_argument_list function_arguments;
    function_arguments(0) = path.c_str();
    feval("rmpath", function_arguments, 1);
}

void dbul_octave_wrapper::run(const vcl_string& path, const vcl_string& name, dbul_octave_argument_list& inargs, dbul_octave_argument_list& outargs)
{
    if(!is_activated_)
    {
        activate();
    }
    if(path != "")
        add_path(path);
    outargs = feval(name, inargs, 1);
    if(path != "")
        rm_path(path);
}

dbul_octave_double_array dbul_octave_get_empty_double_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("double", array_value)
}

dbul_octave_uint8_array dbul_octave_get_empty_uint8_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint8", uint8_array_value)
}

dbul_octave_uint16_array dbul_octave_get_empty_uint16_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint16", uint16_array_value)
}

dbul_octave_uint32_array dbul_octave_get_empty_uint32_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint32", uint32_array_value)
}

vcl_string trim_whitespaces(const vcl_string& s)
{
    int b;
    int e;
    for(b = 0; b < s.length(); b++)
    {
        if(!vcl_isspace(s[b]))
        {
            break;
        }
    }
    for(e = s.length()-1; e > -1; e--)
    {
        if(!vcl_isspace(s[e]))
        {
            break;
        }
    }
    if(e == -1 || b == s.length())
    {
        return "";
    }
    return s.substr(b, e - b + 1);
}

bool is_numeric( vcl_string pszInput)
{
    vcl_istringstream iss( pszInput );
    double dTestSink;
    iss >> dTestSink;
    if ( ! iss )
        return false;
    return ( iss.rdbuf()->in_avail() == 0 );
}

bool dbul_octave_convert_xml_to_octave_value(const bxml_data_sptr& root_xml, dbul_octave_value& ret)
{
    bxml_element* root_element = static_cast<bxml_element*>(root_xml.ptr());
    dbul_octave_struct ret_struct;
    dbul_octave_struct attributes;
    bxml_element::const_attr_iterator ait = root_element->attr_begin();
    for(int i = 0; i < root_element->num_attributes(); i++)
    {
        attributes.assign(ait->first, ait->second);
        ++ait;
    }
    if(root_element->num_attributes() > 0)
    {
        ret_struct.assign("ATTRIBUTE", attributes);
    }
    bxml_element::const_data_iterator dit = root_element->data_begin();
    bool ret_status = true;

    int number_of_text = 0;
    dbul_octave_value content;
    vcl_map<vcl_string, vcl_vector<dbul_octave_value> > temp_map;
    for(int i = 0; i < root_element->num_data(); ++dit, i++)
    {
        if(dit->ptr()->type() == bxml_data::ELEMENT)
        {
            dbul_octave_value child_value;
            ret_status = ret_status && dbul_octave_convert_xml_to_octave_value(*dit, child_value);
            if(!ret_status)
            {
                return false;
            }
            bxml_element* child_element = static_cast<bxml_element*>(dit->ptr());
            temp_map[child_element->name()].push_back(child_value);
        }
        else
        {
            bxml_text* child_text = static_cast<bxml_text*>(dit->ptr());
            vcl_string data = trim_whitespaces(child_text->data());
            if(data == "")
            {
                continue;
            }
            number_of_text++;
            if(is_numeric(data))
            {
                double double_data;
                vcl_sscanf(data.c_str(), "%lf", &double_data);
                content = double_data;
            }
            else
            {
                content = data;
            }
        }
    }
    if(number_of_text == 1)
    {
        if(root_element->num_data() == 1 && root_element->num_attributes() == 0)
        {
            ret = content;
            return true;
        }
        else
        {
            ret_struct.assign("CONTENT", content);
        }
    }
    else if(number_of_text > 1)
    {
        return false;
    }
    vcl_map<vcl_string, vcl_vector<dbul_octave_value> >::iterator it;
    for(it = temp_map.begin(); it != temp_map.end(); it++)
    {
        if(it->second.size() == 1)
        {
            ret_struct.assign(it->first, it->second[0]);
        }
        else
        {
            dbul_octave_argument_list values;
            for(int k = 0; k < it->second.size(); k++)
            {
                values(k) = it->second[k];
            }
            ret_struct.assign(it->first, dbul_octave_value(dbul_octave_cell(values)));
        }
    }
    ret = ret_struct;
    return ret_status;
}

