// This is basic/dbnl/algo/dbnl_bbf_tree.cxx
//:
// \file

#include "dbnl_bbf_tree.h"

#include <vcl_limits.h>
#include <vcl_utility.h>
#include <vcl_algorithm.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector.h>



//------------------------------------------------------------------------------
// BBF Bounding Box
//------------------------------------------------------------------------------

//: Constructor
template <class T, unsigned int n>
dbnl_bbf_box<T,n>::dbnl_bbf_box( const vnl_vector_fixed<T,n>& min_point,
                                      const vnl_vector_fixed<T,n>& max_point )
  : min_point_(min_point), max_point_(max_point)
{
  // swap incorrectly ordered values
  for ( unsigned int i=0; i<n; ++i )
    if ( min_point_[i] > max_point_[i] ) {
      T temp = min_point_[i];
      min_point_[i] = max_point_[i];
      max_point_[i] = temp;
    }
}

//: Copy Constructor
template <class T, unsigned int n>
dbnl_bbf_box<T,n>::dbnl_bbf_box( const dbnl_bbf_box<T,n>& old )
  : min_point_(old.min_point_), max_point_(old.max_point_)
{
}

//: Assignment Operator
template <class T, unsigned int n>
dbnl_bbf_box<T,n> &
dbnl_bbf_box<T,n>::operator= ( const dbnl_bbf_box<T,n>& old )
{
  min_point_ = old.min_point_;
  max_point_ = old.max_point_;
  return *this;
}

//: Return the minimum square distance between \a p and any point in \a b.
template <class T, unsigned int n>
T dbnl_bbf_dist_sq( const vnl_vector_fixed<T,n>& p, const dbnl_bbf_box<T,n>& b )
{
  T sum_sq = 0;

  for ( unsigned int i=0; i<n; ++i ) {
    const T& x0 = b.min_point_[i];
    const T& x1 = b.max_point_[i];
    const T& x  = p[i];
    if ( x < x0 ) {
      sum_sq += vnl_math::sqr( x0 - x );
    }
    else if ( x > x1 ) {
      sum_sq += vnl_math::sqr( x1 - x );
    }
  }

  return sum_sq;
}


//------------------------------------------------------------------------------
// BBF Tree
//------------------------------------------------------------------------------


//: Constuctor
template <class T, unsigned int n>
dbnl_bbf_tree<T,n>::dbnl_bbf_tree(const vcl_vector< vnl_vector_fixed<T,n> >& points, int points_per_leaf)
 : leaf_count_(0), leaves_examined_(0), internal_count_(0),
   internal_examined_(0), points_(points) 
{
  assert(points_per_leaf > 0);
  
  // 1.  Build the initial bounding box.
  vnl_vector_fixed<T,n> low(vcl_numeric_limits<T>::min());
  vnl_vector_fixed<T,n> high(vcl_numeric_limits<T>::max());
  dbnl_bbf_box<T,n> box( low, high );
  
  // 2. create the vector of ids
  vcl_vector< int > indices( points_.size() );
  for ( unsigned int i=0; i<indices.size(); ++i ) indices[ i ] = i;

  // 3. call recursive function to do the real work
  root_ = build_tree( points_per_leaf, box, 0, indices );

  vcl_cout << "total leaves = " << leaf_count_ << vcl_endl
           << "total interal = " << internal_count_ << vcl_endl;
}


//: For sorting pairs by their first element
template <class T>
inline bool
first_less( const vcl_pair<T,int>& left,
            const vcl_pair<T,int>& right )
{
  return left.first < right.first;
}


