//:
// \file
// \brief Evaluation tools like a class that holds experiment statistics
//
// \author Ozge C Ozcanli (ozge@lems.brown.edu)
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
#include <dborl/dborl_exp_stat_sptr.h>
#include <dborl/dborl_image_description_sptr.h>
#include <dborl/dborl_category_info_sptr.h>
#include <vsl/vsl_binary_io.h>

class dborl_evaluation_plot_type {
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

class dborl_exp_stat : public vbl_ref_count
{
public:
  dborl_exp_stat() : positive_cnt_(0), negative_cnt_(0), TP_(0), FP_(0), TN_(0), FN_(0) {}

  dborl_exp_stat(int positive_cnt, int negative_cnt) : positive_cnt_(positive_cnt), negative_cnt_(negative_cnt),
                                                       TP_(0), FP_(0), TN_(0), FN_(0) {}

  int positive_cnt_; // number of positive examplers in the test set
  int negative_cnt_; // number of negative examplers in the test set

  int TP_;   // true positives/hits
  int FP_;   // false positives/false alarms  (type I error)
  int TN_;   // true negatives/correct rejections
  int FN_;   // false negatives/misses        (type II error)

  void increment_TP() { TP_++; }
  void increment_FP() { FP_++; }
  void increment_TN() { TN_++; }
  void increment_FN() { FN_++; }

  void increment_TP_by(int amount) { TP_ = TP_ + amount; }
  void increment_FP_by(int amount) { FP_ = FP_ + amount; }
  void increment_TN_by(int amount) { TN_ = TN_ + amount; }
  void increment_FN_by(int amount) { FN_ = FN_ + amount; }

  void increment_positive_cnt() { positive_cnt_++; }  // if positive cnt and negative cnt are not known apriori
  void increment_negative_cnt() { negative_cnt_++; }

  inline bool all_done() { return (TP_ + FN_) == positive_cnt_ && (FP_ + TN_) == negative_cnt_; }

  inline float TPR()          { return (float)TP_/positive_cnt_; }
  inline float recall()       { return TPR(); }
  inline float FPR()          { return (float)FP_/negative_cnt_; }
  inline float specifity()    { return 1 - FPR(); }
  inline float precision()    { return (float)TP_/(TP_ + FP_); }
  inline float sensitivity()  { return TPR(); }
  inline float accuracy()     { return (float)(TP_ + TN_)/(positive_cnt_ + negative_cnt_); }
  inline float f_measure()    { return (2*recall()*precision())/(recall() + precision()); }
  inline void update_positive_count() { positive_cnt_ = TP_ + FN_; }
  inline void update_negative_count() { negative_cnt_ = FP_ + TN_; }

  void print_stats();
  void print_only_stats();
  void print_stats(vcl_ofstream& of);
  void read_stats(vcl_ifstream& ifs);

  //: for some applications it is not possible to find number of true negatives properly, in that case TN is not printed.
  void print_stats(vcl_string name, vcl_ofstream& of, bool print_FN);
  
};

// Binary io, NOT IMPLEMENTED, signatures defined to use dborl_exp_stat as a brdb_value
void vsl_b_write(vsl_b_ostream & os, dborl_exp_stat const &ph);
void vsl_b_read(vsl_b_istream & is, dborl_exp_stat &ph);
void vsl_b_read(vsl_b_istream& is, dborl_exp_stat* ph);
void vsl_b_write(vsl_b_ostream& os, const dborl_exp_stat* &ph);

bool dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, 
                                         vsol_box_2d_sptr ground_truth_box, 
                                         vsol_box_2d_sptr detection_box, 
                                         float overlap_ratio_threshold);

//: check each gt_box of the model_category in this query image
vsol_box_2d_sptr dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, 
                                                     vcl_string model_category, 
                                                     dborl_image_description_sptr query_desc, 
                                                     vsol_box_2d_sptr detection_box, 
                                                     float overlap_ratio_threshold);

//: check the assignment between detection boxes and the prototype boxes
vsol_box_2d_sptr dborl_evaluation_evaluate_detection(dborl_exp_stat& exp_stat, 
                                                     vcl_string model_category, 
                                                     dborl_image_description_sptr query_desc, 
                                                     vcl_vector<vsol_box_2d_sptr> detection_boxes, 
                                                     float overlap_ratio_threshold);

void dborl_evaluation_evaluate_classification(dborl_exp_stat_sptr exp_stat, 
                                              const vcl_string& gt_class, 
                                              const vcl_string& output_class);

void dborl_evaluation_evaluate_classification(dborl_exp_stat_sptr exp_stat, 
                                              dborl_category_info_set_sptr cis,
                                              const vcl_string& gt_class, 
                                              int output_class_id);



//: ROC is TPR vs FPR plot
//  x axis is FPR and y axis is TPR so print one line of x values and the second line as y values
void dborl_evaluation_print_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats);
void dborl_evaluation_get_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys);

//: RPC is recall vs 1 - precision plot
//  x axis is (1-precision) and y axis is recall so print one line of x values and the second line as y values
void dborl_evaluation_print_RPC_data(vcl_vector<dborl_exp_stat_sptr>& stats);

//: PRC is precision vs recall plot
//  x axis is (recall) and y axis is precision so print one line of x values and the second line as y values
void dborl_evaluation_print_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats);

void dborl_evaluation_print_ROC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of);
void dborl_evaluation_print_RPC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of);
void dborl_evaluation_print_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_ofstream& of);
void dborl_evaluation_get_PRC_data(vcl_vector<dborl_exp_stat_sptr>& stats, vcl_vector<float>& xs, vcl_vector<float>& ys);

//: intersect the (0,1) - (1,0) line on the ROC plot with the ROC curve given by the stats
float dborl_evaluation_ROC_EER(vcl_vector<dborl_exp_stat_sptr>& stats);

//: intersect the (0,1) - (1,0) line on the RPC plot with the RPC curve given by the stats
float dborl_evaluation_RPC_EER(vcl_vector<dborl_exp_stat_sptr>& stats);

//: intersect the (0,1) - (1,0) line on the PRC plot with the PRC curve given by the stats
float dborl_evaluation_PRC_EER(vcl_vector<dborl_exp_stat_sptr>& stats);


#endif  //_DBORL_EVALUATION_H
