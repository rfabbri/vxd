// This is basic/dbil/pro/dbil_blob_tracker_process.cxx

//:
// \file

#include "dbil_blob_tracker_process.h"

#include <vil/vil_image_resource.h>
#include <vil/vil_image_resource_sptr.h>
#include <vil/vil_convert.h>
#include <vil/vil_new.h>

#include <bpro1/bpro1_parameters.h>
#include <vidpro1/storage/vidpro1_image_storage.h>
#include <vidpro1/storage/vidpro1_image_storage_sptr.h>

#include <bil/bil_math.h>
#include <bil/algo/bil_blob_finder.h>

//: Constructor
dbil_blob_tracker_process::dbil_blob_tracker_process()
{
#if 0
  if( !parameters()->add( "Restore"    , "-restore" ,  false   ) 
    ) {
    vcl_cerr << "ERROR: Adding parameters in " __FILE__ << vcl_endl;
  }
#endif
}

//: Destructor
dbil_blob_tracker_process::~dbil_blob_tracker_process()
{
}

bpro1_process*
dbil_blob_tracker_process::clone() const
{
  return new dbil_blob_tracker_process(*this);
}

//: Return the name of this process
vcl_string 
dbil_blob_tracker_process::name()
{
  return "Blob Tracker";
}

int
dbil_blob_tracker_process::input_frames()
{
  return 1;
}

int
dbil_blob_tracker_process::output_frames()
{
  return 0;
}

vcl_vector< vcl_string >
dbil_blob_tracker_process::get_input_type()
{
  vcl_vector< vcl_string > to_return;
  to_return.push_back( "image" );
  return to_return;
}

vcl_vector< vcl_string >
dbil_blob_tracker_process::get_output_type()
{
  vcl_vector<vcl_string > to_return;
  //to_return.push_back( "image" );
  return to_return;
}


// Compute the area and center of a blob
static void dbil_blob_center(const vcl_vector<int>& bi,
                             const vcl_vector<int>& bj,
                             int& area, double& xc, double& yc)
{
  int x1 = bi.back(),  y1 = bj.back();
  int x2 = bi.front(), y2 = bj.front();

  //for degenerate blobs
  int xsum = x1, ysum = y1;

  int temp = x1*y2 - x2*y1;
  area = temp;
  int sx = temp*(x2 + x1);
  int sy = temp*(y2 + y1);
  for(unsigned int i=0; i<bi.size()-1; ++i){
    x1 = bi[i], y1 = bj[i];
    x2 = bi[i+1], y2 = bj[i+1];
    temp = x1*y2 - x2*y1;
    area += temp;
    sx += temp*(x2 + x1);
    sy += temp*(y2 + y1);
    xsum += x1; ysum += y1;
  }
  assert(area%2==0);
  area /= 2;
  if(area!=0)
  {
    xc = sx/(6*area);
    yc = sy/(6*area);
  }
  else
  {
    xc = double(xsum)/bi.size();
    yc = double(ysum)/bj.size();
  }
  if(area<0)
    area *= -1;
}


bool
dbil_blob_tracker_process::execute()
{
  if ( input_data_.size() != 1 ){
    vcl_cout << "In vidpro1_can_test_process::execute() - not exactly one" << " input image" << vcl_endl;
    return false;
  }
  clear_output();


  // get image from the storage class
  vidpro1_image_storage_sptr frame_image;
  frame_image.vertical_cast(input_data_[0][0]);

  vil_image_resource_sptr image_rsc = frame_image->get_image();
  vil_image_view<bool> img = image_rsc->get_view();
  if(!img)
    return false;

  //bool restore;
  //parameters()->get_value( "-restore" , restore );

  //vil_image_view<vxl_byte> result(img.ni(),img.nj(),1);
  //result.fill(0);

  static vil_image_view<bool> inv_img;
  bil_math_boolean_not(img, inv_img);

  bil_blob_finder bf;
  bf.set_work_image(inv_img);
  vcl_vector<int> bi, bj;
  //int blob_count = 0;
  int area=0;
  double xc=0.0, yc=0.0;

  bool detected = false;
  while (bf.next_4con_region(bi,bj)){
    dbil_blob_center(bi,bj,area,xc,yc);

    if(area < 100)
      continue;
    else
    {
      detected = true;
      break;
    }
    /*
    for(unsigned int i=0; i<bi.size(); ++i){
      result(bi[i],bj[i]) = 128;
    }
    int x = int(xc+.5), y = int(yc+.5);
    if(x<0) x=0;
    if(y<0) y=0;
    if(x>=result.ni()) x=result.ni()-1;
    if(y>=result.nj()) y=result.nj()-1;
    result(x,y) = 255;
    ++blob_count;
    */
  }
  //vcl_cout << "counted "<<blob_count<<" blobs" << vcl_endl;
  if(detected)
    vcl_cout << "object detected" << vcl_endl;
  else
    vcl_cout << "clear" << vcl_endl;


  //vidpro1_image_storage_sptr output_storage = vidpro1_image_storage_new();
  //output_storage->set_image(vil_new_image_resource_of_view(result));
  //output_data_[0].push_back(output_storage);

  return true;
}

bool
dbil_blob_tracker_process::finish()
{
  return true;
}
