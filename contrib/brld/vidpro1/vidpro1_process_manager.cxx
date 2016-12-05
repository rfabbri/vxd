// This is brl/bpro1/vidpro1_process_manager.cxx

//:
// \file

#include "vidpro1_process_manager.h"

#include <vcl_cassert.h>
#include <vcl_utility.h>
#include <vul/vul_arg.h>

#include <bpro1/bpro1_storage.h>
#include <bpro1/bpro1_process.h>
#include <vidpro1/vidpro1_repository.h>

vcl_multimap< vcl_string , bpro1_process_sptr > vidpro1_process_manager::process_map;

//: Constructor
vidpro1_process_manager::vidpro1_process_manager()
 : repository_sptr(new vidpro1_repository())
{
  process_queue.clear();
  non_causal_=false;
}


//: Destructor
vidpro1_process_manager::~vidpro1_process_manager()
{
  process_queue.clear();
}


//: Return a process with the given name
bpro1_process_sptr
vidpro1_process_manager::get_process_by_name( const vcl_string& name ) const
{
  vcl_multimap< vcl_string , bpro1_process_sptr >::const_iterator it = process_map.find( name );
  if( it == process_map.end() ) {
    return NULL ;
  }
  return it->second;
}


//: Return a smart pointer to the repository
vidpro1_repository_sptr
vidpro1_process_manager::get_repository_sptr() const
{
  return repository_sptr;
}


//: Set the process manager to use a new repository
void
vidpro1_process_manager::set_repository(const vidpro1_repository_sptr& rep)
{
  this->repository_sptr = rep;
  process_queue.clear();
}

void
vidpro1_process_manager::set_non_causual(bool non_causal)
{
  non_causal=non_causal_;
}

//: Run a process on the current frame
bool
vidpro1_process_manager::run_process_on_current_frame( const bpro1_process_sptr& process,
                                                      vcl_set<bpro1_storage_sptr>* modified )
{
  bool to_return = false;

  process->clear_input();
  process->clear_output();

  // SET INPUTS ////////////////////////////////////////////

  vcl_vector< vcl_string > input_type_list = process->get_input_type();
  vcl_vector< vcl_string > input_names = process->input_names();

  for( unsigned int i = 0 ;
      i < input_names.size();
      i++ ) {
          if(!process->non_causal())
          {
              
              //Get the right storage class from the repository from the previous N frames the process needs
              for(  int a = 0 ;
                  a < process->input_frames();
                  a++ ) {

                      //use the storage names in the process that maps to the exact input to be used
                      bpro1_storage_sptr input_storage_sptr = repository_sptr->get_data_by_name( input_names[i], -a );
                      //assert(input_type_list[i] == input_storage_sptr->type());
                      process->add_input(input_storage_sptr, a);
              }

          }
          else //: to access future frames
          {
              //Get the right storage class from the repository from the previous N frames the process needs
              for(  int a = -process->input_frames();
                  a <= process->input_frames();
                  a++ ) {

                      //use the storage names in the process that maps to the exact input to be used
                      bpro1_storage_sptr input_storage_sptr = repository_sptr->get_data_by_name( input_names[i], -a );
                      //assert(input_type_list[i] == input_storage_sptr->type());
                      process->add_input(input_storage_sptr, a+process->input_frames());
              }
          }
  }

  vcl_vector<vcl_vector<bpro1_storage_sptr> >  initial_inputs;
  for(int a = 0; a < process->input_frames(); ++a)
    initial_inputs.push_back(process->get_input(a));

  //////////////////////////////////////////////////////////
  // initial number of output frames
  int num_outputs = process->output_frames();


  // EXECUTE ///////////////////////////////////////////////
  to_return = process->execute();
  //////////////////////////////////////////////////////////

  // See if any missing input storage classes have been created
  if( to_return ){
    for(int a = 0; a < process->input_frames(); ++a){
      vcl_vector<bpro1_storage_sptr> final_inputs = process->get_input(a);
      for(unsigned int i = 0; i < final_inputs.size(); ++i){
        if( final_inputs[i] && !initial_inputs[a][i] ){
          final_inputs[i]->set_name(input_names[i]);
          to_return = repository_sptr->store_data( final_inputs[i], -a );
          if (modified)
            modified->insert(final_inputs[i]);
        }
      }
    }
  }
  
  int frame_offset = process->output_frames() - num_outputs - 1;
  int curr_frame = repository_sptr->current_frame();
  while( (frame_offset > 0) && repository_sptr->go_to_next_frame() ) --frame_offset;
  while(frame_offset > 0) {
    repository_sptr->add_new_frame();
    repository_sptr->go_to_next_frame();
    --frame_offset;
  }

  // GET OUTPUTS ///////////////////////////////////////////


  if( to_return ) {
    for( int a = 0 ;
        a < process->output_frames();
        a++ ) {

      vcl_vector < bpro1_storage_sptr > output_storage_classes = process->get_output(a);
      vcl_vector< vcl_string > output_names = process->output_names();


      // Any outputs beyond those specified with output_names() are 
      // added to the modified set but not added to the repository
      // it is assumed that these are preexisting but modified data
      for( unsigned int i = 0 ;
          i < output_storage_classes.size();
          i++ )
      {
        if(!output_storage_classes[i])
          continue;
        
        if(i < output_names.size()){
          if(output_storage_classes[i]->name()=="defaultname")
            //assign the chosen names for the output classes
            output_storage_classes[i]->set_name( output_names[i]);

          //storage class types are automatically determined by the repository
          to_return = repository_sptr->store_data( output_storage_classes[i], -a );
        }
        if (modified)
          modified->insert(output_storage_classes[i]);

      }
    }
  }

  //Amir: The processes should not keep the input and output storage classes
  //      once the process has been executed. This is preventing those storage
  //      classes from being destructed.
  process->clear_input();
  process->clear_output();

  //////////////////////////////////////////////////////////
  // return to the original frame
  repository_sptr->go_to_frame(curr_frame);

  // RETURN VALUE //////////////////////////////////////////
  return to_return;
  //////////////////////////////////////////////////////////
}



