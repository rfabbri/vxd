// This is basic/bild/pro/bild_morphology_filters.h
#ifndef bild_morphology_filters_h_
#define bild_morphology_filters_h_

//:
// \file
// \brief Filters for morphological operations
// \author Based on original code by  Matt Leotta (@lems.brown.edu)
// \date 6/12/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbpro/dbpro_process.h>
#include <vil/algo/vil_structuring_element.h>


//: Filter for binary erosion
class bild_binary_erode_filter : public dbpro_filter
{
  public:
    //: Constructor
    bild_binary_erode_filter(const vil_structuring_element& s) : se_(s) {}
    //: Destructor
    virtual ~bild_binary_erode_filter() {}

    //: Execute this process
    dbpro_signal execute();

  private:
    vil_structuring_element se_;

};


//: Filter for binary dilation
class bild_binary_dilate_filter : public dbpro_filter
{
  public:
    //: Constructor
    bild_binary_dilate_filter(const vil_structuring_element& s) : se_(s) {}
    //: Destructor
    virtual ~bild_binary_dilate_filter() {}

    //: Execute this process
    dbpro_signal execute();

  private:
    vil_structuring_element se_;

};

#endif
