#include "dbsol_corner_finder.h"
#include <vnl/vnl_math.h>
#include <vcl_algorithm.h>
#include <vcl_iostream.h>

void dbsol_corner_finder::find_corners(dbsol_interp_curve_2d *c, bool is_open, vcl_string out_angle_diffs)
{
  VICINITY_ = 35;
  DIST_STEP_ = 0.5;
  MIN_TAN_TURN_ = 0.5236;
  curve_ = c;
  is_open_ = is_open;
  out_angle_diffs_ = out_angle_diffs;
  get_tangent_angles();
  compute_average_tangent_angles();
  find_extrema();
}

void dbsol_corner_finder::find_corners(dbsol_interp_curve_2d *c, bool is_open,
                                       int vicinity, double dist_step, double min_tan_turn,
                                       vcl_string out_angle_diffs)
{
  VICINITY_ = vicinity;
  DIST_STEP_ = dist_step;
  MIN_TAN_TURN_ = min_tan_turn;
  curve_ = c;
  is_open_ = is_open;
  out_angle_diffs_ = out_angle_diffs;
  get_tangent_angles();
  compute_average_tangent_angles();
  find_extrema();
}

void dbsol_corner_finder::get_tangent_angles()
{
  for(unsigned i=0; i < curve_->size()+1; i++)
  {
    _tangent.push_back(curve_->tangent_angle_at(curve_->length_at(i)));
//    vcl_cout << c->tangent_angle_at(c->length_at(i)) << vcl_endl;
  }
}

void dbsol_corner_finder::compute_average_tangent_angles()
{
  vcl_pair<double, double> temp;
  int tangent_size_int = static_cast<int>(_tangent.size());
  for (int i=0;i<tangent_size_int;i++)
  {
    temp.first  = aveTangent(i, -VICINITY_); // go negative direction
    temp.second = aveTangent((i+1)%_tangent.size(), VICINITY_); // go positive direction
    
    _aveTangent.push_back(temp);
  }
}

double dbsol_corner_finder::aveTangent(int start, int count)
{   
  int size = curve_->size() + 1;

  if ((start < 0)||(start >= size))
  {
    vcl_cout<<" Error:<aveTangent> Parameters out of range "<<vcl_endl;
    return 0;
  }
  
  int count_mag = abs(count);
  double prev_tangent = 0, tangent_sum = 0;
  
  vcl_vector<double> tangent_list;
  int index = 0;
  double tangent = 0;
  double av_tangent = 0;
  
  double arclength = 0;
  int tan_count = 0;
  int net_tan_count = 0;
  
  // Decide on the points to use for the computation
  // taking care of the arclength
  // This is Poor Man's computation
  while(1) 
  {
    if (count<0)
      index = (start-tan_count-1);
    else
      index = (start+tan_count);
    
    index %= size;
    if (index < 0)
      index += size;
    
    tangent = _tangent[index];
    tangent = fmod(tangent, 2*vnl_math::pi);
    
    if (tangent < 0)
      tangent += 2*vnl_math::pi;

    tangent_list.push_back(tangent);
    
    // Need to handle arcLength
    
//    if (_arcLength[index] < (DIST_STEP+arclength))
    if ( curve_->length_at(index) < (DIST_STEP_+arclength))
    {
      tan_count++;
      arclength = 0;
    }
    else
      arclength += curve_->length_at(index);
//      arclength += _arcLength[index];
    
    net_tan_count++;

    if (net_tan_count >= count_mag)
      break;
  }

  // Sort the tangent list
  vcl_sort(tangent_list.begin(), tangent_list.end());

  // Compute the average of tangent change
  for (unsigned i = 0; i < tangent_list.size(); i++) 
  {
    tangent = tangent_list[i];
    if (i == 0)
      prev_tangent = tangent;
    
    if (fabs(tangent-prev_tangent) > vnl_math::pi) 
    {
      if (tangent > 0)
        tangent = tangent - 2*vnl_math::pi;
      else
        tangent = tangent + 2*vnl_math::pi;
    }
          
    if (fabs(tangent-prev_tangent) > vnl_math::pi) 
    {
      vcl_cout << " Warning : Angle diff > M_PI. Prev " << prev_tangent*(180/vnl_math::pi) 
        << " Tangent = " << tangent*(180/vnl_math::pi) << vcl_endl;
    }
    
    tangent_sum  += tangent;
    prev_tangent  = tangent;
  }
    
  av_tangent = (tangent_sum/(tangent_list.size()*(1.0)));
  av_tangent = fmod(av_tangent, 2*vnl_math::pi);
    
  if (av_tangent < 0)
    av_tangent += 2*vnl_math::pi;
  
  return (av_tangent);
}

void dbsol_corner_finder::find_extrema()
{
  vcl_ofstream outfp;
  if(out_angle_diffs_.empty() == 0)
    outfp.open(out_angle_diffs_.c_str());

  double diff=0;
  vcl_vector<double> angle_diff;
  unsigned i;
  for (i=0;i<_aveTangent.size();i++)
  {
    diff = (_aveTangent[i].first)-(_aveTangent[i].second);
    if (diff<0)
      diff+=2*vnl_math::pi;
    angle_diff.push_back(diff);
  }

  int prev, next;
  int size = _aveTangent.size();

  unsigned last_point_index_to_check;
  if(is_open_ == true)
    last_point_index_to_check = _aveTangent.size()-1;
  else
    last_point_index_to_check = _aveTangent.size();

  for (i=0;i<last_point_index_to_check;i++) 
  {
    prev = i-1;  
    if (prev<0)
      prev +=size;
    next = (i+1)%size;

    if(out_angle_diffs_.empty() == 0)
      outfp << i << " "  << curve_->point_at_sample(i)->x() << " " << curve_->point_at_sample(i)->y() << " " << angle_diff[i] << vcl_endl;

    if ((angle_diff[i]>angle_diff[prev]) && (angle_diff[i]>=angle_diff[next])) 
    {
      if ((angle_diff[i]<vnl_math::pi) && (angle_diff[i]>MIN_TAN_TURN_))
        _extrema.push_back(i);
    }
    if ((angle_diff[i]<angle_diff[prev]) && (angle_diff[i]<=angle_diff[next])) 
    {
      if ((angle_diff[i]>(vnl_math::pi)) && ((angle_diff[i]<(2*vnl_math::pi-MIN_TAN_TURN_))))
        _extrema.push_back(i); 
    } 
  }
  if(out_angle_diffs_.empty() == 0)
    outfp.close();
}

