// This is breye1/bvis1/bvis1_manager.cxx
//
// \file

#include <bvis1/bvis1_manager.h>
#include <bvis1/bvis1_displayer.h>
#include <bvis1/bvis1_view_tableau.h>
#include <bpro1/bpro1_storage.h>
#include <vidpro1/vidpro1_process_manager.h>
#include <vidpro1/vidpro1_repository.h>
#include <vidpro1/vidpro1_repository_sptr.h>

#include <vgui/vgui.h>
#include <vgui/vgui_dialog.h>
#include <vgui/vgui_shell_tableau.h>
#include <vgui/vgui_viewer2D_tableau.h>
#include <vgui/vgui_displaybase_tableau.h>

#include <vbl/io/vbl_io_smart_ptr.h>
#include <vcl_sstream.h>

#include <bvis1/bvis1_util.h>


//static manager instance
bvis1_manager* bvis1_manager::instance_ = 0;
vcl_map< vcl_string, bvis1_displayer_sptr > bvis1_manager::displayers_;

//: Constructor
bvis1_manager::bvis1_manager()
 : vgui_wrapper_tableau(),
   recording_macro_bool_(false),
   is_playing_(false),
   is_loop_playing_(false),
   repository_(NULL),
   active_tool_(NULL),
   skip_frames_(1),
   cache_tableau_(false)
{
}

//: Destructor
bvis1_manager::~bvis1_manager()
{
}

//: Insure only one instance is created
bvis1_manager *bvis1_manager::instance()
{
  if (!instance_){
    instance_ = new bvis1_manager();
    instance_->initialize();
  }
  return bvis1_manager::instance_;
}


//: Initialize the instance
void
bvis1_manager::initialize()
{
  grid_tab_ = vgui_grid_tableau_new(1,1);
  grid_tab_->set_frames_selectable(false);
  this->add_new_view(0);
  vgui_shell_tableau_sptr shell = vgui_shell_tableau_new(grid_tab_);
  this->add_child(shell);

  repository_ = process_manager_.get_repository_sptr();
  repository_->initialize(1);
}


//: Handle events
bool 
bvis1_manager::handle( const vgui_event & e )
{
  if(e.type == vgui_KEY_PRESS && e.modifier == vgui_MODIFIER_NULL){
    switch(e.key){
    case vgui_CURSOR_RIGHT:
      this->next_frame();
      break;
    case vgui_CURSOR_LEFT:
      this->prev_frame();
      break;
    case vgui_CURSOR_UP:
      this->first_frame();
      break;
    case vgui_CURSOR_DOWN:
      this->last_frame();
      break;
    default:
      break;
    }
  }
  return vgui_wrapper_tableau::handle(e);
}


//: Add a new view of the data
void
bvis1_manager::add_new_view( int offset, bool absolute )
{
  vgui_selector_tableau_sptr selector_tab = vgui_selector_tableau_new();
  bvis1_view_tableau_sptr view_tab = bvis1_view_tableau_new(selector_tab, offset, absolute);
  view_tabs_.push_back( view_tab );
  grid_tab_->add_next(vgui_viewer2D_tableau_new(view_tab));
}


