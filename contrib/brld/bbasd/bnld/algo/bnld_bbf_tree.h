// This is basic/dbnl/algo/dbnl_bbf_tree.h
#ifndef dbnl_bbf_tree_h_
#define dbnl_bbf_tree_h_
//:
// \file
// \brief Best-Bin-First search (limited search KD-tree)
// \author Matthew Leotta, (mleotta@lems.brown.edu)
// \date Sun Nov 9 2003
//
// This implementation is based on the kd-tree code found in contrib/rsdl
//
// \verbatim
//  Modifications
// \endverbatim

#include <vcl_vector.h>
#include <vbl/vbl_ref_count.h>
#include <vbl/vbl_smart_ptr.h>
#include <vnl/vnl_vector_fixed.h>

//: A bounding box for BBF nodes
template <class T, unsigned int n>
class dbnl_bbf_box
{
public:
  //: Constructor
  dbnl_bbf_box<T,n>();
  dbnl_bbf_box<T,n>( const vnl_vector_fixed<T,n>& min_point,
                const vnl_vector_fixed<T,n>& max_point );

  //: Copy constructor
  dbnl_bbf_box<T,n>( const dbnl_bbf_box<T,n>& old );

  //: Assignment operator
  dbnl_bbf_box<T,n>& operator= ( const dbnl_bbf_box<T,n>& old );

  //: Minimum point in all boundary dimensions
  vnl_vector_fixed<T,n> min_point_;
  //: Maximum point in all boundary dimensions
  vnl_vector_fixed<T,n> max_point_;
};


//: Return the minimum square distance between \a p and any point in \a b.
template <class T, unsigned int n>
T dbnl_bbf_dist_sq( const vnl_vector_fixed<T,n>& p, const dbnl_bbf_box<T,n>& b );


//: A node in the BBF tree
template <class T, unsigned int n>
class dbnl_bbf_node : public vbl_ref_count
{
 public:
  typedef vbl_smart_ptr<dbnl_bbf_node< T,n > > dbnl_bbf_node_sptr;
  
  //: Constructor for internal node
  dbnl_bbf_node( const dbnl_bbf_box<T,n>& outer_box,
                 const dbnl_bbf_box<T,n>& inner_box,
                unsigned int depth )
    : outer_box_(outer_box), inner_box_(inner_box), depth_(depth),
      point_indices_(0), left_(0), right_(0) {}

  //: Constuctor for leaf node
  dbnl_bbf_node( const dbnl_bbf_box<T,n>& outer_box,
                 const dbnl_bbf_box<T,n>& inner_box,
                 unsigned int depth,
                 const vcl_vector<int>& indices )
    : outer_box_(outer_box), inner_box_(inner_box), depth_(depth),
      point_indices_(indices), left_(0), right_(0) {}
  
  //: Outer bounding box
  dbnl_bbf_box<T,n> outer_box_;
  //: Inner bounding box
  dbnl_bbf_box<T,n> inner_box_;
  //: Depth of node in the tree
  unsigned int depth_;
  //: Indices of the points stored at this leaf
  vcl_vector< int > point_indices_;
  //: Left child
  dbnl_bbf_node_sptr left_;
  //: Right child
  dbnl_bbf_node_sptr right_;
};


// A light-weight class used in the priorty queue
template <class T, unsigned int n>
class dbnl_bbf_queue_entry
{
public:
  typedef vbl_smart_ptr<dbnl_bbf_node< T,n > > dbnl_bbf_node_sptr;

  //: Constructor
  dbnl_bbf_queue_entry() {}
  //: Constructor
  dbnl_bbf_queue_entry( T dist, dbnl_bbf_node_sptr node )
    : dist_(dist), node_(node) {}
  //: Used in sorting by distance
  bool operator< ( const dbnl_bbf_queue_entry& right ) const
  { return right.dist_ < this->dist_; } 

  //: Distance to this node
  T dist_;
  //: Smart pointer to the node
  dbnl_bbf_node_sptr node_;
};


//: The BBF tree data structure
template <class T, unsigned int n>
class dbnl_bbf_tree
{
public:
  typedef vbl_smart_ptr<dbnl_bbf_node< T,n > > dbnl_bbf_node_sptr;

  //: Constuctor
  dbnl_bbf_tree( const vcl_vector< vnl_vector_fixed<T,n> >& points,
                 int points_per_leaf=4 );

  //: Return an estimate of the n closest points to the query point
  // \param n is the number of nearest nodes to return
  // \param max_search_nodes is the number of nodes to examine (-1 means all)
  void n_nearest( const vnl_vector_fixed<T,n>& query_point,
                  vcl_vector< int >& closest_indices,
                  int b=1, int max_search_nodes=-1 ) const;
  
  //: Return an estimate of the n closest points to the query point
  // \param n is the number of nearest nodes to return
  // \param max_search_nodes is the number of nodes to examine (-1 means all)
  void n_nearest_pts( const vnl_vector_fixed<T,n>& query_point,
                      vcl_vector< vnl_vector_fixed<T,n> >& closest_points,
                      int b=1, int max_search_nodes=-1) const;
  
private:
  //: Build the tree
  dbnl_bbf_node_sptr build_tree( int points_per_leaf,
                                 const dbnl_bbf_box<T,n>& outer_box,
                                 int depth,
                                 vcl_vector< int >& indices );
  //: Build an inner bounding box
  dbnl_bbf_box<T,n> build_inner_box( const vcl_vector< int >& indices );
  //: Find the dimension with the greatest variation
  int greatest_variation( const vcl_vector<int>& indices );
  //: Update
  void update_closest( const vnl_vector_fixed<T,n>& query_point, int b,
                       dbnl_bbf_node_sptr p, vcl_vector< int >& closest_indices,
                       vcl_vector< T >& sq_distances, int & num_found ) const;
  //: See if the current leaf contains the NN neighbors
  bool bounded_at_leaf( const vnl_vector_fixed<T,n>& query_point, int b,
                        dbnl_bbf_node_sptr current, const vcl_vector< T >& sq_distances,
                        int & num_found ) const;

  //: The number of leaves in the tree
  int leaf_count_;
  //: The number of leaves examined in a search
  mutable int leaves_examined_;
  //: The number of internal nodes in the tree
  int internal_count_;
  //: The number of internal nodes examined in a search
  mutable int internal_examined_;
  //: the root node in the tree
  dbnl_bbf_node_sptr root_;
  //: vector of keypoints in the tree
  vcl_vector< vnl_vector_fixed<T,n> > points_;
};

#endif // dbnl_bbf_tree_h_
