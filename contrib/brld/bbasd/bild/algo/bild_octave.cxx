// This is basic/bild/algo/bild_octave.cxx

//:
// \file
// \brief
// \author Based on original code by  Firat Kalaycilar (@lems.brown.edu)
// \date Feb 24, 2010

#include "bild_octave.h"

#include <vcl_iostream.h>

vil_image_resource_sptr bild_convert_octave_array_to_image_resource(const buld_octave_uint8_array& oct_array)
{
    dim_vector dv = oct_array.dims();
    int height = dv(0);
    int width = dv(1);
    int nbands = 1;
    if(dv.length() == 3)
    {
        nbands = dv(2);
    }
    vil_image_resource_sptr img_ptr = vil_new_image_resource(width, height, nbands, VIL_PIXEL_FORMAT_BYTE);
    vil_image_view<vxl_byte> ret_image = *(img_ptr->get_view());
    for(int b = 0; b < nbands; b++)
    {
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                ret_image(x, y, b) = oct_array(y, x, b);
            }
        }
    }
    return img_ptr;
}

vil_image_resource_sptr bild_octave_imresize(const buld_octave_value& image_src, double factor, const vcl_string& interp)
{
    buld_octave_argument_list function_arguments;
    buld_octave_argument_list return_argument;
    function_arguments(0) = image_src;
    function_arguments(1) = factor;
    function_arguments(2) = interp.c_str();
    buld_octave.run("", "imresize", function_arguments, return_argument);
    return bild_convert_octave_array_to_image_resource(buld_octave_value_to_octave_uint8_array(return_argument(0)));
}

vil_image_resource_sptr bild_octave_imresize(const buld_octave_value& image_src, double factor)
{
    return bild_octave_imresize(image_src, factor, "nearest");
}