//: Create one view for each visible layer of the current frame
void
bvis1_manager::layer_per_view()
{
  vgui_selector_tableau_sptr selector_tab = active_selector();
  const vcl_vector< vcl_string >& names = selector_tab->child_names();
  vcl_vector<vgui_selector_tableau_sptr> new_selectors;
  for(unsigned int i=0; i<names.size(); ++i){
    if(selector_tab->is_visible(names[i])){
      vgui_selector_tableau_sptr new_selector_tab = vgui_selector_tableau_new();
      for(unsigned int j=0; j<names.size(); ++j){
        new_selector_tab->add(selector_tab->get_tableau(names[j]), names[j]);
        if(i!=j){
          new_selector_tab->toggle(names[j]);
        }
      }
      new_selector_tab->set_active(names[i]);
      new_selectors.push_back(new_selector_tab);
    }
  }
 
  unsigned int num = new_selectors.size();
  unsigned int width = 0;
  while(width*width < num) ++width;
  unsigned int height = width;
  while((height-1)*width >= num) --height;
  while(grid_tab_->cols() < width) grid_tab_->add_column();
  while(grid_tab_->cols() > width) grid_tab_->remove_column();
  while(grid_tab_->rows() < height) grid_tab_->add_row();
  while(grid_tab_->rows() > height) grid_tab_->remove_row(); 
  view_tabs_.clear();
  grid_tab_ = vgui_grid_tableau_new(width,height);
  vgui_shell_tableau_sptr shell = vgui_shell_tableau_new(grid_tab_);
  this->child.assign(shell);    
  
  for(unsigned int i=0; i<new_selectors.size(); ++i){
    bvis1_view_tableau_sptr view_tab = bvis1_view_tableau_new(new_selectors[i], 0, false);
    view_tabs_.push_back(view_tab);
    grid_tab_->add_next(vgui_viewer2D_tableau_new(view_tab)); 
  }

}


//: Remove the view in the active part of the grid
void
bvis1_manager::remove_active_view()
{
  unsigned c,r;
  grid_tab_->get_last_selected_position(&c,&r);

  vgui_tableau_sptr ptab = grid_tab_->get_tableau_at(c,r);
   
  if (ptab != 0) {
     vgui_viewer2D_tableau_sptr rm_viewer;
     rm_viewer.vertical_cast( ptab );

     bvis1_view_tableau_sptr rm_view;
     rm_view.vertical_cast( rm_viewer->child.child() );

     for( vcl_vector<bvis1_view_tableau_sptr>::iterator v_itr = view_tabs_.begin();
          v_itr != view_tabs_.end(); ++v_itr){
       if( *v_itr == rm_view ){
         view_tabs_.erase(v_itr);
         grid_tab_->remove_at(c,r);
         break;
       }
     }
     grid_tab_->layout_grid2();
  }
}

//: Display info about the active tableau
void 
bvis1_manager::get_info_on_active_tableau()
{
  vgui_displaybase_tableau_sptr cur_active_tableau;
  cur_active_tableau.vertical_cast (active_tableau());

  if(!cur_active_tableau.ptr()) return;

  vcl_cout << vcl_endl << "***** Active Tableau Info: **********" << vcl_endl;
  vcl_cout << "TYPE: " << cur_active_tableau->type_name() << vcl_endl;

  //temp hack
  if (cur_active_tableau->type_name()!="bgui_vsol2D_tableau")
    return;

  vcl_vector< vcl_string > all_groups = cur_active_tableau->get_grouping_names();
  vcl_cout << "Groups: ";
  for (unsigned int i=0; i<all_groups.size(); i++)
    vcl_cout << all_groups[i] << ", ";

  vcl_cout << vcl_endl;
  vcl_cout << "*************************************" << vcl_endl;
}

//: Regenerate all tableau from the repository
void 
bvis1_manager::regenerate_all_tableaux()
{
  tableau_map_.clear();

  if(!cache_tableau_)
    return;

  int curr_frame = repository_->current_frame();
  repository_->go_to_frame(0);
  // for each frame
  do{
    // for each type
    for ( vidpro1_repository::storage_map::iterator t_itr = repository_->map_begin();
          t_itr != repository_->map_end();  ++t_itr ){
      // for each storage object
      for ( vcl_vector< bpro1_storage_sptr >::iterator s_itr = t_itr->second.begin();
            s_itr != t_itr->second.end(); ++s_itr ){
        add_to_display(*s_itr);
      }
    }
  }while( repository_->go_to_next_frame() );

  // for the global data
  // for each type
  for ( vidpro1_repository::storage_map::iterator t_itr = repository_->global_map_begin();
        t_itr != repository_->global_map_end();  ++t_itr ){
    // for each storage object
    for ( vcl_vector< bpro1_storage_sptr >::iterator s_itr = t_itr->second.begin();
          s_itr != t_itr->second.end(); ++s_itr ){
      add_to_display(*s_itr);
    }
  }

  repository_->go_to_frame(curr_frame);
}


