// This is dbsol_geno_curve_2d.h
#ifndef dbsol_geno_curve_2d_h
#define dbsol_geno_curve_2d_h
//:
//\file
//\brief Curve that has been interpolated with GENO
//\author Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date 03/03/2005 09:45:01 AM EST
//

#include <dbsol/dbsol_interp_curve_2d.h>
#include <vcl_vector.h>



//: GENO stands for Geometric Non-Oscillatory interpolation. This class is just
// an interface for accessing GENO-interpolated curves.  Although it implements
// access to general interpolation, specialties of higher orders can be
// accommodated by subclassing from it and providing extra methods.
//
// \remarks After interpolation, this class is read-only for non-friends. 
// Do not reuse this class after first interpolation. If you want to run
// interpolation again, use another instance.
//
// \sa dbsol_geno : the actual interpolator class
//
class dbsol_geno_curve_2d : public dbsol_interp_curve_2d {
 public:
   dbsol_geno_curve_2d() : 
     dbsol_interp_curve_2d(), 
     order_(2) //:< just a default order; the interpolator algo may change this
      { }

   dbsol_geno_curve_2d(vcl_vector<dbgl_param_curve *> inter,bool closed) :
      dbsol_interp_curve_2d(inter),
      closed_(closed),order_(2) { }


   virtual void make(const vcl_vector<dbgl_param_curve *> &inter, bool closed) 
      { dbsol_interp_curve_2d::make(inter); closed_=closed;}

   //: algorithms
   friend class dbsol_geno;

   virtual void describe(vcl_ostream &strm, int blanking) const;

   //: stencil(i) is an integer that indicates the relative location of the
   // i-th stencil. leftmost is numbered 0; rightmost is n (order of
   // interpolation) minus 1.
   unsigned stencil(unsigned i) const { return stencil_[i]; }

   bool is_upper(unsigned i) const
     { return stencil_[i] == order()-1; }

   bool is_lower(unsigned i) const
     { return stencil_[i] == 0; }

   unsigned order() const {return order_;}
   bool closed() const {return closed_;}

   //: Returns the curvature at s arclength away from p0_
   double curvature_at(double s) const;

   //: Returns the curvature derivative at s arclength away from p0_
   double curvature_derivative_at(double s) const;

   //: circular indexing shortcut for closed curves
   unsigned next_circular(unsigned i) const 
      {return (i+1) % size();}

   //: circular indexing shortcut for closed curves
   unsigned prev_circular(unsigned i) const 
      {return (i + size() -1) % size();}

 protected:
   //: stencil_[i] is an integer that indicates the relative location of the
   // i-th stencil. leftmost is numbered 0; rightmost is n (order of
   // interpolation) minus 1.
   vcl_vector<unsigned> stencil_;

   //: simple utility to store whether a stencil is leftmost or rightmost
   void set_upper(unsigned i,bool up)
      { stencil_[i] = up? order()-1 : 0; }

   bool closed_; //:< indicates if the curve closed
   unsigned order_;  //:< order of interpolation; 2 for arc, 3 Euler spiral, etc.
};


#endif // dbsol_geno_curve_2d_h
