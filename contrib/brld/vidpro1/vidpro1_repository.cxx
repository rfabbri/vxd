// This is brl/vidpro1/vidpro1_repository.cxx
#include <vidpro1/vidpro1_repository.h>
#include <bpro1/bpro1_storage.h>
#include <vcl_iostream.h>
#include <vcl_cassert.h>
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_map_io.h>
#include <vsl/vsl_vector_io.h>
#include <vsl/vsl_set_io.h>

//:
// \file

vcl_map<vcl_string, bpro1_storage_sptr> vidpro1_repository::registered_types_;


//: Constructor
vidpro1_repository::vidpro1_repository()
: current_frame_(0)
{}


//: Registers a storage type with the repository
//  Call this function with a null smart pointer of each type you plan to use
bool
vidpro1_repository::register_type(const bpro1_storage_sptr& dummy_storage)
{
  if( registered_types_.find(dummy_storage->type()) != registered_types_.end())
    return false;
    
  registered_types_[dummy_storage->type()] = dummy_storage->clone();
  vsl_add_to_binary_loader(*dummy_storage);

  return true;
}


//: Returns returns the set of strings representing all registered types
vcl_set<vcl_string> 
vidpro1_repository::types() const
{
  vcl_set<vcl_string> types;
  for ( vcl_map<vcl_string, bpro1_storage_sptr>::const_iterator 
        itr = registered_types_.begin();
        itr != registered_types_.end();  ++itr)
  {
    types.insert(itr->first);
  }
  return types;
}


//: Clear all data from the repository
void
vidpro1_repository::remove_all()
{
  data_.clear();
  global_data_.clear();
  current_frame_ = 0;
}


//: Remove all data from the repository except those with the given names
void 
vidpro1_repository::remove_all_except(const vcl_set<vcl_string>& retain)
{
  for ( unsigned frame=0; frame<data_.size(); ++frame ){
    for ( storage_map::iterator itr = data_[frame].begin(); 
          itr != data_[frame].end(); ++itr )
    {
      // erasing invalidates iterators so we have to start over until
      // we remove them all
      bool removed = true;
      while( removed ){
        removed = false;
        for ( vcl_vector<bpro1_storage_sptr>::iterator s_itr = itr->second.begin();
        s_itr != itr->second.end(); ++s_itr)
        {
          if(retain.count((*s_itr)->name()) == 0){
            itr->second.erase(s_itr);
            removed = true;
            break;
          }
        }
      }
    }
  }
  for ( storage_map::iterator itr = global_data_.begin(); 
        itr != global_data_.end(); ++itr )
  {
    // erasing invalidates iterators so we have to start over until
    // we remove them all
    bool removed = true;
    while( removed ){
      removed = false;
      for ( vcl_vector<bpro1_storage_sptr>::iterator s_itr = itr->second.begin();
      s_itr != itr->second.end(); ++s_itr)
      {
        if(retain.count((*s_itr)->name()) == 0){
          itr->second.erase(s_itr);
          removed = true;
          break;
        }
      }
    }
  }
}