//: Create a new empty storage class
void 
bvis1_manager::make_empty_storage()
{
  vgui_dialog make_storage_dlg("Make an empty storage class");
  vcl_vector<vcl_string> types;
  vcl_set<vcl_string> typeset = repository_->types();
  for ( vcl_set<vcl_string>::iterator itr = typeset.begin();
        itr != typeset.end();  ++itr )
    types.push_back(*itr);
  static unsigned int choice = 0;
  make_storage_dlg.choice("Type",types,choice);
  vcl_string name;
  make_storage_dlg.field("Name",name);
  bool all_frames = false;
  bool global = false;
  make_storage_dlg.checkbox("Create at all frames",all_frames);
  make_storage_dlg.checkbox("Create global storage",global);
  if( !make_storage_dlg.ask())
    return;

  if(global){
    bpro1_storage_sptr new_data = repository_->new_data_at(types[choice],name,-1); 
    if(new_data){
      regenerate_all_tableaux();
      display_current_frame();
    }
  }
  else if(all_frames){
    for(int i=0; i<repository_->num_frames(); ++i)
      repository_->new_data_at(types[choice],name,i); 
    regenerate_all_tableaux();
    display_current_frame();
  }
  else{
    bpro1_storage_sptr new_data = repository_->new_data(types[choice],name);
    if(new_data){
      add_to_display(new_data);
      display_current_frame();
    }
  }
}

  
//: Registers a storage type with the repository
//  Call this function with a null smart pointer of each type you plan to use
void
bvis1_manager::register_displayer(bvis1_displayer_sptr displayer)
{
  // map displayers by type name
  displayers_[displayer->type()] = displayer;
}


//: Access the process_manager
vidpro1_process_manager *
bvis1_manager::process_manager()
{
  return &process_manager_;
}


//: Access the process manager
vidpro1_repository_sptr
bvis1_manager::repository()
{
  return repository_;
}


void 
bvis1_manager::load_repository()
{
  vgui_dialog load_rep_dlg("Load the repository from a file");
  static vcl_string file_name = "";
  static vcl_string ext = "*.rep";
  load_rep_dlg.file("File:", ext, file_name);
  if( !load_rep_dlg.ask())
    return;

  load_repository_from_file(file_name);
}

void 
bvis1_manager::load_repository_from_file(vcl_string file_name)
{
  bvis1_util::handle_file_name_prefix(file_name);

  vsl_b_ifstream bfs(file_name);
  vidpro1_repository_sptr new_rep;
  vsl_b_read(bfs, new_rep);
  bfs.close();

  active_tool_ = NULL;

  repository_->replace_data(new_rep);

  regenerate_all_tableaux();
  process_manager_.set_repository(repository_);
  display_current_frame(true);
}

void bvis1_manager::add_to_repository()
{
  vgui_dialog add_to_rep_dlg("Add_To the repository from a file");
  static vcl_string file_name = "";
  static vcl_string ext = "*.rep";
  add_to_rep_dlg.file("File:", ext, file_name);
  if( !add_to_rep_dlg.ask())
    return;

  vsl_b_ifstream bfs(file_name);
  vidpro1_repository_sptr new_rep;
  vsl_b_read(bfs, new_rep);
  bfs.close();
  repository_->add_repository(new_rep);
  regenerate_all_tableaux();
  display_current_frame(true);
}

