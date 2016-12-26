/*
 * dbil_octave.h
 *
 *  Created on: Feb 24, 2010
 *      Author: firat
 */

#ifndef DBIL_OCTAVE_H_
#define DBIL_OCTAVE_H_

#include <dbul/dbul_octave.h>
#include <vil/vil_image_view.h>
#include <vil/vil_new.h>
#include <vcl_cmath.h>
#include <vcl_vector.h>

#define DBIL_CONVERT_IMAGE_VIEW_TO_ARRAY \
        for(int b = 0; b < nbands; b++) \
        {\
            for(int y = 0; y < height; y++)\
            {\
                for(int x = 0; x < width; x++)\
                {\
                    array(y, x, b) = image_src(x, y, b);\
                }\
            }\
        }

template <class T>
dbul_octave_value dbil_convert_image_view_to_octave_value(const vil_image_view<T>& image_src);

vil_image_resource_sptr dbil_convert_octave_array_to_image_resource(const dbul_octave_uint8_array& oct_array);

template <class T>
vil_image_resource_sptr dbil_octave_imresize(const vil_image_view<T>& image_src, double factor, const vcl_string& interp);

template <class T>
vil_image_resource_sptr dbil_octave_imresize(const vil_image_view<T>& image_src, double factor);

vil_image_resource_sptr dbil_octave_imresize(const dbul_octave_value& image_src, double factor, const vcl_string& interp);

vil_image_resource_sptr dbil_octave_imresize(const dbul_octave_value& image_src, double factor);

template <class T>
vcl_vector<vil_image_resource_sptr> dbil_octave_create_image_pyramid(const vil_image_view<T>& image_src, const vcl_string& interp, double step, int num_steps);


// IMPLEMENTATION

template <class T>
dbul_octave_value dbil_convert_image_view_to_octave_value(const vil_image_view<T>& image_src)
{
    int width = image_src.ni();
    int height = image_src.nj();
    int nbands = image_src.nplanes();
    vcl_vector<int> sizes(3);
    sizes[0] = height; sizes[1] = width; sizes[2] = nbands;
    vil_pixel_format pf = image_src.pixel_format();
    if(pf == VIL_PIXEL_FORMAT_BYTE)
    {
        dbul_octave_uint8_array array = dbul_octave_get_empty_uint8_array(sizes);
        DBIL_CONVERT_IMAGE_VIEW_TO_ARRAY
        return dbul_octave_value(array);
    }
    else if(pf == VIL_PIXEL_FORMAT_UINT_16)
    {
        dbul_octave_uint16_array array = dbul_octave_get_empty_uint16_array(sizes);
        DBIL_CONVERT_IMAGE_VIEW_TO_ARRAY
        return dbul_octave_value(array);
    }
    else if(pf == VIL_PIXEL_FORMAT_UINT_32)
    {
        dbul_octave_uint32_array array = dbul_octave_get_empty_uint32_array(sizes);
        DBIL_CONVERT_IMAGE_VIEW_TO_ARRAY
        return dbul_octave_value(array);
    }
    else if(pf == VIL_PIXEL_FORMAT_DOUBLE)
    {
        dbul_octave_double_array array = dbul_octave_get_empty_double_array(sizes);
        DBIL_CONVERT_IMAGE_VIEW_TO_ARRAY
        return dbul_octave_value(array);
    }

}

template <class T>
vil_image_resource_sptr dbil_octave_imresize(const vil_image_view<T>& image_src, double factor, const vcl_string& interp)
{
    dbul_octave_value octave_image = dbil_convert_image_view_to_octave_value<T>(image_src);
    dbil_octave_imresize(octave_image, factor, interp);
}

template <class T>
vil_image_resource_sptr dbil_octave_imresize(const vil_image_view<T>& image_src, double factor)
{
    return dbil_octave_imresize<T>(image_src, factor, "nearest");
}

template <class T>
vcl_vector<vil_image_resource_sptr> dbil_octave_create_image_pyramid(const vil_image_view<T>& image_src, const vcl_string& interp, double step, int num_steps)
{
    vcl_vector<vil_image_resource_sptr> pyr_vec;
    dbul_octave_value octave_image = dbil_convert_image_view_to_octave_value<T>(image_src);
    for(int i = 0; i < num_steps; i++)
    {
        double scale = 1 / vcl_pow(step, i);
        vil_image_resource_sptr im_scaled = dbil_octave_imresize(octave_image, scale, interp);
        pyr_vec.push_back(im_scaled);
    }
    return pyr_vec;
}

#endif /* DBIL_OCTAVE_H_ */
