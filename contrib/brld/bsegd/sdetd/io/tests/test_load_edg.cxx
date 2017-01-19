#include <testlib/testlib_test.h>
#include <vcl_limits.h>
#include <dbdet/algo/dbdet_load_edg.h>
#include <dbtest_root_dir.h>


static const double tolerance=vcl_numeric_limits<double>::epsilon()*100;
static void test_load_edg();

MAIN( test_load_edg )
{
  START ("load .edg file");

  test_load_edg();

  SUMMARY();
}

void
test_load_edg()
{
  vcl_string root = dbtest_root_dir();
  vcl_string base_path = root + "/brcv/seg/dbdet/algo/tests/test_data/";
  vcl_string edge_path = base_path + "test_edg.edg";

  dbdet_edgemap_sptr edgemap;
  dbdet_load_edg(edge_path, true, 1.0, edgemap);
  
  TEST("Edgemap nCols", edgemap->ncols(), 101);
  TEST("Edgemap nRows", edgemap->nrows(), 101);
  TEST("Edgemap #edgels", edgemap->edgels.size(), 100);
 
  bool state = true;
  for(int i = 0; i < edgemap->edgels.size(); ++i)
  {
    dbdet_edgel * e = edgemap->edgels[i];
    int val = i+1;
    if(state)
      state = (e->pt.x() == val && e->pt.y() == val && e->tangent == dbdet_angle0To2Pi(val) && e->strength == val);
    else
      break;
   }
   TEST("Test edgels pos, tangent and strength", state, true);
}