//: Build the tree
template <class T, unsigned int n>
typename dbnl_bbf_tree<T,n>::dbnl_bbf_node_sptr
dbnl_bbf_tree<T,n>::build_tree( int points_per_leaf,
                                const dbnl_bbf_box<T,n>& outer_box,
                                int depth,
                                vcl_vector< int >& indices )
{
  unsigned int i;

  // 1. Build the inner box.  This is not done inside the bounding box
  //    class because of the indices.
  dbnl_bbf_box<T,n> inner_box = this->build_inner_box( indices );

  // 2. If only one point is left, create and return a leaf node.
  if ( indices.size() <= (unsigned int)points_per_leaf){
    leaf_count_++;
    return new dbnl_bbf_node<T,n> ( outer_box, inner_box, depth, indices );
  }

  // 3. Find the dimension along which there is the greatest variation
  //    in the points.
  int dim = this->greatest_variation( indices );

  // 4. Form and then sort a vector of temporary pairs, containing the
  //    indices and the values along the selected dim of the data.
  //    Sort by the values.
  vcl_vector< vcl_pair< T, int > > values( indices.size() );
  for ( i=0; i<indices.size(); ++i )
    values[i] = vcl_pair<T, int> ( points_[ indices[i] ][dim], indices[i] );
  vcl_sort( values.begin(), values.end(), first_less<T> );

  // 5. Partition the vector and the bounding box along the dimension.
  unsigned int med_loc = (indices.size()-1) / 2;
  T median_value = (values[med_loc].first + values[med_loc+1].first) / 2;
  
  dbnl_bbf_box<T,n> left_outer_box( outer_box ), right_outer_box( outer_box );
  left_outer_box.max_point_[dim] = median_value;
  right_outer_box.min_point_[dim] = median_value;
  
  vcl_vector< int > left_indices( med_loc+1 ), right_indices( indices.size()-med_loc-1 );
  for ( i=0; i<=med_loc; ++i ) left_indices[i] = values[i].second;
  for ( ; i<indices.size(); ++i ) right_indices[i-med_loc-1] = values[i].second;

  // Create a new internal node and branch
  dbnl_bbf_node_sptr node = new dbnl_bbf_node<T,n>( outer_box, inner_box, depth );
  internal_count_++;
  node->left_  = this->build_tree( points_per_leaf, left_outer_box,  depth+1, left_indices );
  node->right_ = this->build_tree( points_per_leaf, right_outer_box, depth+1, right_indices );

  return node;
}

//: Build an inner bounding box
template <class T, unsigned int n>
dbnl_bbf_box<T,n>
dbnl_bbf_tree<T,n>::build_inner_box( const vcl_vector< int >& indices )
{
  assert( indices.size() > 0 );
  vnl_vector_fixed<T,n> min_point( points_[ indices[ 0 ] ] );
  vnl_vector_fixed<T,n> max_point( min_point );

  for (unsigned int i=1; i < indices.size(); ++i ) {
    const vnl_vector_fixed<T,n>& pt = points_[ indices[ i ] ];
    for ( unsigned int j=0; j < n; ++j ) {
      if ( pt[j] < min_point[j] )
        min_point[j] = pt[j];
      if ( pt[j] > max_point[j] )
        max_point[j] = pt[j];
    }
  }
  return dbnl_bbf_box<T,n>( min_point, max_point );
}

//: Find the dimension with the greatest variation
template <class T, unsigned int n>
int
dbnl_bbf_tree<T,n>::greatest_variation( const vcl_vector<int>& indices )
{
  int dim = -1;
  T interval_size = 0;

  for ( unsigned int i=0; i<n; ++i ) {
    T min_v, max_v;
    min_v = max_v = points_[ indices[0] ][0];
    for ( unsigned int j=1; j<indices.size(); ++j ) {
      T v = points_[ indices[j] ][i];
      if ( v < min_v ) min_v = v;
      if ( v > max_v ) max_v = v;
    }
    if ( dim < 0 || max_v - min_v > interval_size ) {
      dim = i;
      interval_size = max_v - min_v;
    }
  }

  return dim;
}