//: Clear the repository and replace with the data in new_rep;
// \note the registered types in new_rep must be a subset of types registered here
void 
vidpro1_repository::replace_data(const vidpro1_repository_sptr& new_rep)
{
  this->remove_all();
  this->initialize(new_rep->num_frames());
 // this->data_ = new_rep->data_;
  for ( int frame = 0; frame < new_rep->num_frames(); ++frame ){
    for ( storage_map::const_iterator itr = new_rep->data_[frame].begin(); 
          itr != new_rep->data_[frame].end(); ++itr )
    {
      this->data_[frame][itr->first] = itr->second; 
    }
  }
  for ( storage_map::const_iterator itr = new_rep->global_data_.begin(); 
        itr != new_rep->global_data_.end(); ++itr )
  {
    this->global_data_[itr->first] = itr->second; 
  }

}
//: add in a new repository to the existing storage elements
bool vidpro1_repository::add_repository(const vidpro1_repository_sptr& new_rep)
{
  if(!new_rep)
    {
      vcl_cerr << __FILE__ << ": warning, trying to add null repository "
               << vcl_endl;
      return false;
    }
  int n_frames = static_cast<int>(data_.size());
  assert (n_frames>=0);
  for ( int frame = 0; frame < new_rep->num_frames(); ++frame )
    if(frame<n_frames){
      for ( storage_map::const_iterator itr = new_rep->data_[frame].begin(); 
            itr != new_rep->data_[frame].end(); ++itr )
        {
          const vcl_vector<bpro1_storage_sptr>& temp = itr->second;
          for(vcl_vector<bpro1_storage_sptr>::const_iterator sit = temp.begin();
              sit != temp.end(); ++sit)
            {
              storage_map::iterator it = data_[frame].find((*sit)->type());
              if( it != data_[frame].end() )
                it->second.push_back( *sit );
            }
        }}
    else
      data_.push_back(new_rep->data_[frame]);
      
    for ( storage_map::const_iterator itr = new_rep->global_data_.begin(); 
          itr != new_rep->global_data_.end(); ++itr )
    {
      const vcl_vector<bpro1_storage_sptr>& temp = itr->second;
      for(vcl_vector<bpro1_storage_sptr>::const_iterator sit = temp.begin();
          sit != temp.end(); ++sit)
        {
          storage_map::iterator it = global_data_.find((*sit)->type());
          if( it != global_data_.end() )
            it->second.push_back( *sit );
        }
    }
  return true;
}

void
vidpro1_repository::initialize( int num_frames )
{
  this->remove_all();
  // create a template map of NULL smart pointers
  //storage_map <==> vcl_map< vcl_string, vcl_vector< bpro1_storage_sptr > >
  vidpro1_repository::storage_map temp_map;
  for( vcl_map<vcl_string, bpro1_storage_sptr>::const_iterator 
       type_itr = registered_types_.begin();
       type_itr != registered_types_.end();  ++type_itr ){
    temp_map[type_itr->first] = vcl_vector< bpro1_storage_sptr >();
  }
  // Copy the template map for each frame added
  data_.resize(num_frames, temp_map);
  global_data_ = temp_map;
}

void vidpro1_repository::initialize_global()
{
  for( vcl_map<vcl_string, bpro1_storage_sptr>::const_iterator 
       type_itr = registered_types_.begin();
       type_itr != registered_types_.end();  ++type_itr ){
    global_data_[type_itr->first] = vcl_vector< bpro1_storage_sptr >();
  }
}

void
vidpro1_repository::add_new_frame()
{
  // create a map of NULL smart pointers
  vidpro1_repository::storage_map temp_map;
  for( vcl_map<vcl_string, bpro1_storage_sptr>::const_iterator 
       type_itr = registered_types_.begin();
       type_itr != registered_types_.end();  ++type_itr ){
    temp_map[type_itr->first] = vcl_vector< bpro1_storage_sptr >();
  }
  data_.push_back( temp_map );
}


//: Increment the current frame number if not at the end
bool
vidpro1_repository::go_to_next_frame()
{
  if( current_frame_ + 1 < int(data_.size()) ) {
    current_frame_++;
    return true;
  }
  return false;
}

bool
vidpro1_repository::go_to_next_n_frame(int n)
{
  if( current_frame_ + n < int(data_.size()) ) {
    current_frame_+=n;
    return true;
  }
  return false;
}

//: Decrement the current frame number if not at the beginning
bool
vidpro1_repository::go_to_prev_frame()
{
  if( current_frame_ > 0 ) {
    current_frame_--;
    return true;
  }
  return false;
}
bool
vidpro1_repository::go_to_prev_n_frame(int n)
{
  if( current_frame_ - n >=0 ) {
    current_frame_-=n;
    return true;
  }
  return false;
}

//: Jump the the specified frame number if valid
bool
vidpro1_repository::go_to_frame( int num )
{
  if( valid_frame( num ) ){
    current_frame_ = num;
    return true;
  }
  return false;
}


//: Returns true if the specified frame is valid
bool
vidpro1_repository::valid_frame( int frame ) const
{
  return ( frame >= 0 &&
           frame < int(data_.size()) &&
           !data_.empty() );
}


