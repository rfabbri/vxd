#include <testlib/testlib_test.h>
#include <bnld/bnld_newton_root.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>

// equation: f(x) = (sin(a) - sin(a + x)) / x - b = 0
class f2 : public bnld_newton_root_func
{
public:
  f2(double a, double b): a_(a), b_(b){}
  virtual double f(double x){ 
    return (vcl_sin(a_)-vcl_sin(a_ + x))/x -b_;
  };
  virtual double df( double x){
    return -vcl_cos(x+a_)/x - (vcl_sin(a_)-vcl_sin(a_+x))/ x/x;
  };
private:
  double a_;
  double b_;
};


MAIN( test_newton_root )
{
   START ("newton_root");
   //2.513274123
   double x2 = vnl_math::pi;
   double x1 = vnl_math::pi_over_2;
   double xacc = 1e-8;
   
   f2 func2(vnl_math::pi_over_2 * 1.2, .756826729);
   double root;
   double succeed = bnld_newton_secant_root(func2, 
                            x1, x2, xacc, & root);
   TEST("solve f(x) = (sin(a) - sin(a+x))/x - b = 0", 
     vnl_math::abs(root - 2.513274123) < 1e-6 && succeed, true);
   vcl_cout << "a = " << vnl_math::pi_over_2 * 1.2 << vcl_endl;
   vcl_cout << "b = " << .756826729 << vcl_endl;
   vcl_cout << "root = " << root << vcl_endl;

   SUMMARY();
}
