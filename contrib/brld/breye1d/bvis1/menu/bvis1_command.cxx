// This is breye1/bvis1/menu/bvis1_command.cxx
//:
// \file

#include "bvis1_command.h"
#include <vcl_vector.h>
#include <vcl_set.h>
#include <vcl_string.h>
#include <vcl_iostream.h>
#include <vcl_cstdio.h>
#include <vgui/vgui_dialog.h>
#include <bvis1/bvis1_manager.h>
#include <bvis1/bvis1_util.h>
#include <bpro1/bpro1_process.h>
#include <bpro1/bpro1_parameters_sptr.h>
#include <bpro1/bpro1_parameters.h>
#include <vidpro1/vidpro1_repository_sptr.h>
#include <vidpro1/vidpro1_repository.h>



//: execute the tool command to select and reset the tool
void
bvis1_tool_command::execute()
{
  bvis1_manager::instance()->set_active_tool(tool_);
} 


//: execute the execute the process
void
bvis1_process_command::execute()
{
  if(!process_.ptr()){
    vcl_cerr << "Warning: not able to execute a Null process" << vcl_endl;
    return;
  }
  
  vgui_dialog param_dialog( process_->name().c_str() );

  //We need to get the parameters for the process first because I/O types could change
  //with different parameters

  bpro1_parameters_sptr param_sptr = process_->parameters();

  vcl_vector< bpro1_param* > param_vector = param_sptr->get_param_list();

  for( vcl_vector< bpro1_param* >::iterator it = param_vector.begin();
       it != param_vector.end();  ++it ) {

    if( bpro1_param_type<int> * param = dynamic_cast<bpro1_param_type<int> *>(*it) ) {
      param_dialog.field( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_choice_param_type * param = dynamic_cast<bpro1_choice_param_type *>(*it) ) {
      param_dialog.choice( param->description().c_str() , param->choices(), param->temp_ref() );
    }
    else if( bpro1_param_type<unsigned int> * param = dynamic_cast<bpro1_param_type<unsigned int> *>(*it) ) {
      param_dialog.field( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_param_type<float> * param = dynamic_cast<bpro1_param_type<float> *>(*it) ) {
      param_dialog.field( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_param_type<double> * param = dynamic_cast<bpro1_param_type<double> *>(*it) ) {
      param_dialog.field( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_param_type<vcl_string> * param = dynamic_cast<bpro1_param_type<vcl_string> *>(*it) ) {
      param_dialog.field( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_param_type<bool> * param = dynamic_cast<bpro1_param_type<bool> *>(*it) ) {
      param_dialog.checkbox( param->description().c_str() , param->temp_ref() );
    }
    else if( bpro1_param_type<bpro1_filepath> * param = dynamic_cast<bpro1_param_type<bpro1_filepath> *>(*it) ) {
      param_dialog.file( param->description().c_str(), param->temp_ref().ext, param->temp_ref().path );
    }
    else{
      vcl_cerr << "No valid dialog interface for parameter: " << (*it)->name() << vcl_endl;
    }
  }

  if (!param_vector.empty()){
    if (param_dialog.ask()) {

      for( vcl_vector< bpro1_param* >::iterator it = param_vector.begin();
          it != param_vector.end();
          ++it )
      {
        (*it)->set_from_temp();

        if( bpro1_param_type<bpro1_filepath> * param = dynamic_cast<bpro1_param_type<bpro1_filepath> *>(*it) ) {
           bpro1_filepath path(param->value());
           bvis1_util::handle_file_name_prefix(path.path);
           param->set_value(path);
        }

      }
      // TESTING OUTPUT //////////////////////////////////////////////
      //((bpro1_process *)ptr)->get_parameters_sptr()->print_all();
      ////////////////////////////////////////////////////////////////
    }
    else  // The user has selected "cancel"
      return;
  }

  //get the repository
  vidpro1_repository_sptr repository_sptr = bvis1_manager::instance()->repository();

  //Prompt the user to select input/output variable
  vgui_dialog io_dialog("Select Inputs and outputs" );

  //display input options
  vcl_vector< vcl_string > input_type_list = process_->get_input_type();
  if (!input_type_list.empty())
    io_dialog.message("Select Input(s) From Available ones:");

  //store the choices
  vcl_vector<int> input_choices(input_type_list.size());
  vcl_vector< vcl_vector <vcl_string> > available_storage_classes(input_type_list.size());
  vcl_vector< vcl_string > input_names(input_type_list.size());
  
  for( unsigned int i = 0 ;
      i < input_type_list.size();
      i++ )
  {
    //for this input type allow user to select from available storage classes in the repository
    for (unsigned f=0; f < process_->input_frames(); ++f) {
      vcl_vector <vcl_string> cframe =  repository_sptr->get_all_storage_class_names(input_type_list[i], 
          repository_sptr->current_frame() - f);
      available_storage_classes[i].insert(available_storage_classes[i].end(), cframe.begin(), cframe.end());
    }
    available_storage_classes[i].push_back("(NONE)");

    //Multiple choice - with the list of options.
    io_dialog.choice(input_type_list[i].c_str(), available_storage_classes[i], input_choices[i]);
  }

  //display output options
  vcl_vector< vcl_string > output_type_list = process_->get_output_type();
  vcl_vector< vcl_string > output_suggested_names = process_->suggest_output_names();
  if (!output_type_list.empty())
    io_dialog.message("Select Output(s) From Available ones:");

  //store the choices
  vcl_vector<vcl_string> output_choices(output_type_list.size());

  //since output storage classes don't exist yet we can only assign names
  //make sure that these names are assigned to the storage classes after the process is executed
  //first build up a collection of all existing names
  vcl_map<vcl_string, vcl_set<vcl_string> > existing_names;
  for( unsigned int i = 0 ;
      i < output_type_list.size();
      i++ )
  {
    if(existing_names.find(output_type_list[i]) == existing_names.end())
    {
      vcl_vector<vcl_string> names = 
        repository_sptr->get_all_storage_class_names(output_type_list[i]);
      for( unsigned int j=0; j<names.size(); ++j )
        existing_names[output_type_list[i]].insert(names[j]);
    }
  }
   
  for( unsigned int i = 0 ;
      i < output_type_list.size();
      i++ )
  {
    //generate a default name that can be changed if desired
    vcl_string suggested_name = output_type_list[i];
    if(i < output_suggested_names.size())
      suggested_name = output_suggested_names[i];
    vcl_string name_str;
    int c = 0;
    do{
      vcl_stringstream name_stream;
      name_stream << suggested_name << c++;
      name_str = name_stream.str();
    }while(existing_names[output_type_list[i]].count(name_str) != 0);
 
    output_choices[i] = name_str;
    existing_names[output_type_list[i]].insert(name_str);
    io_dialog.field( output_type_list[i].c_str() , output_choices[i] );
  }

  //display dialog
  if (io_dialog.ask()) {
    vgui_dialog null_inputs("Name Missing Inputs");
    bool found_missing = false;
    for ( unsigned int i=0; i < input_choices.size(); i++){
      if( input_choices[i]+1 < (int)available_storage_classes[i].size() ){
        input_names[i] = available_storage_classes[i][input_choices[i]];
        null_inputs.message((input_type_list[i]+" : "+input_names[i]).c_str());
      }
      else{
        null_inputs.field(input_type_list[i].c_str(), input_names[i]);
        found_missing = true;
      }
    }
    if(found_missing)
      null_inputs.ask();
    
    process_->set_input_names(input_names);

    process_->set_output_names(output_choices);

    bpro1_process_sptr process_to_run = process_;
    //if we are in the middle of recording a macro
    //add this process to the process queue unless the queue is not allowed for this process
    if (bvis1_manager::instance()->recording_macro() && allow_queue_){
      process_to_run = process_->clone();
      bvis1_manager::instance()->process_manager()->add_process_to_queue(process_to_run);
    }
    //JLM modifciation
    if(!(bvis1_manager::instance()->recording_macro()) && allow_queue_)
      {
        bvis1_manager::instance()->process_manager()->clear_process_queue();
        bvis1_manager::instance()->process_manager()->
          add_process_to_queue(process_to_run);
      }

    vcl_set<bpro1_storage_sptr> modified;
    //now run the process
    bvis1_manager::instance()->process_manager()->run_process_on_current_frame(process_to_run, &modified);

    // update the display for any modified storage objects
    for ( vcl_set<bpro1_storage_sptr>::iterator itr = modified.begin();
          itr != modified.end(); ++itr ) {
      bvis1_manager::instance()->add_to_display(*itr);
    }

    bvis1_manager::instance()->display_current_frame();
  }
}