bool
vidpro1_process_manager::run_process_queue_on_current_frame( vcl_set<bpro1_storage_sptr>* modified )
{
  vcl_vector< bpro1_process_sptr >::iterator it = process_queue.begin();
  for (; it != process_queue.end(); ++it){
    bool success = run_process_on_current_frame( (*it), modified );
    if( !success )
      return false;
  }

  return true;
}


//: Call the finish function on the process
bool
vidpro1_process_manager::finish_process( int first_frame, int last_frame,
                                        const bpro1_process_sptr& process,
                                        vcl_set<bpro1_storage_sptr>* modified )
{
  
  process->clear_input(last_frame+1);
  process->clear_output(last_frame+1);

  // SET INPUTS ////////////////////////////////////////////

  vcl_vector< vcl_string > input_type_list = process->get_input_type();
  vcl_vector< vcl_string > input_names = process->input_names();

  vcl_vector< vcl_string > output_type_list = process->get_output_type();
  vcl_vector< vcl_string > output_names = process->output_names();

  //Get the right storage classes from the repository
  for( int f = first_frame; f <= last_frame; ++f) {
    for( unsigned i = 0; i < input_names.size(); ++i ) {
      //use the storage names in the process that maps to the exact input to be used
      bpro1_storage_sptr input_storage_sptr = repository_sptr->get_data_by_name_at( input_names[i], f );
      //assert(input_storage_sptr && input_type_list[i] == input_storage_sptr->type());
      process->add_input(input_storage_sptr, f);
    }
    for( unsigned i = 0; i < output_names.size(); ++i ) {
      //use the storage names in the process that maps to the exact input to be used
      bpro1_storage_sptr output_storage_sptr = repository_sptr->get_data_by_name_at( output_names[i], f );
      if(output_storage_sptr){
        //assert(output_type_list[i] == output_storage_sptr->type());
        process->add_input(output_storage_sptr, f);
      }
    }
  }

  if(!process->finish())
    return false;

  // GET OUTPUTS ///////////////////////////////////////////
  for( int f = 0; f <= last_frame; ++f)
  {
    vcl_vector < bpro1_storage_sptr > output_storage_classes = process->get_output(f);
    vcl_vector< vcl_string > output_names = process->output_names();

    // Any outputs beyond those specified with output_names() are 
    // added to the modified set but not added to the repository
    // it is assumed that these are preexisting but modified data
    for(unsigned int i = 0; i < output_storage_classes.size(); ++i )
    {
      if(!output_storage_classes[i])
        continue;
        
      if(i < output_names.size()){
        if(output_storage_classes[i]->name()=="defaultname")
          //assign the chosen names for the output classes
          output_storage_classes[i]->set_name( output_names[i]);
  
        if(output_storage_classes[i]->frame() < 0)
          repository_sptr->store_data_at( output_storage_classes[i], f );
      }
      if (modified)
        modified->insert(output_storage_classes[i]);

    }
  }
  return true;
}


//: Call the finish function on the process queue
bool
vidpro1_process_manager::finish_process_queue( int first_frame, int last_frame,
                                              vcl_set<bpro1_storage_sptr>* modified )
{
  vcl_vector< bpro1_process_sptr >::iterator it = process_queue.begin();
  for (; it != process_queue.end(); ++it){
    bool success = finish_process( first_frame, last_frame, (*it), modified );
    if( !success )
      return false;
  }

  return true;
}



void
vidpro1_process_manager::add_process_to_queue( const bpro1_process_sptr& process )
{
  process_queue.push_back(process);
}


void
vidpro1_process_manager::delete_last_process()
{
  process_queue.pop_back();
}


void
vidpro1_process_manager::clear_process_queue()
{
  process_queue.clear();
}


vcl_vector <vcl_string>
vidpro1_process_manager::get_process_queue_list() const
{
  vcl_vector <vcl_string> list;
  vcl_vector< bpro1_process_sptr >::const_iterator i = process_queue.begin();
  for (; i!= process_queue.end(); i++)
  {
    list.push_back( (*i)->name() );
  }

  return list;
}


void
vidpro1_process_manager::register_process( const bpro1_process_sptr& sptr )
{

  process_map.insert( vcl_pair< vcl_string , bpro1_process_sptr >( sptr->name() , sptr ) );

}


