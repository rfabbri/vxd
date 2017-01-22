// This is borld_exp_stat.h
#ifndef borld_exp_stat_h
#define borld_exp_stat_h
//:
//\file
//\brief  Basic class to hold experiment statistics
//\author Based on original code by Ozge C Ozcanli
//\date 11/15/07
//
// \verbatim
//  Modification
//    Ricardo Fabbri Moved to VXD into a separate class
// \endverbatim
//

#include <vbl/vbl_ref_count.h>
#include <vcl_iostream.h>
#include <vsl/vsl_binary_io.h>

class borld_exp_stat : public vbl_ref_count
{
public:
  borld_exp_stat() : positive_cnt_(0), negative_cnt_(0), TP_(0), FP_(0), TN_(0), FN_(0) {}

  borld_exp_stat(int positive_cnt, int negative_cnt) : positive_cnt_(positive_cnt), negative_cnt_(negative_cnt),
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

  //: for some applications it is not possible to find number of true negatives
  // properly, in that case TN is not printed.
  void print_stats(vcl_string name, vcl_ofstream& of, bool print_FN);
};

//
// Binary io, NOT IMPLEMENTED, signatures defined to use borld_exp_stat as a brdb_value
void vsl_b_write(vsl_b_ostream & os, borld_exp_stat const &ph);
void vsl_b_read(vsl_b_istream & is, borld_exp_stat &ph);
void vsl_b_read(vsl_b_istream& is, borld_exp_stat* ph);
void vsl_b_write(vsl_b_ostream& os, const borld_exp_stat* &ph);

#endif // borld_exp_stat_h

