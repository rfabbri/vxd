#include "dbnl_eno_zerox_label.h"
#include <stdio.h>

#if !VCL_STATIC_CONST_INIT_INT_NO_DEFN
const unsigned dbnl_eno_zerox_label::unlabeled
      VCL_STATIC_CONST_INIT_INT_DEFN((unsigned)-1);

const unsigned dbnl_eno_zerox_label::maxzeros
      VCL_STATIC_CONST_INIT_INT_DEFN(4);
#endif

dbnl_eno_zerox_label::
  dbnl_eno_zerox_label(
      const double *data,
      const unsigned *label,
      const dbnl_eno_zerox_vector &zc)
  :
  label_(4*zc.size(),unlabeled)
{

  for (unsigned i=0; i<zc.size(); ++i) {

    if (zc[i].number()) {
      if (zc[i].number() == 1)
        label_one_zerox_in_interval(i,zc[i],data,label);
      else if (zc[i].number() == 2)
        label_two_zerox_in_interval(i,zc[i],data,label);
      else
        vcl_cerr << "This should not be happening: No. of Zerox=" 
          << zc[i].number() << vcl_endl;
    }
  }
}

void dbnl_eno_zerox_label::
label_one_zerox_in_interval(
    unsigned pos,
    const dbnl_eno_zero_crossing &, 
    const double *data, 
    const unsigned *data_label)
{
  double start,end;

  start=data[pos];
  end=data[pos+1];

  if (start > 0 && end > 0){
    printf("Both end points can't be +ve with one zerox\n");
    printf("Loc=%d; start=%g; end=%g",pos, start,end);
    label_[maxzeros*pos]=dbnl_eno_zerox_label::unlabeled;
  } 
  else if (start > 0)
    label_[maxzeros*pos]=data_label[pos];
  else if (end > 0)
    label_[maxzeros*pos]=data_label[pos+1];
  else {
    printf("Both end points can't be -ve with one zerox\n");
    printf("Loc=%d; start=%g; end=%g",pos, start,end);
    label_[maxzeros*pos] =dbnl_eno_zerox_label::unlabeled;
  }
}

void
dbnl_eno_zerox_label::
label_two_zerox_in_interval(
    unsigned pos,
    const dbnl_eno_zero_crossing &zc_i, 
    const double *data, 
    const unsigned *data_label)
{
  double start,end; 

  start = data[pos];
  end   = data[pos+1];

  /*If both end points are +ve  label the zerox based on the closest
   end point*/
  if (start >= 0 && end >= 0){
    if (zc_i.location(0) < zc_i.location(1)){
      label_[maxzeros*pos]   = data_label[pos];
      label_[maxzeros*pos+1] = data_label[pos+1];
    }
    else{
      vcl_cerr << "in label_two_zerox_in_interval:: hmm... zero crossings not sorted.." << vcl_endl;
      label_[maxzeros*pos+1] = data_label[pos];
      label_[maxzeros*pos]   = data_label[pos+1];
    }
  }
  else if (start < 0 || end < 0){
    if (start > 0 || end > 0)
      vcl_cerr << 
        "label_two_zerox_in_interval: warning: we have two zero xings but function changes sign" 
        << vcl_endl << "Start: " << start << " End: "
        << end << " Pos: " << pos << vcl_endl;
    /*If end points are -ve cannot label the zerox*/
    label_[maxzeros*pos]=dbnl_eno_zerox_label::unlabeled;
    label_[maxzeros*pos+1]=dbnl_eno_zerox_label::unlabeled;
  }
}

//: print alongside dbnl_eno_1d and zerox
void dbnl_eno_zerox_label::
print(dbnl_eno_1d &eno, dbnl_eno_zerox_vector &zc, vcl_ostream& strm) const
{
   strm << "==== Eno 1D with Labeled Zerocrossings ====" << vcl_endl
        << "len: " << eno.size() << vcl_endl;

   unsigned i;
   for (i=0; i < eno.size(); ++i) {
      eno[i].print(strm);
      zc[i].print(strm);
      vcl_cout << "Labels: ";

      for (unsigned k=0; k < zc[i].number(); ++k) {
        vcl_cout << LABEL_(i,k) << " ";
      }
      vcl_cout << vcl_endl << vcl_endl << vcl_endl;
   }
}
