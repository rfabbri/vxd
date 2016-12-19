// This is breye1/bvis1/bvis1_manager.h
#ifndef bvis1_manager_h_
#define bvis1_manager_h_

//:
// \file
// \brief This file defines a class that acts as a gui manager.
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 10/30/03
//
// \verbatim
//  Modifications
//  \author Ozge Can Ozcanli (ozge@lems.brown.edu) added get_views() method
//  \date 06/27/05 
// \endverbatim

#include <vcl_vector.h>
#include <vcl_map.h>
#include <vcl_set.h>
#include <vcl_string.h>
#include <vbl/vbl_ref_count.h>

#include <vgui/vgui_tableau_sptr.h>
#include <vgui/vgui_wrapper_tableau.h>
#include <vgui/vgui_grid_tableau.h>

#include <vgui/vgui_selector_tableau.h>

#include <bvis1/bvis1_displayer_sptr.h>
#include <bpro1/bpro1_storage_sptr.h>
#include <bpro1/bpro1_storage.h>
#include <vidpro1/vidpro1_repository_sptr.h>
#include <vidpro1/vidpro1_process_manager.h>

#include <bvis1/bvis1_view_tableau_sptr.h>
#include <bvis1/bvis1_tool_sptr.h>
#include <bvis1/bvis1_tool.h>


class bvis1_manager : public vgui_wrapper_tableau 
{

public:

  //: Destructor
  ~bvis1_manager();
  
  //: Access the static instance of the tableau manager
  static bvis1_manager * instance();

  //: Handle events
  bool handle( const vgui_event & e );

  //: Access the process manager
  vidpro1_process_manager * process_manager();
  
  //: Registers a displayer type
  //  Call this function with an instance of each type you plan to use
  static void register_displayer(bvis1_displayer_sptr displayer);

  //: Access the process manager
  vidpro1_repository_sptr repository();

  //: Access the selector tableau in the active view
  vgui_selector_tableau_sptr active_selector();

  //: Access the active tableau at the current frame
  vgui_tableau_sptr active_tableau();

  //: Find the names of the visible data in the active view
  vcl_set<vcl_string> visible_storage();

  //: Look up the storage class that created the given tableau
  bpro1_storage_sptr storage_from_tableau(const vgui_tableau_sptr& tab);

  //: Create a tableau for storage class and cache it
  bool add_to_display(const bpro1_storage_sptr& storage);

  //: Regenerate all tableau from the repository
  void regenerate_all_tableaux();

  //: Create a new empty storage class
  void make_empty_storage();

  //: Add a new view of the data
  void add_new_view( int offset = 0, bool absolute = false );

  //: Remove the view in the active part of the grid
  void remove_active_view();
  
  //: Create one view for each visible layer of the current frame
  void layer_per_view();

  //: Display info about the active tableau
  void get_info_on_active_tableau();

  //-------------------------------------------------------------

  //: Load the repository from a binary data file
  void load_repository_from_file(vcl_string file_name);

  //: Load the repository from a binary data file using a dialog
  void load_repository();

  //: Save the repository from a binary data file
  void save_repository();

  //: Add additional storage items to the repository from a binary data file
  void add_to_repository();

  //: Display the structure of the repository
  void view_repository();

  //: Save OpenGL render areas as a movie
  void save_view_as_movie() const;

  //-------------------------------------------------------------

  //: Set the current active tool
  void set_active_tool(bvis1_tool_sptr tool);

  //: Return the active tool
  bvis1_tool_sptr active_tool() const;


  //-------------------------------------------------------------

  //: Play the video sequence
  void play_video();
  //: Loop and Play the video sequence
  void loop_play_video();

  //: Stop the video if it is playing
  void stop_video();

  //: Process each frame in the video and display the results
  void process_and_play_video();

  //: Go to the next frame
  void next_frame();

  //: Go to the previous frame
  void prev_frame();

  //: Jump to the first frame
  void first_frame();

  //: Jump to the last frame
  void last_frame();

  //: Go to the specified frame
  void goto_frame();

  //: process the current frame only
  void process_frame();

  //--------------------------------------------------------------

  //: Add a new frame
  void add_new_frame();

  //: Return the current frame number
  int current_frame();
  
  //: quit the application
  void quit();

  //--------------------------------------------------------------

  //: Macro functions
  bool recording_macro();
  void start_recording_macro();
  void stop_recording_macro();

  //--------------------------------------------------------------

  //: Adjust the tableau smart pointers for the current frame and refresh the display
  void display_current_frame(bool clear_old = false);

  //: Toggle caching of tableaux
  void toggle_tableau_cache();

  //: Calls the appropriate type of displayer to make a tableau
  //: \param cacheable returns true by reference if we are allowed to cache this tableau
  vgui_tableau_sptr make_tableau(const bpro1_storage_sptr& storage, bool& cacheable) const;

  //: Calls the appropriate type of displayer to update a tableau
  bool update_tableau(const vgui_tableau_sptr& tableau, 
                      const bpro1_storage_sptr& storage ) const;

  //:return the view vector
  vcl_vector<bvis1_view_tableau_sptr> get_views() {return view_tabs_;}

  // reset the skipping frame
  bool resetskip();

  // get a displayer by type name e.g. vsol2D
  bvis1_displayer_sptr displayer(  vcl_string const& type );

protected:
  //: Constructor
  bvis1_manager();

private:

  //: Initialize the static instance
  void initialize();

  //: A static instance of the tableau manager
  static bvis1_manager * instance_;

  //: Is the manager recording processes
  bool recording_macro_bool_;

  //: Is the manager playing the video
  bool is_playing_;

  //: Is the manager playing the video in a loop
  bool is_loop_playing_;

  //: The process_manager
  vidpro1_process_manager process_manager_;

  //: A smart pointer to the repository
  vidpro1_repository_sptr repository_;

  //: The grid tableau at the root
  vgui_grid_tableau_sptr grid_tab_;

  //: The view tableaux that provided a view of the data
  vcl_vector<bvis1_view_tableau_sptr> view_tabs_;

  //: A map from storage object pointers to tableaux
  vcl_map< bpro1_storage*, vgui_tableau_sptr > tableau_map_;

  //: Vector of maps from type to displayer
  static vcl_map< vcl_string, bvis1_displayer_sptr > displayers_;

  //: Currently active tool
  bvis1_tool_sptr active_tool_;
  //: varibale to skip_frames when playing
  int skip_frames_;

  //: This flag indicates whether or not to cache tableau 
  bool cache_tableau_;

};

#endif // bvis1_manager_h_