//: Save the repository from a binary data file
void 
bvis1_manager::save_repository()
{
  vgui_dialog save_rep_dlg("Save the repository to a file");
  static vcl_string file_name = "";
  static vcl_string ext = "*.rep";
  static bool save_visible = false;
  save_rep_dlg.file("File:", ext, file_name);
  save_rep_dlg.checkbox("Save Visible Only",save_visible);
  if( !save_rep_dlg.ask())
    return;

  vsl_b_ofstream bfs(file_name);
  if(save_visible){
    vidpro1_repository_sptr new_rep = new vidpro1_repository(*(this->repository_));
    vcl_set<vcl_string> names = this->visible_storage();
    new_rep->remove_all_except(names);
    vsl_b_write(bfs, new_rep);
  }
  else
    vsl_b_write(bfs, this->repository_);
  bfs.close();
}

//: Display the structure of the repository
void 
bvis1_manager::view_repository()
{
  repository()->print_summary();
}

//: Save OpenGL render areas as a movie
void 
bvis1_manager::save_view_as_movie() const
{
  vcl_cerr << "FIXME not supported (rfabbri)\n";
#if 0
  vgui_dialog save_rep_dlg("Save Views as Movie");
  int start = 1;
  int end = bvis1_manager::instance()->repository()->num_frames();
  vcl_string file_name = "";
  vcl_string ext = "*.avi";
  int id_codec = 0;

  save_rep_dlg.file("File Name, Prefix or Directory:", ext, file_name);
  save_rep_dlg.field("Start Frame:",start);
  save_rep_dlg.field("End Frame:",end);

  vcl_list<vcl_string> supported_codecs_lst = vidl1_io::supported_types();

  if (supported_codecs_lst.empty()) {
    vcl_cerr << "Error: no codecs compiled into vidl1\n";
    return;
  }

  vcl_vector<vcl_string> supported_codecs(
      supported_codecs_lst.begin(), supported_codecs_lst.end());

  save_rep_dlg.choice("Output Codec", supported_codecs, id_codec);
  if(!save_rep_dlg.ask())
    return;

  vidl1_movie_sptr movie = new vidl1_movie(new vidl1_clip(new bvis1_gl_codec(start,end)));
  vidl1_io::save(movie, file_name.c_str(), supported_codecs[id_codec].c_str());
#endif
}

//: Access the selector tableau in the active view
vgui_selector_tableau_sptr 
bvis1_manager::active_selector()
{
  unsigned c,r;
  grid_tab_->get_active_position(&c,&r);
  vgui_viewer2D_tableau_sptr active_viewer;
  active_viewer.vertical_cast( grid_tab_->get_tableau_at(c,r) );
  if(!active_viewer) return NULL;

  bvis1_view_tableau_sptr active_view;
  active_view.vertical_cast( active_viewer->child.child() );
  if(!active_view) return NULL;

  vgui_selector_tableau_sptr active_selector = active_view->selector();
  if(!active_selector) return NULL;

  return active_selector;
}

//: Access the active tableau at the current frame
vgui_tableau_sptr
bvis1_manager::active_tableau()
{
  vgui_selector_tableau_sptr active_selector = this->active_selector();
  if( !active_selector )
    return NULL;
  
  return active_selector->active_tableau();
}

//: Find the names of the visible data in the active view
vcl_set<vcl_string> 
bvis1_manager::visible_storage()
{
  vcl_set<vcl_string> vis_names;
  vgui_selector_tableau_sptr active_selector = this->active_selector();
  if( !active_selector )
    return vis_names;

  vcl_vector<vcl_string> all_names = active_selector->child_names();
  for( vcl_vector<vcl_string>::iterator itr = all_names.begin();
       itr != all_names.end(); ++itr )
  {
    if(active_selector->is_visible(*itr))
      vis_names.insert(*itr);
  }

  return vis_names;
}


//: Look up the storage class that created the given tableau
bpro1_storage_sptr
bvis1_manager::storage_from_tableau(const vgui_tableau_sptr& tab)
{
  vcl_map< bpro1_storage*, vgui_tableau_sptr >::iterator itr;
  for( itr=tableau_map_.begin(); itr != tableau_map_.end(); ++itr ){
    if (itr->second == tab) break;
  }
  if( itr != tableau_map_.end() )
    return bpro1_storage_sptr(itr->first);
  return NULL;
}
  