//: Return an estimate of the n closest points to the query point
template <class T, unsigned int n>
void
dbnl_bbf_tree<T,n>::n_nearest_pts( const vnl_vector_fixed<T,n>& query_point,
                                   vcl_vector< vnl_vector_fixed<T,n> >& closest_points,
                                   int num, int max_search_nodes ) const
{
  vcl_vector< int > closest_indices( num );
  this->n_nearest(query_point, closest_indices, num, max_search_nodes);

  closest_points.resize( num );
  for ( unsigned int i=0; i<closest_indices.size(); ++i ) {
    closest_points[i] = points_[ closest_indices[i] ];
  }
}
    
              
//: Return an estimate of the n closest points to the query point
template <class T, unsigned int n>
void
dbnl_bbf_tree<T,n>::n_nearest( const vnl_vector_fixed<T,n>& query_point,
                               vcl_vector< int >& closest_indices,
                               int num, int max_search_nodes ) const
{
  assert(num>0);
  assert(max_search_nodes != 0);
  leaves_examined_ = internal_examined_ = 0;

  if ( closest_indices.size() != (unsigned int)num )
    closest_indices.resize( num );
    
  vcl_vector< T > sq_distances( num );
  vcl_vector< dbnl_bbf_queue_entry<T,n> > priority_queue;
  priority_queue.reserve( 100 );    // should be more than enough
  
  int num_found = 0;
  T left_box_sq_dist, right_box_sq_dist;
  T sq_dist;

  //  Go down the tree, finding the leaf node which contains the query point
  dbnl_bbf_node_sptr current = root_;
  while ( current->left_ ) {
    internal_examined_++;

    if ( dbnl_bbf_dist_sq( query_point, current->left_->outer_box_ ) <= 0 ) {
      right_box_sq_dist = dbnl_bbf_dist_sq( query_point, current->right_->inner_box_ );
      priority_queue.push_back( dbnl_bbf_queue_entry<T,n>( right_box_sq_dist, current->right_ ) );
      current = current->left_ ;
    }
    else {
      left_box_sq_dist = dbnl_bbf_dist_sq( query_point, current->left_->inner_box_ );
      priority_queue.push_back( dbnl_bbf_queue_entry<T,n>( left_box_sq_dist, current->left_ ) );
      current = current->right_ ;
    }
  }
  vcl_make_heap( priority_queue.begin(), priority_queue.end() );
  sq_dist = 0;

//   vcl_cout << "\nAfter initial trip down the tree, here's the heap\n";
//   int i;
//   for ( i=0; i<priority_queue.size(); ++i )
//     vcl_cout << "  " << i << ":  sq distance " << priority_queue[i].dist_
//              << ", node depth " << priority_queue[i].node_->depth_ << vcl_endl;
              
  bool first_leaf = true;

  do {
    // vcl_cout << "\ncurrent -- sq_dist " << sq_dist << ", depth: " << current->depth_
    //         << current->outer_box_ << "\ninner_box: "
    //      << current->inner_box_ << "\n";
    // vcl_cout << "heap size: " << heap_vec.size() << vcl_endl;
    if ( num_found < num || sq_dist < sq_distances[ num_found-1 ] ) {
      if ( ! current->left_ ) {  // a leaf node
        // vcl_cout << "Leaf\n";
        leaves_examined_ ++ ;
        update_closest( query_point, num, current, closest_indices, sq_distances, num_found );
        if ( first_leaf ) {  // check if we can quit just at this leaf node.
          // vcl_cout << "First leaf\n";
          first_leaf = false;
          if ( this-> bounded_at_leaf( query_point, num, current, sq_distances, num_found ) )
            break;
        }
      }

      else {
        // vcl_cout << "Internal\n";
        internal_examined_ ++ ;

        left_box_sq_dist = dbnl_bbf_dist_sq( query_point, current->left_->inner_box_ );
        // vcl_cout << "left sq distance = " << left_box_sq_dist << vcl_endl;
        if ( num_found < num || sq_distances[ num_found-1 ] > left_box_sq_dist ) {
          // vcl_cout << "pushing left onto the heap\n";
          priority_queue.push_back( dbnl_bbf_queue_entry<T,n>( left_box_sq_dist, current->left_ ) );
          vcl_push_heap( priority_queue.begin(), priority_queue.end() );
        };

        right_box_sq_dist = dbnl_bbf_dist_sq( query_point, current->right_->inner_box_ );
        // vcl_cout << "right sq distance = " << right_box_sq_dist << vcl_endl;
        if ( num_found < num || sq_distances[ num_found-1 ] > right_box_sq_dist ) {
          // vcl_cout << "pushing right onto the heap\n";
          priority_queue.push_back( dbnl_bbf_queue_entry<T,n>( right_box_sq_dist, current->right_ ) );
          vcl_push_heap( priority_queue.begin(), priority_queue.end() );
        }
      }
    }
    // else vcl_cout << "skipping node\n";

    if ( leaves_examined_ >= max_search_nodes && max_search_nodes > 0)
      break;
    else if ( priority_queue.size() == 0 )
      break;
    else {
      vcl_pop_heap( priority_queue.begin(), priority_queue.end() );
      sq_dist = priority_queue.back().dist_;
      current = priority_queue.back().node_;
      priority_queue.pop_back();
    }
  } while ( true );

//  vcl_cout << "\nAfter n_nearest, leaves_examined_ = " << leaves_examined_
//           << ", fraction = " << float(leaves_examined_) / leaf_count_
//           << "\n     internal_examined_ = " << internal_examined_
//           << ", fraction = " << float(internal_examined_) / internal_count_ << vcl_endl;
           
  assert(num_found >= 0);
  if(num_found < num)
    closest_indices.resize( num_found );
}

