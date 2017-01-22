//:
// \file
// \brief Base Class for Object Descriptions
//        Ground Truth files are generated using these classes,
//        If ground truth is not available for an object, then the file should be empty but appropriately formatted 
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
// \date 10/03/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim


#if !defined(_DBORL_DESCRIPTION_BASE_H)
#define _DBORL_DESCRIPTION_BASE_H

#include <vcl_map.h>
#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>
#include <vcl_iostream.h>

class dborl_object_type
{
public:
  enum type {
    image,
    point_cloud,
    vehicle_track,
    video
  };
};

class dborl_image_description;

class dborl_description_base : public vbl_ref_count
{
public:
  vcl_map<vcl_string, int> category_list_;
  virtual void b_read() = 0;
  virtual void b_write() = 0;

  virtual void write_xml(vcl_ostream& os) = 0;
  
  virtual unsigned get_object_type() = 0;

  void add_category(vcl_string cat) { category_list_[cat] = 1; }
  void add_to_category_cnt(vcl_string cat, int cnt) { category_list_[cat] = category_list_[cat] + cnt; }
  bool category_exists(vcl_string cat) { return category_list_.find(cat) != category_list_.end(); }

  inline bool has_single_category() { return category_list_.size() == 1; }
  inline vcl_string get_first_category() { return category_list_.begin()->first; }
  inline int get_category_cnt(vcl_string cat) { return category_list_[cat]; }  // be careful if this entry does not exist, then adds it to the map

  virtual dborl_description_base* cast_to_description_base() { return this; }
  virtual dborl_image_description* cast_to_image_description() = 0;
};

#endif  //_DBORL_DESCRIPTION_BASE_H
