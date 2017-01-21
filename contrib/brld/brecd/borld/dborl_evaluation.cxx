// This is file rec/dborl/dborl_evaluation.cxx

//:
// \file


#include "dborl_evaluation.h"

#include <dborl/dborl_image_description_utils.h>
#include <bsol/bsol_algs.h>
#include <bdgl/bdgl_curve_algs.h>
#include <vsol/vsol_box_2d.h>
#include <vdgl/vdgl_edgel_chain_sptr.h>
#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_digital_curve.h>
#include <vdgl/vdgl_interpolator_linear.h>
#include <dborl/dborl_category_info.h>
#include <vul/vul_awk.h>

void dborl_exp_stat::print_stats()
{
  vcl_cout << "N+: " << positive_cnt_ << " N-: " << negative_cnt_ << " TP: " << TP_ << " FP: " << FP_ << " TN: " << TN_ << " FN: " << FN_ << vcl_endl;
  if (all_done())
    vcl_cout << "confirmed! N+ equals TP + FN and N- equals FP + TN\n";
  else
    vcl_cout << "problem! exp not finished! N+ does not equal TP + FN or N- does not equal FP + TN\n";
  vcl_cout << "TPR: " << TPR() << " FPR: " << FPR() << " recall: " << recall() << " precision: " << precision() << vcl_endl;
}
void dborl_exp_stat::print_only_stats()
{
  vcl_cout << "N+: " << positive_cnt_ << " N-: " << negative_cnt_ << " TP: " << TP_ << " FP: " << FP_ << " TN: " << TN_ << " FN: " << FN_ << vcl_endl;
}

void dborl_exp_stat::print_stats(vcl_ofstream& of)
{
  of << "N+: " << positive_cnt_ << " N-: " << negative_cnt_ << " TP: " << TP_ << " FP: " << FP_ << " TN: " << TN_ << " FN: " << FN_ << vcl_endl;
  if (all_done())
    of << "confirmed! N+ equals TP + TN and N- equals FP + TN\n";
  else
    of << "problem! exp not finished! N+ does not equal TP + TN or N- does not equal FP + TN\n";
  of << "TPR: " << TPR() << " FPR: " << FPR() << " recall: " << recall() << " precision: " << precision() << vcl_endl;
}
//: on the first line, assumes the format, e.g.: 
//  N+: 21 N-: 0 TP: 6 FP: 15 TN: 0 FN: 0
void dborl_exp_stat::read_stats(vcl_ifstream& ifs)
{
  vul_awk awk(ifs);
  for( ; awk ; ++awk ) {
    if ( awk.NF() != 12 ) continue; 
    vcl_stringstream np1(awk[1]);  np1 >> positive_cnt_;
    vcl_stringstream np2(awk[3]);  np2 >> negative_cnt_;
    vcl_stringstream np3(awk[5]);  np3 >> TP_;
    vcl_stringstream np4(awk[7]);  np4 >> FP_;
    vcl_stringstream np5(awk[9]);  np5 >> TN_;
    vcl_stringstream np6(awk[11]);  np6 >> FN_;
    break;
  }
}

//: for some applications it is not possible to find number of true negatives properly, in that case TN is not printed.
void dborl_exp_stat::print_stats(vcl_string name, vcl_ofstream& of, bool print_FN)
{
  if (print_FN)
    of << "<statistics name=\"" << name << "\" TP=\"" << TP_ << "\" FP=\"" << FP_ << "\" TN=\"" << TN_ << "\" FN=\"" << FN_ << "\"></statistics>\n";
  else
    of << "<statistics name=\"" << name << "\" TP=\"" << TP_ << "\" FP=\"" << FP_ << "\" FN=\"" << FN_ << "\"></statistics>\n";
}

// =============================================================================