//: Return an iterator to the beginning of the data at the current frame
vidpro1_repository::storage_map::iterator
vidpro1_repository::map_begin()
{
  assert(!data_.empty());
  return data_[current_frame_].begin();
}


//: Return an iterator to the end of the data at the current frame
vidpro1_repository::storage_map::iterator
vidpro1_repository::map_end()
{
  assert(!data_.empty());
  return data_[current_frame_].end();
}


//: Return an iterator to the beginning of the data at the current frame
vidpro1_repository::storage_map::iterator
vidpro1_repository::global_map_begin()
{
  return global_data_.begin();
}


//: Return an iterator to the end of the data at the current frame
vidpro1_repository::storage_map::iterator
vidpro1_repository::global_map_end()
{
  return global_data_.end();
}


//: Retrieve a vector of names that describe the storage classes of a given type
vcl_vector < vcl_string > 
vidpro1_repository::get_all_storage_class_names(const vcl_string& type)
{
  return get_all_storage_class_names(type, current_frame_);
}


//: Retrieve a vector of names that describe the storage classes of a given type
vcl_vector < vcl_string > 
vidpro1_repository::get_all_storage_class_names(const vcl_string& type, int frame)
{
  vcl_vector<vcl_string> names;

  if(frame >= -1){
    storage_map::iterator it = global_data_.find(type);
    if( it != global_data_.end() ){
      unsigned ind = 0;
      for (; ind < it->second.size(); ind++)
        names.push_back(it->second[ind]->name());
    }
  }
  if( valid_frame( frame ) ) {
    storage_map::iterator it = data_[frame].find(type);
    if( it != data_[frame].end() ){
      unsigned ind = 0;
      for (; ind < it->second.size(); ind++)
        names.push_back(it->second[ind]->name());
    }
  }

  return names;
}

  
//: Returns the set of all storage classes (all types) at the given frame
vcl_set < bpro1_storage_sptr > 
vidpro1_repository::get_all_storage_classes(int frame) const
{
  vcl_set < bpro1_storage_sptr > storage_set;
  if(frame >= -1){
    for(storage_map::const_iterator it = global_data_.begin();
        it != global_data_.end();  ++it )
    {
      for (unsigned ind = 0; ind < it->second.size(); ind++)
        storage_set.insert(it->second[ind]);
    }
  }
  if( valid_frame( frame ) ) {
    for(storage_map::const_iterator it = data_[frame].begin();
        it != data_[frame].end();  ++it )
    {
      for (unsigned ind = 0; ind < it->second.size(); ind++)
        storage_set.insert(it->second[ind]);
    }
  }
  return storage_set;
}


//: Returns the number of storage classes of a given type at the current frame
int 
vidpro1_repository::get_storage_class_size(const vcl_string& type) const
{
  if( valid_frame( current_frame_ ) ) {
    storage_map::const_iterator it = data_[current_frame_].find(type);
    if( it != data_[current_frame_].end() )
      return it->second.size();
  }
  return 0;
}


//: Retrieve a storage smart pointer to the data named \p name at the current frame
//  The optional frame_offset is added to the current frame number
bpro1_storage_sptr 
vidpro1_repository::get_data_by_name(const vcl_string& name, int frame_offset )
{
  return get_data_by_name_at( name, current_frame_ + frame_offset);
}


//: Retrieve a storage smart pointer to the data named \p name at the given frame
bpro1_storage_sptr 
vidpro1_repository::get_data_by_name_at( const vcl_string& name, int frame)
{
  // search global data
  for( storage_map::iterator t_itr = global_data_.begin();
        t_itr != global_data_.end(); ++t_itr )
  {
    for( vcl_vector< bpro1_storage_sptr >::const_iterator s_itr = t_itr->second.begin();
          s_itr != t_itr->second.end();  ++s_itr )
    {
      if( (*s_itr)->name() == name )
        return *s_itr;
    }
  }
  // search local data
  if( valid_frame( frame ) ) {
    for( storage_map::iterator t_itr = data_[frame].begin();
         t_itr != data_[frame].end(); ++t_itr )
    {
      for( vcl_vector< bpro1_storage_sptr >::const_iterator s_itr = t_itr->second.begin();
           s_itr != t_itr->second.end();  ++s_itr )
      {
        if( (*s_itr)->name() == name )
          return *s_itr;
      }
    }
  }
  return NULL;
}