//: Set the current active tool
//  This function is reponsible to make sure that the tool that
//  was just activated has the right tableau and storage class
//  assigned to it
void 
bvis1_manager::set_active_tool(bvis1_tool_sptr tool)
{
  if( active_tool_ )
    active_tool_->deactivate();

  //find out what data type the tool needs

  //find out the currently active tableau as reported by the grid tableau 
  //and correponding the selector tableau
  vgui_tableau_sptr active_tab = active_tableau();

  //if the currently active tableau doesn't agree with the tool
  //check to see if there exists a tableau that is compatible

  //if there are multiple instances of agreeable tableaux
  //put up a dialog box to force the user to choose the right
  //tableau

  //if there are no instances of the required data types
  //create empty data storage classes and tableaux of the
  //required type(s)

  //Everything's ok: assign this as the active tool
  active_tool_ = tool;

  ////need to check if everything went smoothly here
  //active_tool_->set_tableau( active_tab );

  if( active_tool_ )
  {
    //need to check if everything went smoothly here
    active_tool_->set_tableau( active_tab );
    active_tool_->activate();
  }
}

//: Return the active tool
bvis1_tool_sptr 
bvis1_manager::active_tool() const
{
  return active_tool_;
}


bool
bvis1_manager::recording_macro()
{
  return recording_macro_bool_;
}

void
bvis1_manager::start_recording_macro()
{
  vcl_cout << "Recording Process Macro Started..." << vcl_endl;
  recording_macro_bool_ = true;
}

void
bvis1_manager::stop_recording_macro()
{
  vcl_cout << "Stopped Recording Process Macro." << vcl_endl;
  recording_macro_bool_ = false;
}

//: Play the video sequence
void
bvis1_manager::play_video()
{
  if( repository_->current_frame() == repository_->num_frames()-1 )
    this->first_frame();
  is_playing_ = true;
  while( repository_->go_to_next_n_frame(skip_frames_) && is_playing_ )
    display_current_frame();
  is_playing_ = false;
}

//: Play the video sequence
void
bvis1_manager::loop_play_video()
{
  if( repository_->current_frame() == repository_->num_frames()-1 )
        this->first_frame();
  is_loop_playing_ = true;
  
  while(is_loop_playing_ && repository_->go_to_next_n_frame(skip_frames_))
  {
    display_current_frame();
    if( repository_->current_frame() == repository_->num_frames()-1 )
        this->first_frame();
  }
  is_loop_playing_ = false;
}

//: Stop the video if it is playing
void 
bvis1_manager::stop_video()
{
  is_playing_ = false;
  is_loop_playing_ =false;
}


//: Process each frame in the video and display the results
void
bvis1_manager::process_and_play_video()
{
  vgui_dialog frames_dlg("Process and Play Video");
  int first_frame = repository_->current_frame();
  vcl_stringstream prompt;
  prompt << "Process from frame "<<first_frame+1<<" to ";
  int last_frame = repository_->num_frames();
  frames_dlg.field(prompt.str().c_str(), last_frame);
  frames_dlg.field("No of Frames to Skip", skip_frames_);
  if(!frames_dlg.ask())
    return;
  last_frame--;
 
  is_playing_ = true;
  while( is_playing_ &&
         repository_->current_frame() < last_frame &&
         repository_->go_to_next_n_frame(skip_frames_) ){
    
    vcl_set<bpro1_storage_sptr> modified; 
    process_manager_.run_process_queue_on_current_frame(&modified);
    for ( vcl_set<bpro1_storage_sptr>::iterator itr = modified.begin();
          itr != modified.end(); ++itr ) {
      this->add_to_display(*itr);

      
    }
    display_current_frame();
    }
  if(last_frame < repository_->current_frame())
  {
    is_playing_ = false;
    return;
  }

  vcl_cout << "finish frames " << first_frame+1 << " to " << last_frame+1 << vcl_endl;
  // finish the process queue
  vcl_set<bpro1_storage_sptr> modified; 
  process_manager_.finish_process_queue(first_frame,last_frame,&modified);
  for ( vcl_set<bpro1_storage_sptr>::iterator itr = modified.begin();
        itr != modified.end(); ++itr ) {
    this->add_to_display(*itr);
  }
  display_current_frame();
 
  is_playing_ = false;
}

