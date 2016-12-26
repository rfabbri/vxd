#include "dbnl_matrix_multiply.h"

#include <v3p_netlib.h>
#include <vnl/vnl_fortran_copy.h>
#include <vcl_iostream.h>

vnl_matrix<double>  multiply(vnl_matrix<double> &Am,vnl_matrix<double> &Bm, char At,char Bt )
    {
      v3p_netlib_integer M;
      if(At=='N')
        M=Am.rows();
      else
        M=Am.cols();

      v3p_netlib_integer N;
      if(Bt=='N')
        N=Bm.cols();
      else
        N=Bm.rows();

      v3p_netlib_integer K;
      if(At=='N')
        K=Am.cols();
      else
        K=Am.rows();
      double alpha=1;
      double beta=0;

      v3p_netlib_integer LDA=Am.rows();
      v3p_netlib_integer LDB=Bm.rows();
      v3p_netlib_integer LDC=M;

      vnl_fortran_copy<double> a(Am);
      vnl_fortran_copy<double> b(Bm);

      double * A=a;
      double * B=b;

      double * C=new double[M*N];

      // FIXME I'm not sure what the last 2 parameters should be
      v3p_netlib_dgemm_(&At,&Bt,&M,&N,&K,&alpha,A,&LDA,B,&LDB,&beta,C,&LDC,0,0);

      vnl_matrix<double> Cm(M,N);
      for(int j=0;j<N;j++)
          for(int i=0;i<M;i++)
                Cm(i,j)=C[i+j*M];

      delete[] C;
      return Cm;
    }
