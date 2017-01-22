//:
// \file
// \brief Evaluation tools like a class that holds experiment statistics
//
// \author Based on original code by  Ozge C Ozcanli (@lems.brown.edu)
// \date 11/15/07
//      
// \verbatim
//   Modifications
//  
// \endverbatim

//
//
#if !defined(_DBORL_EVALUATION_H)
#define _DBORL_EVALUATION_H

#include <vbl/vbl_ref_count.h>
#include <vcl_vector.h>
#include <vcl_map.h>
#include <vcl_utility.h>
#include <vcl_string.h>
#include <vsol/vsol_box_2d_sptr.h>
#include <buld/buld_exp_stat.h>
#include <buld/buld_exp_stat_sptr.h>
#include <borld/borld_image_description_sptr.h>
#include <borld/borld_category_info_sptr.h>

class borld_evaluation_plot_type {
public:
  enum type {
    ROC,   // TPR vs FPR or TPR vs FPPI or TPR vs FPPW
    RPC,   // recall vs precision
    PRC,   // precision vs recall
    PERF   // any performance plot, eg TPR vs some parameter or FPR vs some parameter etc.
  };
 
  static vcl_string get_plot_type_str(short type) {
    vcl_string tag;
    switch(type) {
      case ROC: tag = "plot_roc"; break;
      case RPC: tag = "plot_rpc"; break;
      case PRC: tag = "plot_prc"; break;
      case PERF: tag = "plot_perf"; break;
      default: tag = "UNKNOWN"; break;
    }
    return tag;
  }
};

bool borld_evaluation_evaluate_detection(buld_exp_stat& exp_stat, 
                                         vsol_box_2d_sptr ground_truth_box, 
                                         vsol_box_2d_sptr detection_box, 
                                         float overlap_ratio_threshold);

//: check each gt_box of the model_category in this query image
vsol_box_2d_sptr borld_evaluation_evaluate_detection(buld_exp_stat& exp_stat, 
                                                     vcl_string model_category, 
                                                     borld_image_description_sptr query_desc, 
                                                     vsol_box_2d_sptr detection_box, 
                                                     float overlap_ratio_threshold);

//: check the assignment between detection boxes and the prototype boxes
vsol_box_2d_sptr borld_evaluation_evaluate_detection(buld_exp_stat& exp_stat, 
                                                     vcl_string model_category, 
                                                     borld_image_description_sptr query_desc, 
                                                     vcl_vector<vsol_box_2d_sptr> detection_boxes, 
                                                     float overlap_ratio_threshold);

void borld_evaluation_evaluate_classification(buld_exp_stat_sptr exp_stat, 
                                              const vcl_string& gt_class, 
                                              const vcl_string& output_class);

void borld_evaluation_evaluate_classification(buld_exp_stat_sptr exp_stat, 
                                              borld_category_info_set_sptr cis,
                                              const vcl_string& gt_class, 
                                              int output_class_id);



//: ROC is TPR vs FPR plot
//  x axis is FPR and y axis is TPR so print one line of x values and the second line as y values
void borld_evaluation_print_ROC_data(vcl_vector<buld_exp_stat_sptr>& stats);
void borld_evaluation_get_ROC_data(vcl_vector<buld_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys);

//: RPC is recall vs 1 - precision plot
//  x axis is (1-precision) and y axis is recall so print one line of x values and the second line as y values
void borld_evaluation_print_RPC_data(vcl_vector<buld_exp_stat_sptr>& stats);

//: PRC is precision vs recall plot
//  x axis is (recall) and y axis is precision so print one line of x values and the second line as y values
void borld_evaluation_print_PRC_data(vcl_vector<buld_exp_stat_sptr>& stats);

void borld_evaluation_print_ROC_data(vcl_vector<buld_exp_stat_sptr>& stats, vcl_ofstream& of);
void borld_evaluation_print_RPC_data(vcl_vector<buld_exp_stat_sptr>& stats, vcl_ofstream& of);
void borld_evaluation_print_PRC_data(vcl_vector<buld_exp_stat_sptr>& stats, vcl_ofstream& of);
void borld_evaluation_get_PRC_data(vcl_vector<buld_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys);

//: intersect the (0,1) - (1,0) line on the ROC plot with the ROC curve given by the stats
float borld_evaluation_ROC_EER(vcl_vector<buld_exp_stat_sptr>& stats);

//: intersect the (0,1) - (1,0) line on the RPC plot with the RPC curve given by the stats
float borld_evaluation_RPC_EER(vcl_vector<buld_exp_stat_sptr>& stats);

//: intersect the (0,1) - (1,0) line on the PRC plot with the PRC curve given by the stats
float borld_evaluation_PRC_EER(vcl_vector<buld_exp_stat_sptr>& stats);


#endif  //_DBORL_EVALUATION_H
