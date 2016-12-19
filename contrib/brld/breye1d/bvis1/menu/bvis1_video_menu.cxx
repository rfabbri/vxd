
#include "bvis1_video_menu.h"
#include <bvis1/bvis1_manager.h>


bvis1_video_menu::bvis1_video_menu()
{
}

bvis1_video_menu::~bvis1_video_menu()
{
}

void
bvis1_video_menu::add_to_menu( vgui_menu & menu )
{
  add( "Play Video" , play_video );
  add( "Loop Play Video" , loop_play_video );

  add( "Stop Video" , stop_video );
  add( "Process & Play Video", process_and_play_video );
  separator();
  add( "Next Frame" , next_frame );
  add( "Prev Frame" , prev_frame );
  add( "First Frame" , first_frame );
  add( "Last Frame" , last_frame );
  add( "Go To Frame", goto_frame );
  separator();
  add( "Process Frame", process_frame );
  separator();
  add( "Reset Skip Frames", resetskipframes );
  menu.add( "Video" , (*this));
}

void
bvis1_video_menu::first_frame()
{
  bvis1_manager::instance()->first_frame();
}

//: Play the video
void
bvis1_video_menu::play_video()
{
  bvis1_manager::instance()->play_video();
}
//: Play the video in a Loop
void
bvis1_video_menu::loop_play_video()
{
  bvis1_manager::instance()->loop_play_video();
}

//: Stop Playing the video
void
bvis1_video_menu::stop_video()
{
  bvis1_manager::instance()->stop_video();
}

//: Processes the frames as it plays the video
void
bvis1_video_menu::process_and_play_video()
{
  bvis1_manager::instance()->process_and_play_video();
}

void
bvis1_video_menu::next_frame()
{
  bvis1_manager::instance()->next_frame();
}


void
bvis1_video_menu::prev_frame()
{
  bvis1_manager::instance()->prev_frame();
}

void
bvis1_video_menu::last_frame()
{
  bvis1_manager::instance()->last_frame();
}

void
bvis1_video_menu::goto_frame()
{
  bvis1_manager::instance()->goto_frame();
}

//: Apply the video processes in the queue to the current frame
void
bvis1_video_menu::process_frame()
{
  bvis1_manager::instance()->process_frame();
}

void
bvis1_video_menu::resetskipframes()
{
  bvis1_manager::instance()->resetskip();
}
