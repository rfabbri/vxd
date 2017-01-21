#include "borld_exp_stat.h"


void borld_exp_stat::print_stats()
{
  vcl_cout << "N+: " << positive_cnt_ << " N-: " << negative_cnt_ << " TP: " << TP_ << " FP: " << FP_ << " TN: " << TN_ << " FN: " << FN_ << vcl_endl;
  if (all_done())
    vcl_cout << "confirmed! N+ equals TP + FN and N- equals FP + TN\n";
  else
    vcl_cout << "problem! exp not finished! N+ does not equal TP + FN or N- does not equal FP + TN\n";
  vcl_cout << "TPR: " << TPR() << " FPR: " << FPR() << " recall: " << recall() << " precision: " << precision() << vcl_endl;
}
void borld_exp_stat::print_only_stats()
{
  vcl_cout << "N+: " << positive_cnt_ << " N-: " << negative_cnt_ << " TP: " << TP_ << " FP: " << FP_ << " TN: " << TN_ << " FN: " << FN_ << vcl_endl;
}

void borld_exp_stat::print_stats(vcl_ofstream& of)
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
void borld_exp_stat::read_stats(vcl_ifstream& ifs)
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
void borld_exp_stat::print_stats(vcl_string name, vcl_ofstream& of, bool print_FN)
{
  if (print_FN)
    of << "<statistics name=\"" << name << "\" TP=\"" << TP_ << "\" FP=\"" << FP_ << "\" TN=\"" << TN_ << "\" FN=\"" << FN_ << "\"></statistics>\n";
  else
    of << "<statistics name=\"" << name << "\" TP=\"" << TP_ << "\" FP=\"" << FP_ << "\" FN=\"" << FN_ << "\"></statistics>\n";
}
