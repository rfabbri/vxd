// This is basic/dbul/dbul_matlab.cxx

//:
// \file
// \brief
// \author Firat Kalaycilar (firat@lems.brown.edu)
// \date Feb 16, 2010


#include "dbul_matlab.h"

dbul_matlab_wrapper dbul_matlab;

dbul_matlab_wrapper::dbul_matlab_wrapper()
{
    is_activated_ = false;
    engine_ = 0;
}

dbul_matlab_wrapper::~dbul_matlab_wrapper()
{
    if(is_activated_)
    {
        engClose(engine_);
    }
}

void dbul_matlab_wrapper::activate()
{
    if(engine_ = engOpen("matlab -nosplash")) {
        is_activated_ = true;
    }
}

void dbul_matlab_wrapper::add_path(const vcl_string& path)
{
    vcl_string cmd = vcl_string("addpath ") + path;
    engEvalString(engine_, cmd.c_str());
}

void dbul_matlab_wrapper::rm_path(const vcl_string& path)
{
    vcl_string cmd = vcl_string("rmpath ") + path;
    engEvalString(engine_, cmd.c_str());
}

void dbul_matlab_wrapper::run(const vcl_string& path, const vcl_string& name, vcl_vector<mxArray*>& inargs, vcl_vector<mxArray*>& outargs)
{
    if(!is_activated_)
    {
        activate();
    }
    if(path != "")
        add_path(path);
    //outargs = feval(name, inargs, 1);
    if(path != "")
        rm_path(path);
}

/*dbul_matlab_double_array dbul_matlab_get_empty_double_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("double", array_value)
}

dbul_matlab_uint8_array dbul_matlab_get_empty_uint8_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint8", uint8_array_value)
}

dbul_matlab_uint16_array dbul_matlab_get_empty_uint16_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint16", uint16_array_value)
}

dbul_matlab_uint32_array dbul_matlab_get_empty_uint32_array(vcl_vector<int>& sizes)
{
    DBUL_GET_EMPTY_ARRAY("uint32", uint32_array_value)
}*/