//: Retrieve a storage smart pointer to the data indexed by ind of a given type at the current frame
//  The optional frame_offset is added to the current frame number
bpro1_storage_sptr 
vidpro1_repository::get_data(const vcl_string& type, int frame_offset, int ind)
{
  return get_data_at( type, current_frame_ + frame_offset, ind);
}



//: Retrieve a storage smart pointer to the data indexed by ind of a given type at the given frame
bpro1_storage_sptr 
vidpro1_repository::get_data_at(const vcl_string& type, int frame, int ind)
{
  if( frame == -1 ) {
    storage_map::iterator it = global_data_.find(type);
    if( it != global_data_.end() )
      if( int(it->second.size()) > ind)
        return it->second[ind];
  }
  if( valid_frame( frame ) ) {
    storage_map::iterator it = data_[frame].find(type);
    if( it != data_[frame].end() )
      if( int(it->second.size()) > ind)
        return it->second[ind];
  }
  return NULL;
}


//: Store the storage smart pointer to the data at the current frame
//  The optional frame_offset is added to the current frame number
//  Returns false if no storage of this type has been defined
bool 
vidpro1_repository::store_data(const bpro1_storage_sptr& storage, int frame_offset)
{
   return store_data_at( storage, current_frame_ + frame_offset );
}

//: Pop back the storage smart pointer to the data at the current frame with the given type
//  The optional frame_offset is added to the current frame number
//  Returns false if no storage of this type has been defined
bool 
vidpro1_repository::pop_data(const vcl_string& type, int frame_offset)
{
   return pop_data_at( type, current_frame_ + frame_offset );
}

//: Store the storage smart pointer to the data at the given frame
//  Returns false if no storage of this type has been defined
bool 
vidpro1_repository::store_data_at(const bpro1_storage_sptr& storage, int frame)
{
  if( !storage.ptr() )
    return false;
    
  if( storage->frame() >= -1 ){
    vcl_cerr << __FILE__ << ": warning, this storage class is already stored at frame "
             << storage->frame() << vcl_endl;
    return false;
  }
  
  // special case allowing a storage class to override local storage and become global
  if( storage->frame() == -2 )
    frame = -1;
  
  // store global data
  if( frame == -1 ){
     storage_map::iterator it = global_data_.find(storage->type());
    if( it != global_data_.end() ){
      storage->set_frame(-1);
      it->second.push_back( storage );
      return true;
    }
    else{
      vcl_cerr << __FILE__ << ": error, unknown storage type "
               << storage->type() << vcl_endl;
      return false;
    }
  }
  
  // store local data
  if( valid_frame( frame ) ) {
    storage_map::iterator it = data_[frame].find(storage->type());
      storage->set_frame(frame);
      it->second.push_back( storage );
      return true;
    }
  
  return false;
}

//: Pop back the storage smart pointer to the data at the given frame with the given type
//  Returns false if no storage of this type has been defined
bool 
vidpro1_repository::pop_data_at(const vcl_string& type, int frame) 
{  
  // pop local data
  if( frame == -1 ){
    storage_map::iterator it = global_data_.find(type);
    if( it != global_data_.end() ){
      it->second.pop_back();
      return true;
    }
  }
  
  // pop global data    
  if( valid_frame( frame ) ) {
    storage_map::iterator it = data_[frame].find(type);
    if( it != data_[frame].end() ){
      it->second.pop_back();
      return true;
    }
  }
  return false;
}

//: Create a new empty storage class
//  The optional frame_offset is added to the current frame number
//  \return false if this data type is not registered
bpro1_storage_sptr  
vidpro1_repository::new_data( const vcl_string& type, 
                             const vcl_string& name, 
                             int frame_offset )
{
  return new_data_at( type, name, current_frame_ + frame_offset );
}


