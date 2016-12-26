#ifndef dbnl_matrix_multiply_h
#define dbnl_matrix_multiply_h

#include <vnl/vnl_matrix.h>

vnl_matrix<double>  multiply(vnl_matrix<double> &A,vnl_matrix<double> &B, char At='N',char Bt='N' );


#endif
