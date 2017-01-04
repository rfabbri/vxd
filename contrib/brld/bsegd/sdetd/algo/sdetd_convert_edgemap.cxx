//This is sdet/edge/sdet_edgemap_to_image.cxx

//:
//\file


#include "sdetd_convert_edgemap.h"
#include <sdet/sdet_sel_utils.h>

//: Function to convert an edge map (floating elements) to a BW image
// "roi_box" is the region of the edge map that we to convert.
bool 
sdetd_convert_edgemap_to_image(const sdet_curve_fragment_graph& CFG, 
                               vil_image_view<vxl_byte >& bw_image,
                               const vgl_box_2d<int >& roi_box)
{
  // reset the size of the image
  bw_image.set_size(roi_box.width()+1, roi_box.height()+1);
  bw_image.fill(0);

  for (vcl_list<sdet_edgel_chain*>::const_iterator f_it = CFG.frags.begin(); 
    f_it != CFG.frags.end(); ++f_it)
  {
    sdet_edgel_chain* chain = (*f_it);

  //for now, if chain has sparse edges, it will appear as dotted lines, I build up a new chain with inserted new edges where sparse

  sdet_edgel_list new_chain;
  for (unsigned j=0; j<chain->edgels.size()-1;j++)
  {
	sdet_edgel* e1 = chain->edgels[j];
	new_chain.push_back(e1);
	sdet_edgel* e2 = chain->edgels[j+1];
	double dist = vgl_distance(e1->pt, e2->pt);
	double d_x = e2->pt.x() - e1->pt.x();
	double d_y = e2->pt.y() - e1->pt.y();

	if(dist>1)
	{
	  for (unsigned k=1; k<= int(dist); k++)
	  {
	    sdet_edgel* e_new = new sdet_edgel(*e1);
	    double new_x = e_new->pt.x() + k/dist*d_x;
	    double new_y = e_new->pt.y() + k/dist*d_y;
	    e_new->pt.set(new_x, new_y);
	    new_chain.push_back(e_new);
	  }
	}
  }
  new_chain.push_back(chain->edgels.back());

    for (unsigned j=0; j<new_chain.size(); j++)
    {
      vgl_point_2d<double > pt = new_chain[j]->pt;
      int pt_x = sdet_round(pt.x());
      int pt_y = sdet_round(pt.y());
      if (roi_box.contains(pt_x, pt_y))
      {
        int image_i = pt_x - roi_box.min_x();
        int image_j = pt_y - roi_box.min_y();
        bw_image(image_i, image_j) = 255;
      }
    }
  }
  

  return true;
}

//: Function to convert a subpixel edge map  to a BW image. 
// "roi_box" is the region of the edge map to convert.
bool 
sdetd_convert_edgemap_to_image(const sdet_edgemap& edgemap, 
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box)
{
  // reset the size of the image
  bw_image.set_size(roi_box.width()+1, roi_box.height()+1);
  bw_image.fill(0);

  for (vcl_vector<sdet_edgel *>::const_iterator 
       e_it = edgemap.edgels.begin(); 
       e_it != edgemap.edgels.end(); ++e_it)
  {
    sdet_edgel* e = *e_it;

    const vgl_point_2d<double > &pt = (e->pt);
    int pt_x = sdet_round(pt.x());
    int pt_y = sdet_round(pt.y());
    if (roi_box.contains(pt_x, pt_y))
    {
      int image_i = pt_x - roi_box.min_x();
      int image_j = pt_y - roi_box.min_y();
      bw_image(image_i, image_j) = 255;
#ifndef NDEBUG
      if (edgemap.edge_cells(pt_y,pt_x).empty()) {
        vcl_cout << "[ix,iy]: [" << pt_x << "," << pt_y << "]\n";
        vcl_cout << "x,y: " << pt.x() << ", " << pt.y() << vcl_endl;
      }
      assert(!edgemap.edge_cells(pt_y,pt_x).empty());
#endif
    }
  }

  return true;
}

bool 
sdetd_convert_edgemap_to_image(const sdet_edgemap& edgemap, 
                               vil_image_view<vxl_byte >& bw_image)
{
  vgl_box_2d<int > bbox;
  bbox.add(vgl_point_2d<int > (0, 0));
  bbox.add(vgl_point_2d<int > (edgemap.width()-1, edgemap.height()-1) );
  return sdetd_convert_edgemap_to_image(edgemap, bw_image, bbox);
}

//////////////////////

//: Function to convert an edge map (floating elements) to a grayscale image
// "roi_box" is the region of the edge map that we to convert.
bool
sdetd_convert_edgemap_to_grayscale_image(const sdet_curve_fragment_graph& CFG,
                               vil_image_view<vxl_byte >& bw_image,
                               const vgl_box_2d<int >& roi_box)
{
  // reset the size of the image
  bw_image.set_size(roi_box.width()+1, roi_box.height()+1);
  bw_image.fill(0);

  for (vcl_list<sdet_edgel_chain*>::const_iterator f_it = CFG.frags.begin();
    f_it != CFG.frags.end(); ++f_it)
  {
    sdet_edgel_chain* chain = (*f_it);

    for (unsigned j=0; j<chain->edgels.size(); j++)
    {
      vgl_point_2d<double > pt = chain->edgels[j]->pt;
      int pt_x = sdet_round(pt.x());
      int pt_y = sdet_round(pt.y());
      if (roi_box.contains(pt_x, pt_y))
      {
        int image_i = pt_x - roi_box.min_x();
        int image_j = pt_y - roi_box.min_y();
        bw_image(image_i, image_j) = 255 * chain->edgels[j]->strength;
      }
    }
  }


  return true;
}

//: Function to convert a subpixel edge map  to a grayscale image.
// "roi_box" is the region of the edge map to convert.
bool
sdetd_convert_edgemap_to_grayscale_image(const sdet_edgemap& edgemap,
                                    vil_image_view<vxl_byte >& bw_image,
                                    const vgl_box_2d<int >& roi_box)
{
  // reset the size of the image
  bw_image.set_size(roi_box.width()+1, roi_box.height()+1);
  bw_image.fill(0);

  for (vcl_vector<sdet_edgel *>::const_iterator
       e_it = edgemap.edgels.begin();
       e_it != edgemap.edgels.end(); ++e_it)
  {
    sdet_edgel* e = *e_it;

    const vgl_point_2d<double > &pt = (e->pt);
    int pt_x = sdet_round(pt.x());
    int pt_y = sdet_round(pt.y());
    if (roi_box.contains(pt_x, pt_y))
    {
      int image_i = pt_x - roi_box.min_x();
      int image_j = pt_y - roi_box.min_y();
      bw_image(image_i, image_j) = 255 * e->strength;
    }
  }

  return true;
}

bool
sdet_convert_edgemap_to_grayscale_image(const sdet_edgemap& edgemap,
                               vil_image_view<vxl_byte >& bw_image)
{
  vgl_box_2d<int > bbox;
  bbox.add(vgl_point_2d<int > (0, 0));
  bbox.add(vgl_point_2d<int > (edgemap.width()-1, edgemap.height()-1) );
  return sdetd_convert_edgemap_to_grayscale_image(edgemap, bw_image, bbox);
}