//: Go to the next frame
void
bvis1_manager::next_frame()
{
  if( repository_->go_to_next_n_frame(skip_frames_) )
    display_current_frame();
}

//: Go to the previous frame
void
bvis1_manager::prev_frame()
{
  if( repository_->go_to_prev_n_frame(skip_frames_) )
    display_current_frame();
}

//: Jump to the first frame
void
bvis1_manager::first_frame()
{
  if( repository_->go_to_frame( 0 ) )
    display_current_frame();
}

//: Jump to the last frame
void
bvis1_manager::last_frame()
{
  if( repository_->go_to_frame( repository_->num_frames() - 1 ) )
    display_current_frame();
}

//: Go to the specified frame
void bvis1_manager::goto_frame()
{
  static unsigned frame = 0;
  vgui_dialog frame_dl("GoTo Frame");
  frame_dl.field("Frame Index",frame);
  if (!frame_dl.ask())
    return;
  if(repository_->go_to_frame( frame ))
    display_current_frame();
}

//: process the current frame only
void
bvis1_manager::process_frame()
{
  vcl_set<bpro1_storage_sptr> modified; 
  process_manager_.run_process_queue_on_current_frame(&modified);
  for ( vcl_set<bpro1_storage_sptr>::iterator itr = modified.begin();
        itr != modified.end(); ++itr ) {
    this->add_to_display(*itr);
  }
  display_current_frame();
}

//: Quit the application
void
bvis1_manager::quit()
{
  vgui_dialog quit_dl("Quit");
  quit_dl.message(" ");
  quit_dl.message("   Are you sure you want to quit bvis?   ");
  quit_dl.message(" ");

  if (quit_dl.ask())
    vgui::quit();
}

//: Return the current frame number
int 
bvis1_manager::current_frame()
{
  return repository_->current_frame();
}


//: Add a new frame
void
bvis1_manager::add_new_frame()
{
  if( repository_->num_frames() == 0 ) {
    repository_->initialize( 1 );
  }
  else {
    repository_->add_new_frame();
    last_frame();
  }

  display_current_frame();
}


