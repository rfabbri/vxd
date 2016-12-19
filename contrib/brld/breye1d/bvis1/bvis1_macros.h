// This is breye1/bvis1/bvis1_macros.h
#ifndef bvis1_macros_h_
#define bvis1_macros_h_
//:
// \file
// \brief Macros for use in a main function.
// \author Matt Leotta, (mleotta@lems.brown.edu)
// \date 2/4/04
//
// See examples\bvis1_brown_eyes_main.cxx
//
// \verbatim
//  Modifications
// \endverbatim

#define REG_DISPLAYER(disp) \
  bvis1_manager::register_displayer( bvis1_displayer_sptr(new disp() ) )

#define REG_STORAGE(store) {\
  store ## _sptr model = store ## _new(); \
  model->register_binary_io(); \
  vidpro1_repository::register_type( model ); \
  }

#define REG_PROCESS(proc) \
  vidpro1_process_manager::register_process( bpro1_process_sptr( new proc() ) );



#endif // bvis1_macros_h_
