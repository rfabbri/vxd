// This is dbsol/tests/test_file_io.cxx
// \author Nhon Trinh
// \date May 31, 2006

#include <testlib/testlib_test.h>
//#include <dbgl/algo/dbgl_distance.h>
//////#include <dbgl/algo/dbgl_biarc.h>
//#include <vcl_iostream.h>
//#include <vcl_cmath.h>
//////#include <vnl/vnl_math.h>

#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polygon_2d_sptr.h>

#include <dbsol/dbsol_file_io.h>


void test_file_io()
{
  vcl_vector<vsol_point_2d_sptr > pts;
  pts.push_back(new vsol_point_2d(0, 0));
  pts.push_back(new vsol_point_2d(1, 0));
  pts.push_back(new vsol_point_2d(1, 1));


  // TEST Polyline IO
  vsol_polyline_2d_sptr polyline = new vsol_polyline_2d(pts);
  vcl_string polyline_filename = "test_polyline.con";
  dbsol_save_con_file(polyline_filename.c_str(), polyline);


  vsol_spatial_object_2d_sptr loaded_contour = 
    dbsol_load_con_file(polyline_filename.c_str());

  bool polyline_io_success = false;
  if (loaded_contour)
  {
    if (loaded_contour->cast_to_curve())
    {
      if (loaded_contour->cast_to_curve()->cast_to_polyline())
      {
        vsol_polyline_2d_sptr loaded_polyline = 
          loaded_contour->cast_to_curve()->cast_to_polyline();
        if (loaded_polyline->size() == polyline->size())
        {
          bool same = true;
          for (unsigned int i=0; i<polyline->size(); ++i)
          {
            vsol_point_2d_sptr p1 = polyline->vertex(i);
            vsol_point_2d_sptr p2 = loaded_polyline->vertex(i);
            if (p1->x() != p2->x() || p1->y() != p2->y())
            {
              same = false;
              break;
            }
          }

          polyline_io_success = same;
        
        }
      }
    }
  }

  TEST("Polyline file io", polyline_io_success, true);


  // TEST Polygon IO
  vsol_polygon_2d_sptr polygon = new vsol_polygon_2d(pts);
  vcl_string polygon_filename = "test_polygon.con";
  dbsol_save_con_file(polygon_filename.c_str(), polygon);


  loaded_contour = dbsol_load_con_file(polygon_filename.c_str());

  bool polygon_io_success = false;
  if (loaded_contour)
  {
    if (loaded_contour->cast_to_region())
    {
      if (loaded_contour->cast_to_region()->cast_to_polygon())
      {
        vsol_polygon_2d_sptr loaded_polygon = 
          loaded_contour->cast_to_region()->cast_to_polygon();
        if (loaded_polygon->size() == polygon->size())
        {
          bool same = true;
          for (unsigned int i=0; i<polygon->size(); ++i)
          {
            vsol_point_2d_sptr p1 = polygon->vertex(i);
            vsol_point_2d_sptr p2 = loaded_polygon->vertex(i);
            if (p1->x() != p2->x() || p1->y() != p2->y())
            {
              same = false;
              break;
            }
          }

          polygon_io_success = same;
        }
      }
    }
  }

  TEST("Polygon file io", polygon_io_success, true);

  return;

}


//: Test functions of biarc class
MAIN( test_file_io )
{
  START ("Test file io functions for vsol objects");
  test_file_io();
  SUMMARY();
}