//: Adjust the tableau smart pointers for the current frame and refresh the display
void
bvis1_manager::display_current_frame(bool clear_old) 
{
  if(!cache_tableau_){
    tableau_map_.clear();
  }

  // update each of the views
  for( vcl_vector<bvis1_view_tableau_sptr>::iterator v_itr = view_tabs_.begin();
       v_itr != view_tabs_.end(); ++v_itr ){

    // determine which frame this view should use
    int curr_frame = (*v_itr)->frame();

    if(!repository_->valid_frame(curr_frame)){
      if(clear_old)
        (*v_itr)->selector()->clear();
      continue;
    }

    // all storage classes at the current frame
    vcl_set<bpro1_storage_sptr> storage_set = repository()->get_all_storage_classes(curr_frame);
    // all names of tableau attached to the current view
    const vcl_vector<vcl_string>& all_names = (*v_itr)->selector()->child_names();

    // Check each tableau currently drawn in this view.  For each, do one of the following
    // - remove it if there is no related storage class
    // - replace it with a cached tableau
    // - update it with the data of the storage class
    for ( vcl_vector<vcl_string>::const_iterator itr = all_names.begin();
          itr != all_names.end(); ++itr ){
      vgui_tableau_sptr tab = (*v_itr)->selector()->get_tableau(*itr);
      bpro1_storage_sptr stg = repository()->get_data_by_name_at(*itr,curr_frame);
      if(!stg){
        if(clear_old)
          (*v_itr)->selector()->remove(*itr--);
        else
          (*v_itr)->selector()->add(NULL, *itr);
        continue;
      }
      // check the cache
      if(cache_tableau_){
        typedef vcl_map< bpro1_storage*, vgui_tableau_sptr >::iterator m_itr_t;
        m_itr_t search_itr = tableau_map_.find(stg.ptr());
        if(search_itr != tableau_map_.end()){
          storage_set.erase(stg);
          (*v_itr)->selector()->add(search_itr->second, *itr);
          continue;
        }
      }
      else{
        // try to update an existing tableau;
        if(update_tableau(tab,stg)){
          storage_set.erase(stg);
          tableau_map_[stg.ptr()] = tab;
        }
      }
    }
    // create any missing tableau from the storage
    for(vcl_set<bpro1_storage_sptr>::iterator s_itr = storage_set.begin();
        s_itr != storage_set.end();  ++s_itr)
    {
      vcl_string name = (*s_itr)->name();

      // only create a new tableau if it hasn't already been created
      typedef vcl_map< bpro1_storage*, vgui_tableau_sptr >::iterator m_itr_t;
      m_itr_t search_itr = tableau_map_.find(s_itr->ptr());
      if(search_itr != tableau_map_.end()){
        (*v_itr)->selector()->add(search_itr->second, name);
      }
      else{
        bool cacheable = false;
        vgui_tableau_sptr tab = this->make_tableau( *s_itr, cacheable );
        if (tab){
          if(cacheable)
            tableau_map_[s_itr->ptr()] = tab;
          (*v_itr)->selector()->add(tab, name);
        }
      }
    }
  }


  // display the frame number in the status bar
  vgui::out << "frame " << this->current_frame()+1 << " of " << repository_->num_frames()<< '\n';

  // redraw the display
  this->post_redraw();
  vgui::run_till_idle();
}


//: Toggle caching of tableaux
void
bvis1_manager::toggle_tableau_cache()
{
  cache_tableau_ = !cache_tableau_;
  tableau_map_.clear();
  vgui::out << "Tableau Caching ";
  if(cache_tableau_)
    vgui::out << "Enabled\n";
  else
    vgui::out << "Disabled\n";
}


//: Calls the appropriate type of displayer to make a tableau
vgui_tableau_sptr
bvis1_manager::make_tableau(const bpro1_storage_sptr& storage, bool& cacheable ) const
{
  vcl_map< vcl_string , bvis1_displayer_sptr >::const_iterator itr;
  itr = displayers_.find(storage->type());
  if( itr != displayers_.end() ){
    cacheable = itr->second->cacheable();
    return itr->second->make_tableau( storage );
  }
  cacheable = false;
  return NULL;
}


//: Calls the appropriate type of displayer to update a tableau
bool
bvis1_manager::update_tableau(const vgui_tableau_sptr& tableau, 
                             const bpro1_storage_sptr& storage ) const
{
  vcl_map< vcl_string , bvis1_displayer_sptr >::const_iterator itr;
  itr = displayers_.find(storage->type());
  if( itr != displayers_.end() )
    return itr->second->update_tableau( tableau, storage );
  return false;
}


//: Create a tableau for storage class and cache it
bool
bvis1_manager::add_to_display(const bpro1_storage_sptr& storage)
{
  if(!storage.ptr() || storage->frame() < -1)
    return false;

  if(cache_tableau_){
    return true;
  }

  vcl_string name = storage->name();

  bool cacheable = false;
  vgui_tableau_sptr new_tab = this->make_tableau( storage, cacheable );
  if (!new_tab.ptr())
    return false;

  if(cacheable)
    tableau_map_[storage.ptr()] = new_tab;
  return true; 
}

bool
bvis1_manager::resetskip()
{
    skip_frames_=1;
    return true;
}

bvis1_displayer_sptr bvis1_manager::displayer(  vcl_string const& type )
{
  return displayers_[type];
}