//: 
bool dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, vsol_box_2d_sptr gt_box, vsol_box_2d_sptr det_box, float overlap_ratio_threshold)
{
  if (!gt_box && !det_box)
    exp_stat.increment_TN();
  else if (!gt_box && det_box)
    exp_stat.increment_FP();
  else if (gt_box && !det_box)
    exp_stat.increment_FN();
  else if (gt_box && det_box) {
    vsol_box_2d_sptr inters;
    if (bsol_algs::intersection(det_box, gt_box, inters)) {
      vsol_box_2d_sptr uni;
      bsol_algs::box_union(det_box, gt_box, uni);
      float ratio = (float)(inters->area()/uni->area());
      if (ratio >= overlap_ratio_threshold) 
        exp_stat.increment_TP();
      else
        exp_stat.increment_FP();
    } else
      exp_stat.increment_FP();
  }

  return true;
}

//: is there an overlap between the ground truth and the detection box more than the overlap_ratio_threshold
bool dborl_evaluation_overlap(vsol_box_2d_sptr gt_box, vsol_box_2d_sptr det_box, float overlap_ratio_threshold, float& ratio)
{
  vsol_box_2d_sptr inters;
  if (bsol_algs::intersection(det_box, gt_box, inters)) {
    vsol_box_2d_sptr uni;
    bsol_algs::box_union(det_box, gt_box, uni);
    ratio = (float)(inters->area()/uni->area());
    if (ratio >= overlap_ratio_threshold) 
      return true;
    else
      return false; // there is an overlap but less than the threshold
  } else
    return false;  // no intersection
}



// -----------------------------------------------------------------------------
//: check each gt_box of the model_category in this query image
//  return the overlapping ground truth box if there is a detection and a ground truth bounding box that overlaps it sufficiently
vsol_box_2d_sptr dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, 
                        vcl_string model_category, 
                        dborl_image_description_sptr query_desc, 
                        vsol_box_2d_sptr detection_box, 
                        float overlap_ratio_threshold)
{
  vcl_vector<vsol_box_2d_sptr> boxes;

  if (!dborl_get_boxes(boxes, query_desc, model_category)) 
    return 0;
    
  if (!boxes.size()) {
    dborl_evaluation_evaluate_detection(exp_stat, 0, detection_box, overlap_ratio_threshold);
    return 0;
  } else {  
    if (!detection_box) {  // there is a ground truth box but there is no detection --> FN
      exp_stat.increment_FN();
      return 0;
    }
  }
  
  // there is a detection check if there is at least one ground truth box overlaps
  for (unsigned i = 0; i < boxes.size(); i++) {
    float dummy;
    if (dborl_evaluation_overlap(boxes[i], detection_box, overlap_ratio_threshold, dummy)) {
      exp_stat.increment_TP();
      return boxes[i];
    }
  }

  //: no overlap
  exp_stat.increment_FP();   // there is a detection but does not overlap with any of the ground truth boxes
  return 0;
}

//: check the assignment between detection boxes and the prototype boxes
vsol_box_2d_sptr dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, 
                                                     vcl_string model_category, 
                                                     dborl_image_description_sptr query_desc, 
                                                     vcl_vector<vsol_box_2d_sptr> detection_boxes, 
                                                     float overlap_ratio_threshold)
{
  /*  TODO
  vcl_vector<vsol_box_2d_sptr> boxes;
  if (!get_boxes(boxes, query_desc, model_category)) 
    return 0;

  if (!detection_boxes.size()) {  // no detection
    if (boxes.size()>0) {
      exp_stat.increment_FN_by(boxes.size());
    } else {
      exp_stat.increment_TN();  // increment TN just for fun, in a detection experiment, it is not possible to find an accurate TN, so TN should never be used
    }
    return 0;
  }

  for (unsigned i = 0; i < detection_boxes.size(); i++) {  // assign each detection to the best overlapping box
    float best_ratio = 0;
    int best_j = -1;
    for (unsigned j = 0; j < boxes.size(); j++) {
      float ratio;
      if (!dborl_evaluation_overlap(boxes[i], detection_box, overlap_ratio_threshold, ratio))
        continue;
      if (best_ratio < ratio) {
        best_ratio = ratio;
        best_j = j;
      }

    }

  }
  */

  return 0;
}


