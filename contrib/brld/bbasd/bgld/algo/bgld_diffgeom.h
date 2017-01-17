// This is bgld_diffgeom.h
#ifndef bgld_diffgeom_h_
#define bgld_diffgeom_h_
//:
//\file
//\brief Differential geometry estimation for curves
//\author Based on original code by  Ricardo Fabbri (rfabbri), Brown University  (rfabbri.github.io)
//\date Wed May 25 15:53:23 BRT 2016
//

#include <vcl_vector.h>
#include <vgl/vgl_point_2d.h>
#include <vnl/vnl_vector_fixed.h>

// Curvature estimation by D. Kroons's approach
//
// Original code is Copyright (c) 2011, Dirk-Jan Kroon
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in
//       the documentation and/or other materials provided with the distribution
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
void 
bgld_compute_curvature(
    const vcl_vector< vgl_point_2d<double> > &vertices, 
    vnl_vector<double> *k
    );

// This function calculates the normals, of the line points
// using the neighbouring points of each contour point, and 
// forward an backward differences on the end points
//
// Originally written by  D.Kroon University of Twente (August 2011)
//
void 
bgld_compute_normals(
    const vcl_vector< vgl_point_2d<double> > &vertices, 
    vcl_vector< vnl_vector_fixed<double, 2> > *n
    );

#endif // bgld_diffgeom_h