template <class T, unsigned int n>
void
dbnl_bbf_tree<T,n>::update_closest( const vnl_vector_fixed<T,n>& query_point,
                                    int num,
                                    dbnl_bbf_node_sptr p,
                                    vcl_vector< int >& closest_indices,
                                    vcl_vector< T >& sq_distances,
                                    int & num_found ) const
{
  assert(num>0);
  // vcl_cout << "Update_closest for leaf " << vcl_endl;
  // vcl_cout << " query_point = " << query_point << vcl_endl;
  // vcl_cout << " inner bounding box = \n" << p->inner_box_ << vcl_endl;
  // vcl_cout << " sq_dist = " << rsdl_dist_sq( query_point, p->inner_box_ ) << vcl_endl;

  for ( unsigned int i=0; i < p->point_indices_.size(); ++i ) {  // check each id
    int id = p->point_indices_[i];
    T sq_dist = vnl_vector_ssd( query_point, points_[ id ] );
    // vcl_cout << "  id = " << id << ", point = " << points_[ id ]
    //          << ", sq_dist = " << sq_dist << vcl_endl;

    // if enough points have been found and the distance of this point is
    // too large then skip it.

    if ( num_found >= num && sq_dist >= sq_distances[ num_found-1 ] )
      continue;

    // Increment the num_found counter if fewer than the desired
    // number have already been found.

    if ( num_found < num ) {
      num_found ++;
    }

    // Insert the id and square distance in order.

    int j=num_found-2;
    while ( j >= 0 && sq_distances[j] > sq_dist ) {
      closest_indices[ j+1 ] = closest_indices[ j ];
      sq_distances[ j+1 ] = sq_distances[ j ];
      j -- ;
    }
    closest_indices[ j+1 ] = id;
    sq_distances[ j+1 ] = sq_dist;
  }
  // vcl_cout << "  End of leaf computation, num_found =  " << num_found
  //          << ", and they are: " << vcl_endl;
  // for ( int k=0; k<num_found; ++k )
  //   vcl_cout << "     " << k << ":  indices: " << closest_indices[ k ]
  //            << ", sq_dist " << sq_distances[ k ] << vcl_endl;
}


//  If there are already n points (at the first leaf) and if a box
//  drawn around the first point of half-width the distance of the
//  n-th point fits inside the outer box of this node, then no points
//  anywhere else in the tree will can replace any of the closest
//  points.
template <class T, unsigned int n>
bool
dbnl_bbf_tree<T,n>::bounded_at_leaf( const vnl_vector_fixed<T,n>& query_point,
                                     int num,
                                     dbnl_bbf_node_sptr current,
                                     const vcl_vector< T >& sq_distances,
                                     int & num_found ) const
{
  assert(num>0);
  // vcl_cout << "bounded_at_leaf\n"
  //          << "num_found = " << num_found << "\n";

  if ( num_found != num ) {
    return false;
  }

  double radius = vcl_sqrt( sq_distances[ num-1 ] );

  for ( unsigned int i=0; i < n; ++i ) {
    double x = query_point[i];
    if ( current->outer_box_.min_point_[i] > x - radius ||
         current->outer_box_.max_point_[i] < x + radius ) {
      return false;
    }
  }

  return true;
}

#define DBNL_BBF_TREE_INSTANTIATE(T,n) \
template class dbnl_bbf_box< T,n >; \
template class dbnl_bbf_node< T,n >; \
template class dbnl_bbf_queue_entry< T,n >; \
template class dbnl_bbf_tree< T,n >; \
VCL_INSTANTIATE_INLINE(bool first_less( const vcl_pair<T,int>& left, \
                                        const vcl_pair<T,int>& right )); \
typedef dbnl_bbf_node<T,n> dbnl_bbf_node_inst; \
VBL_SMART_PTR_INSTANTIATE( dbnl_bbf_node_inst );
