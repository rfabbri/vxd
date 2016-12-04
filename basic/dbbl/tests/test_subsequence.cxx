#include <testlib/testlib_test.h>
#include <dbbl/dbbl_subsequence.h>
#include <vcl_functional.h>

class is_less_than_zero : public vcl_unary_function<unsigned, bool> {
public:
  //: delta_theta is effective if it is between 0 and vnl_math::pi/2. 
  is_less_than_zero(
      const vcl_vector<int> &v)
    : v_(v)
  { }

  bool operator() (unsigned i) const {
    return v_[i] < 0;
  }

private:
  const vcl_vector<int> &v_;
};

MAIN( test_subsequence )
{
  START ("subsequence");

  //: We will partition this vcl_vector into all contiguous subsequences whose
  // elements are either all <0, or all >= 0.
  vcl_vector<int> n;
  n.push_back(0);
  n.push_back(-1);
  n.push_back(4);
  n.push_back(4);
  n.push_back(-3);
  n.push_back(-3);
  n.push_back(-3);

  unsigned num_subsequences_gt = 4;

  //: ground truth indicates which contiguous subsequences are negative
  vcl_vector<bool> is_negative_truth(4);
  is_negative_truth[0] = false;
  is_negative_truth[1] = true;
  is_negative_truth[2] = false;
  is_negative_truth[3] = true;

  dbbl_subsequence_set ss;
  dbbl_subsequence orig_seq(0, n.size()); 
  orig_seq.set_orig_id(169);  //< any id to represent n
  dbbl_contiguous_partition(orig_seq, is_less_than_zero(n), &ss);

  vcl_cout << "num subsequences " << ss.num_subsequences() << vcl_endl;
  TEST("num subsequences", ss.num_subsequences(), num_subsequences_gt);
  TEST("total size of subsequences", ss.total_size(), n.size());
  for (unsigned i=0; i < ss.num_subsequences(); ++i) {
    TEST("subsequence original id", ss[i].orig_id(), 169);
    for (unsigned k = ss[i].ini(); k < ss[i].end(); ++k) {
      TEST("subsequence value ok", n[k] < 0, is_negative_truth[i]);
    }
  }
  
  SUMMARY();
}