//: Create a new empty storage class
//  \return false if this data type is not registered
bpro1_storage_sptr  
vidpro1_repository::new_data_at( const vcl_string& type, 
                                const vcl_string& name, 
                                int frame )
{
  if( name == "" )
    return NULL;
  vcl_map<vcl_string, bpro1_storage_sptr>::iterator 
    result = registered_types_.find(type);
  if( result == registered_types_.end() )
    return NULL;

  bpro1_storage_sptr blank = result->second->clone();
  blank->set_name(name);
  if(!this->store_data_at(blank, frame))
    return NULL;

  return blank;
}


//: Binary save self to stream.
void 
vidpro1_repository::b_write(vsl_b_ostream &os) const
{
  vsl_b_write(os, version());
  vsl_b_write(os, this->data_);
  vsl_b_write(os, this->global_data_);
}


//: Binary load self from stream.
void 
vidpro1_repository::b_read(vsl_b_istream &is)
{
  if (!is) return;

  short ver;
  vsl_b_read(is, ver);
  switch(ver)
  {
  case 1:{
    vcl_set<vcl_string> dummy1;
    vsl_b_read(is, dummy1);
    vsl_b_read(is, this->data_);
    storage_map dummy2;
    vsl_b_read(is, dummy2);
    break;}
  case 2:
    vsl_b_read(is, this->registered_types_);
    vsl_b_read(is, this->data_);
    break;
  case 3:
    vsl_b_read(is, this->data_);
    break;
  case 4:
    vsl_b_read(is, this->data_);
    vsl_b_read(is, this->global_data_);
    break;


  default:
    vcl_cerr << "I/O ERROR: bpro1_storage::b_read(vsl_b_istream&)\n"
             << "           Unknown version number "<< ver << '\n';
    is.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}


//: Return IO version number;
short 
vidpro1_repository::version() const
{
  return 4;
}


//: Print a summary of the repository structure to cout
void 
vidpro1_repository::print_summary()
{
  vcl_cout << vcl_endl << "**** vidpro1_repository summary: ****" << vcl_endl;
  vcl_cout << "Number of frames: " << num_frames() << vcl_endl;

  vcl_cout << "Registered Types: " ;
  
  for ( vcl_map<vcl_string, bpro1_storage_sptr>::iterator 
        rt_itr = registered_types_.begin(); 
        rt_itr != registered_types_.end();  ++rt_itr)
    vcl_cout << (rt_itr->first) << ", " ;
  vcl_cout << vcl_endl;

  // data storage classes
  if (num_frames()) {
     storage_map::iterator map_itr = data_[current_frame_].begin();
     for(; map_itr!=data_[current_frame_].end(); map_itr++) {
       if (map_itr->second.size()==0) continue;

       vcl_cout << map_itr->first << ": " ;
       for (unsigned i=0; i<map_itr->second.size(); i++)
         vcl_cout << map_itr->second[i]->name() << " ";
       vcl_cout << vcl_endl;
     }
  }
  storage_map::iterator map_itr = global_data_.begin();
  for(; map_itr!=global_data_.end(); map_itr++) {
    if (map_itr->second.size()==0) continue;

    vcl_cout << map_itr->first << ": " ;
    for (unsigned i=0; i<map_itr->second.size(); i++)
      vcl_cout << map_itr->second[i]->name() << " ";
    vcl_cout << vcl_endl;
  }
  vcl_cout << "************************************" << vcl_endl;
}

//: Binary save vidpro1_repository* to stream.
void
vsl_b_write(vsl_b_ostream &os, const vidpro1_repository* r)
{
  if (r==0){
    vsl_b_write(os, false); // Indicate null pointer stored
  }
  else{
    vsl_b_write(os,true); // Indicate non-null pointer stored
    r->b_write(os);
  }
}


//: Binary load vidpro1_repository* from stream.
void
vsl_b_read(vsl_b_istream &is, vidpro1_repository* &r)
{
  delete r;
  bool not_null_ptr;
  vsl_b_read(is, not_null_ptr);
  if (not_null_ptr){
    r = new vidpro1_repository();
    r->b_read(is);
  }
  else
    r = 0;
}


//: Print an ASCII summary to the stream
void
vsl_print_summary(vcl_ostream &os, const vidpro1_repository* r)
{
  os << "vidpro1_repository{";
  if(r)
    os << r->num_frames() << " frames";
  else
    os << "NULL";
  os << "}";
}

