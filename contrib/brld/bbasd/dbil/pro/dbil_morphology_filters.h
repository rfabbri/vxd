// This is basic/dbil/pro/dbil_morphology_filters.h
#ifndef dbil_morphology_filters_h_
#define dbil_morphology_filters_h_

//:
// \file
// \brief Filters for morphological operations
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 6/12/06
//
// \verbatim
//  Modifications
// \endverbatim


#include <dbpro/dbpro_process.h>
#include <vil/algo/vil_structuring_element.h>


//: Filter for binary erosion
class dbil_binary_erode_filter : public dbpro_filter
{
  public:
    //: Constructor
    dbil_binary_erode_filter(const vil_structuring_element& s) : se_(s) {}
    //: Destructor
    virtual ~dbil_binary_erode_filter() {}

    //: Execute this process
    dbpro_signal execute();

  private:
    vil_structuring_element se_;

};


//: Filter for binary dilation
class dbil_binary_dilate_filter : public dbpro_filter
{
  public:
    //: Constructor
    dbil_binary_dilate_filter(const vil_structuring_element& s) : se_(s) {}
    //: Destructor
    virtual ~dbil_binary_dilate_filter() {}

    //: Execute this process
    dbpro_signal execute();

  private:
    vil_structuring_element se_;

};

#endif
