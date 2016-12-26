// This is basic/dbul/dbul_solar_position.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif
//:
// \file
// \author Matt Leotta (Brown)
// \date   October 28, 2005

#include "dbul_solar_position.h"
#include <vcl_cmath.h>


//:  Compute position of the sun given time, date, and location
void
dbul_solar_position(int day, double utc_hour, 
                    double lat, double lng,
                    double& alt, double& az)
{
  const double pi = 3.14159265358979323846;
  
  // Calculate solar declination as per Carruthers et al.
  double t = 2.0 * pi * ( (day-1) / 365.0);
  double declination = 0.322003 
                     - 22.9711  * vcl_cos(t) 
                     + 3.94638  * vcl_sin(t)
                     - 0.357898 * vcl_cos(2*t)
                     + 0.019334 * vcl_sin(2*t)
                     - 0.14398  * vcl_cos(3*t)
                     + 0.05928  * vcl_sin(3*t);
                     
  // convert to radians          
  declination *= pi/180.0;

  // Calculate the equation of time as per Carruthers et al.
  t = (279.134 + 0.985647 * day) * (pi/180.0);
  double equation = 5.0323
                  - 100.976 * vcl_sin(t)
                  + 595.275 * vcl_sin(2*t)
                  + 3.6858  * vcl_sin(3*t)
                  - 12.47   * vcl_sin(4*t)
                  - 430.847 * vcl_cos(t)
                  + 12.5024 * vcl_cos(2*t)
                  + 18.25   * vcl_cos(3*t);

  // Convert seconds to hours.
  equation = equation / 3600.00;

  // Calculate difference (in hours) from the prime meridian
  double difference = (lng * 4) / 60.0;
  
  // Caculate solar time.
  double solar_time = utc_hour + equation + difference;
  
  // Calculate hour angle.
  double hour_angle = (15 * (solar_time - 12)) * (pi/180.0);
  
  // convert latitude to radians
  lat *= pi/180.0;
  
  // Calculate altitude.
  t = (vcl_sin(declination) * vcl_sin(lat)) + (vcl_cos(declination) * vcl_cos(lat) * vcl_cos(hour_angle));
  alt = vcl_asin(t);

  // Calculate azimuth 
  t = (vcl_cos(lat) * vcl_sin(declination)) - (vcl_cos(declination) * vcl_sin(lat) * vcl_cos(hour_angle));
  az = vcl_acos(t / vcl_cos(alt));
  
  // vcl_acos produces results in (0,pi) but the correct solution lies 
  // in (0,2pi), and there are two solutions for vcl_acos.
  // before solar noon take the solutions from (0,pi]
  // after solar noon take the solution from (pi,2pi) 
  if(hour_angle > 0)
    az = 2*pi-az;
}