//: ROC is TPR vs FPR plot
//  x axis is FPR and y axis is TPR so print one line of x values and the second line as y values
void dborl_evaluation_print_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << stats[i]->FPR() << "\t";  // x1 x2 ..
  }
  vcl_cout << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << stats[i]->TPR() << "\t";  // y1 y2 ..
  }
  vcl_cout << vcl_endl;
}
void dborl_evaluation_print_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    of << stats[i]->FPR() << "\t";  // x1 x2 ..
  }
  of << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    of << stats[i]->TPR() << "\t";  // y1 y2 ..
  }
  of << vcl_endl;
}
void dborl_evaluation_get_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys)
{
  xs.clear();  ys.clear();
  for (unsigned i = 0; i < stats.size(); i++) {
    xs.push_back(stats[i]->FPR());
  }
  for (unsigned i = 0; i < stats.size(); i++) {
    ys.push_back(stats[i]->TPR());
  }
}

//: RPC is recall vs 1 - precision plot
//  x axis is (1-precision) and y axis is recall so print one line of x values and the second line as y values
void dborl_evaluation_print_RPC_data(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << (1.0f - stats[i]->precision()) << "\t";  // x1 x2 ..
  }
  vcl_cout << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << stats[i]->recall() << "\t";  // y1 y2 ..
  }
  vcl_cout << vcl_endl;
}
void dborl_evaluation_print_RPC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    of << (1.0f - stats[i]->precision()) << "\t";  // x1 x2 ..
  }
  of << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    of << stats[i]->recall() << "\t";  // y1 y2 ..
  }
  of << vcl_endl;
}

//: PRC is precision vs recall plot
//  x axis is (recall) and y axis is precision so print one line of x values and the second line as y values
void dborl_evaluation_print_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << stats[i]->recall() << "\t";  // x1 x2 ..
  }
  vcl_cout << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    vcl_cout << stats[i]->precision() << "\t";  // y1 y2 ..
  }
  vcl_cout << vcl_endl;
}
void dborl_evaluation_print_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of)
{
  for (unsigned i = 0; i < stats.size(); i++) {
    of << stats[i]->recall() << "\t";  // x1 x2 ..
  }
  of << vcl_endl;
  for (unsigned i = 0; i < stats.size(); i++) {
    of << stats[i]->precision() << "\t";  // y1 y2 ..
  }
  of << vcl_endl;
}
void dborl_evaluation_get_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys)
{
  xs.clear();  ys.clear();
  for (unsigned i = 0; i < stats.size(); i++) {
    xs.push_back(stats[i]->recall());
  }
  for (unsigned i = 0; i < stats.size(); i++) {
    ys.push_back(stats[i]->precision());
  }
}

//: intersect the (0,1) - (1,0) line on the ROC plot with the ROC curve given by the stats
float dborl_evaluation_ROC_EER(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  vdgl_edgel_chain_sptr chain = new vdgl_edgel_chain();
  for (unsigned i = 0; i<stats.size(); i++) {
    float x = stats[i]->FPR();
    float y = stats[i]->TPR();
    vdgl_edgel e(x, y);
    chain->add_edgel(e);
  }

  vdgl_interpolator_sptr interp = new vdgl_interpolator_linear(chain);
  vdgl_digital_curve_sptr dc = new vdgl_digital_curve(interp);

  vgl_point_2d<double> p1(0, 1);
  vgl_point_2d<double> p2(1, 0);
  vcl_vector<vgl_point_2d<double> > pts;
  vgl_line_2d<double> dummy_line(p1, p2);
  if (!bdgl_curve_algs::intersect_line_fast(dc, dummy_line, pts))
    return -1;

  if (!pts.size())
    return -1;
  else if (pts.size() > 1) 
    return -1;

  float eer_fpr = (float)pts[0].x();
  float eer_tpr = 1.0f - eer_fpr;
  return eer_tpr;
}

//: intersect the (0,1) - (1,0) line on the RPC plot with the RPC curve given by the stats
float dborl_evaluation_RPC_EER(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  vdgl_edgel_chain_sptr chain = new vdgl_edgel_chain();
  for (unsigned i = 0; i<stats.size(); i++) {
    float x = (1.0f - stats[i]->precision());
    float y = stats[i]->recall();
    vdgl_edgel e(x, y);
    chain->add_edgel(e);
  }

  vdgl_interpolator_sptr interp = new vdgl_interpolator_linear(chain);
  vdgl_digital_curve_sptr dc = new vdgl_digital_curve(interp);

  vgl_point_2d<double> p1(0, 1);
  vgl_point_2d<double> p2(1, 0);
  vcl_vector<vgl_point_2d<double> > pts;
  vgl_line_2d<double> dummy_line(p1, p2);
  if (!bdgl_curve_algs::intersect_line_fast(dc, dummy_line, pts))
    return -1;

  if (!pts.size())
    return -1;
  else if (pts.size() > 1) 
    return -1;

  float eer_fpr = (float)pts[0].x();
  float eer_tpr = 1.0f - eer_fpr;
  return eer_tpr;
}

//: intersect the (0,1) - (1,0) line on the PRC plot with the PRC curve given by the stats
float dborl_evaluation_PRC_EER(vcl_vector<dborl_exp_stat_sptr>& stats)
{
  vdgl_edgel_chain_sptr chain = new vdgl_edgel_chain();
  for (unsigned i = 0; i<stats.size(); i++) {
    float x = stats[i]->recall();
    float y = stats[i]->precision();
    vdgl_edgel e(x, y);
    chain->add_edgel(e);
  }

  vdgl_interpolator_sptr interp = new vdgl_interpolator_linear(chain);
  vdgl_digital_curve_sptr dc = new vdgl_digital_curve(interp);

  vgl_point_2d<double> p1(0, 1);
  vgl_point_2d<double> p2(1, 0);
  vcl_vector<vgl_point_2d<double> > pts;
  vgl_line_2d<double> dummy_line(p1, p2);
  if (!bdgl_curve_algs::intersect_line_fast(dc, dummy_line, pts))
    return -1;

  if (!pts.size())
    return -1;
  else if (pts.size() > 1) 
    return -1;

  float eer_fpr = (float)pts[0].x();
  float eer_tpr = 1.0f - eer_fpr;
  return eer_tpr;
}


void dborl_evaluation_evaluate_classification(dborl_exp_stat_sptr exp_stat, 
                                              const vcl_string& gt_class, 
                                              const vcl_string& output_class)
{
  if (output_class == gt_class)
    exp_stat->increment_TP();
  else 
    exp_stat->increment_FP();
}

void dborl_evaluation_evaluate_classification(dborl_exp_stat_sptr exp_stat, 
                                              dborl_category_info_set_sptr cis,
                                              const vcl_string& gt_class, 
                                              int output_class_id)
{
  //first find the class of the gt_class
  dborl_category_info_sptr cat = cis->find_category(gt_class);
  if (!cat) {
    vcl_cerr << "In dborl_evaluation_evaluate_classification() -- cannot find the class: " << gt_class << " in the available classes list!\n";
    throw 0;
  }
  if (cat->id_ == output_class_id)
    exp_stat->increment_TP();
  else
    exp_stat->increment_FP();
}



//: Binary io, NOT IMPLEMENTED, signatures defined to use dborl_exp_stat as a brdb_value
void vsl_b_write(vsl_b_ostream & os, dborl_exp_stat const &ph)
{
  vcl_cerr << "vsl_b_write() -- Binary io, NOT IMPLEMENTED, signatures defined to use dborl_exp_stat as a brdb_value\n";
  return;
}

void vsl_b_read(vsl_b_istream & is, dborl_exp_stat &ph)
{
  vcl_cerr << "vsl_b_read() -- Binary io, NOT IMPLEMENTED, signatures defined to use dborl_exp_stat as a brdb_value\n";
  return;
}

void vsl_b_read(vsl_b_istream& is, dborl_exp_stat* ph)
{
  delete ph;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr)
  {
    ph = new dborl_exp_stat();
    vsl_b_read(is, *ph);
  }
  else
    ph = 0;
}

void vsl_b_write(vsl_b_ostream& os, const dborl_exp_stat* &ph)
{
  if (ph==0)
  {
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else
  {
    vsl_b_write(os,true); // Indicate non-null pointer stored
    vsl_b_write(os,*ph);
  }
}
