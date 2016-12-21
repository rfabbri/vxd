// This is basic/dbgl/dbgl_barycentric.cxx
//:
//\file

#include <vgl/vgl_distance.h>
#include <dbgl/dbgl_triangle.h>
#include <dbgl/dbgl_barycentric.h>

double tri_compactness (double* d, double& area)
{
  double compactness = 4.0 * vcl_sqrt(3.0) * area / (d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);

  ///assert (compactness <= 1);
  if (compactness > 1) {
    assert (dbgl_eq_m (compactness, 1));
    compactness = 1;
  }

  return compactness;
}

//: Compute the barycentric coordinate of (G1, G2, G3)
//  Return false if numerical instability detected.
bool get_barycentric (const vgl_point_3d<double>& G1,
                      const vgl_point_3d<double>& G2,
                      const vgl_point_3d<double>& G3,
                      double& alpha, double& beta, double& gamma)
{
  const double u12 = dbgl_sqdist_3d (G1, G2);
  const double u23 = dbgl_sqdist_3d (G2, G3);
  const double u13 = dbgl_sqdist_3d (G1, G3);

  return _get_barycentric (u12, u23, u13, alpha, beta, gamma);
}

//: Compute the barycentric coordinate of (G1, G2, G3)
//  Input: d12, d23, d13.
//  Return false if numerical instability detected.
bool _get_barycentric (const double& u12, const double& u23, const double& u13,
                       double& alpha, double& beta, double& gamma)
{
  const double delta2_16 = _delta2_16_u (u12, u23, u13);

  alpha = u23 * (u12 + u13 - u23) / delta2_16;
  beta  = u13 * (u12 + u23 - u13) / delta2_16;
  gamma = u12 * (u13 + u23 - u12) / delta2_16;

  assert (dbgl_eq_m (alpha + beta + gamma, 1.0));
  return true;
}

void compute_tri_angles (const vgl_point_3d<double>& A, 
                         const vgl_point_3d<double>& B,
                         const vgl_point_3d<double>& C, 
                         double angle[3])
{
  const double a = vgl_distance (B, C);
  const double b = vgl_distance (A, C);
  const double c = vgl_distance (A, B);
  angle[0] = vcl_acos ( (b*b + c*c - a*a)/(b*c*2) );
  angle[1] = vcl_acos ( (a*a + c*c - b*b)/(a*c*2) );
  angle[2] = vcl_acos ( (a*a + b*b - c*c)/(a*b*2) );
}

//: Compute the circumCenter from at least 3 dbsk3d_bnd_pts.
//  We don't need to define a separate function for the Degenerate cases.
//
//  This is NOT the barycentric solution!
//
//  Input: 3 dbsk3d_bnd_pts of the dbsk3d_fs_edge
//  Output: the circumCenter and the time of the linkElm.
//  return: true if numerically stable, false if not.
//
vgl_point_3d<double> circum_center_3pts (const vgl_point_3d<double>& A, 
                                         const vgl_point_3d<double>& B,
                                         const vgl_point_3d<double>& C)
{
  // Use coordinates relative to point `a' of the triangle.
  double xba = B.x() - A.x();
  double yba = B.y() - A.y();
  double zba = B.z() - A.z();
  double xca = C.x() - A.x();
  double yca = C.y() - A.y();
  double zca = C.z() - A.z();

  // Squares of lengths of the edges incident to `a'. 
  double balength = xba * xba + yba * yba + zba * zba;
  double calength = xca * xca + yca * yca + zca * zca;

  // Take your chances with floating-point roundoff. 
  double xcrossbc = yba * zca - yca * zba;
  double ycrossbc = zba * xca - zca * xba;
  double zcrossbc = xba * yca - xca * yba;

  // Calculate the denominator of the formulae. 

  double dLength = xcrossbc * xcrossbc + ycrossbc * ycrossbc + zcrossbc * zcrossbc;
  
  assert (dLength != 0);
  double denominator = 0.5 / dLength;

  // Calculate offset (from `a') of circumcenter.
  double circumX = (balength * yca - calength * yba) * zcrossbc -
                   (balength * zca - calength * zba) * ycrossbc;
  circumX *= denominator;
  double circumY = (balength * zca - calength * zba) * xcrossbc -
                   (balength * xca - calength * xba) * zcrossbc;
  circumY *= denominator;
  double circumZ = (balength * xca - calength * xba) * ycrossbc -
                   (balength * yca - calength * yba) * xcrossbc;
  circumZ *= denominator;

  vgl_point_3d<double> circum_cen;
  circum_cen.set (circumX+A.x(), circumY+A.y(), circumZ+A.z());

  return circum_cen;
  ///double circumCenTime = sqrt (circumX*circumX + circumY*circumY + circumZ*circumZ);
}


#if 0
//: Ming: the tri-coord stuffs from Frederic
//: Evaluates the trilinear coordinates of the shock linkElm
//  return false if encounter numerical issues.
#define E_EPSILON 1E-6
bool dbsk3d_fs_edge::computeTriCoord()
{
  vcl_vector<dbmsh3d_vertex*> genes = get_ordered_genes ();

  if (genes.size()!=3)
    return false;

  dbmsh3d_vertex* pGeneA = genes[0];
  dbmsh3d_vertex* pGeneB = genes[1];
  dbmsh3d_vertex* pGeneC = genes[2];

  // Compute the Mu's : Squares of diameters linking pairs of sources
  double dMuAB = dbgl_sqdist_3d (pGeneB->pt(), pGeneA->pt());
  double dMuAC = dbgl_sqdist_3d (pGeneC->pt(), pGeneA->pt());
  double dMuBC = dbgl_sqdist_3d (pGeneC->pt(), pGeneB->pt());

  //double fPerimeter = (double) (sqrt(dMuAB) + sqrt(dMuAC) + sqrt(dMuBC));

  // Compute the (signed) trilinear coordinates
  double            triCoord_[3];  // Trilinear coordinates 

  ///triCoord_[0] = dAlpha;
  ///triCoord_[1] = dBeta;
  ///triCoord_[2] = dGamma;
  triCoord_[0] = dMuAC + dMuAB - dMuBC;
  double dBeta  = dMuBC + dMuAB - dMuAC;
  double dGamma = dMuBC + dMuAC - dMuAB;

  double dSum = 0.0;
  double dTmp = fabs(dMuBC * triCoord_[0] + dMuAC * dBeta + dMuAB * dGamma);
  double fTriArea;
  if(dTmp < E_EPSILON) {
    fTriArea = 0.0;
    triCoord_[0] *= dMuBC;
    dBeta  *= dMuAC;
    dGamma *= dMuAB;
    // Sum of coord. should be equal to one 
    dSum = fabs(triCoord_[0] + dBeta + dGamma - 1.0);
  }
  else {
    double dAreaSq = dTmp / 16.0;
    fTriArea = (double) sqrt(dAreaSq);
    triCoord_[0] *= (dMuBC / dTmp);
    dBeta  *= (dMuAC / dTmp);
    dGamma *= (dMuAB / dTmp);
    // Sum of coord. should be equal to one
    dSum = fabs (triCoord_[0] + dBeta + dGamma - 1.0);
    if(dSum > 0.01) {
      // If sum is near 0: likely that genes are co-linear 
      fprintf (stderr, "WARNING: linkElm %d, Tri Coords =\n", id_);
      fprintf(stderr, "\t(%lf , %lf , %lf) , Sum = %lf .\n",  triCoord_[0], dBeta, dGamma,  (triCoord_[0]+dBeta+dGamma));
    }
  }

  ///triCoord_[0] = dAlpha;
  triCoord_[1] = dBeta;
  triCoord_[2] = dGamma;

  if(dSum > 0.01) {
    ///TriType_ = A13LinkElement::TRIANGLE_FLAT_COLINEAR;
  }
  else {
    int iFlag = 0;
    if(triCoord_[0] < 0.0) iFlag++;
    if(dBeta  < 0.0) iFlag++;
    if(dGamma < 0.0) iFlag++;
    switch(iFlag) {
    case 0:
      ///TriType_ = A13LinkElement::TRIANGLE_ACUTE;
      break;
    case 1:
      ///TriType_ = A13LinkElement::TRIANGLE_OBTUSE;
      break;
    default:
      assert (0);
      fprintf(stderr, "ERROR: Too many neg. Tri Coords:\n");
      return false;
      break;
    }
  }

  return true;
}

//------------------------------------------------------------
//: computeQuadCoord()
//  Evaluates the barycentric coordinates of the Shock
//  Vertex with respect to its associated Tetrahedron,
//  as the circumcenter of the circumsphere.
//  Computes the signed volume as well.
//  Sets the distance (squared) to genes as well.
//  Return: true if success
//          false if has numerical errors.
//
//  fQuadCoord_[0] = fAlpha;
//  fQuadCoord_[1] = fBeta;
//  fQuadCoord_[2] = fGamma;
//  fQuadCoord_[3] = fDelta;
//

#define E_EPSILON  1E-6

bool dbsk3d_fs_vertex::computeQuadCoord()
{
  if (Genes_.size()!=4) {
    type_ = TYPE_DEGE;
    return false;
  }

  dbmsh3d_vertex* Gi = Genes_[0];
  dbmsh3d_vertex* Gj = Genes_[1];
  dbmsh3d_vertex* Gk = Genes_[2];
  dbmsh3d_vertex* Gl = Genes_[3];

  //: Compute the Mu's : Squares of diameters linking pairs of sources 
  double mu1 = dbgl_sqdist_3d (Gj->pt(), Gi->pt());
  double mu2 = dbgl_sqdist_3d (Gk->pt(), Gi->pt());
  double mu3 = dbgl_sqdist_3d (Gk->pt(), Gj->pt());
  double mu4 = dbgl_sqdist_3d (Gl->pt(), Gi->pt());
  double mu5 = dbgl_sqdist_3d (Gl->pt(), Gj->pt());
  double mu6 = dbgl_sqdist_3d (Gl->pt(), Gk->pt());

  //: Compute the (signed) quadrilinear coordinates
  double mu1mu6 = mu1 * mu6;
  double mu2mu5 = mu2 * mu5;
  double mu3mu4 = mu3 * mu4;

  double fAlpha = mu1mu6 * (mu3 + mu5 - mu6) + 
                  mu2mu5 * (mu3 + mu6 - mu5) +
                  mu3mu4 * (mu5 + mu6 - mu3) - (2 * mu3 * mu5 * mu6);

  double fBeta  = mu1mu6 * (mu2 + mu4 - mu6) + 
                  mu2mu5 * (mu4 + mu6 - mu2) +
                  mu3mu4 * (mu2 + mu6 - mu4) - (2 * mu2 * mu4 * mu6);

  double fGamma = mu1mu6 * (mu4 + mu5 - mu1) +
                  mu2mu5 * (mu1 + mu4 - mu5) +
                  mu3mu4 * (mu1 + mu5 - mu4) - (2 * mu1 * mu4 * mu5);

  double fDelta = mu1mu6 * (mu2 + mu3 - mu1) +
                  mu2mu5 * (mu1 + mu3 - mu2) +
                  mu3mu4 * (mu1 + mu2 - mu3) - (2 * mu1 * mu2 * mu3);

  //: WHY??
  double fSum = fAlpha + fBeta + fGamma + fDelta;
  double fTmp = fSum / 288.0;
  double fVolume;
  if(fTmp > 0.0)
    fVolume = (double) sqrt((double) fTmp);
  else
    fVolume = - (double) sqrt((double) -fTmp);

  if(fabs(fVolume) < E_EPSILON) {
    if(fSum < 0.0) {
      fAlpha = - fAlpha;
      fBeta  = - fBeta;
      fGamma = - fGamma;
      fDelta = - fDelta;
    }
  }
  else {
    fAlpha /= fSum;
    fBeta  /= fSum;
    fGamma /= fSum;
    fDelta /= fSum;
  }
  //double fSigma = fAlpha + fBeta + fGamma + fDelta;
  double      quadCoord_[4]; 

  quadCoord_[0] = fAlpha;
  quadCoord_[1] = fBeta;
  quadCoord_[2] = fGamma;
  quadCoord_[3] = fDelta;

  // --- Check Squared Radius values ---
  double fDSqA = dbgl_sqdist_3d (Gi->pt(), pt_);
  double fDSqB = dbgl_sqdist_3d (Gj->pt(), pt_);
  double fDSqC = dbgl_sqdist_3d (Gk->pt(), pt_);
  double fDSqD = dbgl_sqdist_3d (Gl->pt(), pt_);

  ///time_ = sqrt (fDSqA);
  double fTmpX, fTmpY, fTmpZ;
  if(fDSqA < E_EPSILON) {
    fprintf(stderr, "WARNING:\n\ttiny radius = %f\n", fDSqA);
    fTmpX = fabs(fDSqA - fDSqB);
    fTmpY = fabs(fDSqA - fDSqC);
    fTmpZ = fabs(fDSqA - fDSqD);
  }
  else {
    fTmpX = fabs(fDSqA - fDSqB) / fDSqA;
    fTmpY = fabs(fDSqA - fDSqC) / fDSqA;
    fTmpZ = fabs(fDSqA - fDSqD) / fDSqA;
  }
  //BAD:  If differences greater than 1 percent 
  if (fTmpX > 0.01 || fTmpY > 0.01 || fTmpZ > 0.01) {
    fprintf (stderr, "WARNING:\n\tRadii are not equal.\n");
    ////BAD: assert (0);
    return true;
  }

  return true;
}

#endif


#if 0
/*------------------------------------------------------------*\
 * Function: GetQuadCoordOfVertex
 *
 * Usage: Evaluates the barycentric coordinates of the Shock
 *    Vertex with respect to its associated Tetrahedron,
 *    as the circumcenter of the circumsphere.
 *    Computes the signed volume as well.
 *
\*------------------------------------------------------------*/

int
GetQuadCoordOfVertex(InputData *pInData, ShockVertex *pRed,
         ShockData *pShockData)
{
  char    *pcFName = "GetQuadCoordOfVertex";
  int    iGeneA,iGeneB,iGeneC,iGeneD, iFlag;
  static double  fEpsilon = (float) E_EPSILON;
  double    fAx,fAy,fAz, fBx,fBy,fBz, fCx,fCy,fCz, fDx,fDy,fDz;
  double    fTmpX,fTmpY,fTmpZ, fMuAB,fMuAC,fMuAD,fMuBC,fMuBD,fMuCD;
  double    fTmp1,fTmp2,fTmp3, fAlpha,fBeta,fGamma,fDelta;
  double    fVolume, fSum, fTmp, fABx,fABy,fABz;
  double    fACx,fACy,fACz, fADx,fADy,fADz;
  InputData  *pGeneA, *pGeneB, *pGeneC, *pGeneD;
  Pt3dCoord  *pGeneCoordA, *pGeneCoordB, *pGeneCoordC, *pGeneCoordD;

  /* Preliminaries */

  iGeneA = pRed->iGene[0];
  iGeneB = pRed->iGene[1];
  iGeneC = pRed->iGene[2];
  iGeneD = pRed->iGene[3];

  pGeneA = pInData+iGeneA;
  pGeneCoordA  = &(pGeneA->Sample);
  fAx = pGeneCoordA->fPosX;
  fAy = pGeneCoordA->fPosY;
  fAz = pGeneCoordA->fPosZ;

  pGeneB = pInData+iGeneB;
  pGeneCoordB  = &(pGeneB->Sample);
  fBx = pGeneCoordB->fPosX;
  fBy = pGeneCoordB->fPosY;
  fBz = pGeneCoordB->fPosZ;

  pGeneC = pInData+iGeneC;
  pGeneCoordC  = &(pGeneC->Sample);
  fCx = pGeneCoordC->fPosX;
  fCy = pGeneCoordC->fPosY;
  fCz = pGeneCoordC->fPosZ;

  pGeneD = pInData+iGeneD;
  pGeneCoordD  = &(pGeneD->Sample);
  fDx = pGeneCoordD->fPosX;
  fDy = pGeneCoordD->fPosY;
  fDz = pGeneCoordD->fPosZ;


  /* Compute the Mu's : Squares of diameters linking pairs of sources */

  fABx  = fBx - fAx; /* Distance AB */
  fTmpX = fABx * fABx;
  fABy  = fBy - fAy;
  fTmpY = fABy * fABy;
  fABz  = fBz - fAz;
  fTmpZ = fABz * fABz;

  fMuAB = fTmpX + fTmpY + fTmpZ;

  fACx  = fCx - fAx; /* Distance AC */
  fTmpX = fACx * fACx;
  fACy  = fCy - fAy;
  fTmpY = fACy * fACy;
  fACz  = fCz - fAz;
  fTmpZ = fACz * fACz;

  fMuAC = fTmpX + fTmpY + fTmpZ;

  fADx  = fDx - fAx; /* Distance AD */
  fTmpX = fADx * fADx;
  fADy  = fDy - fAy;
  fTmpY = fADy * fADy;
  fADz  = fDz - fAz;
  fTmpZ = fADz * fADz;

  fMuAD = fTmpX + fTmpY + fTmpZ;

  fTmpX = fCx - fBx; /* Distance BC */
  fTmpX *= fTmpX;
  fTmpY = fCy - fBy;
  fTmpY *= fTmpY;
  fTmpZ = fCz - fBz;
  fTmpZ *= fTmpZ;

  fMuBC = fTmpX + fTmpY + fTmpZ;

  fTmpX = fDx - fBx; /* Distance BD */
  fTmpX *= fTmpX;
  fTmpY = fDy - fBy;
  fTmpY *= fTmpY;
  fTmpZ = fDz - fBz;
  fTmpZ *= fTmpZ;

  fMuBD = fTmpX + fTmpY + fTmpZ;

  fTmpX = (fDx - fCx); /* Distance CD */
  fTmpX *= fTmpX;
  fTmpY = (fDy - fCy);
  fTmpY *= fTmpY;
  fTmpZ = (fDz - fCz);
  fTmpZ *= fTmpZ;

  fMuCD = fTmpX + fTmpY + fTmpZ;

  /* Compute the (signed) quadrilinear coordinates */

  fTmp1 = fMuAB * fMuCD;
  fTmp2 = fMuAC * fMuBD;
  fTmp3 = fMuAD * fMuBC;

  fAlpha =  fTmp1 * (fMuBC + fMuBD - fMuCD) +
           fTmp2 * (fMuBC + fMuCD - fMuBD) +
            fTmp3 * (fMuBD + fMuCD - fMuBC) - 
            (2 * fMuBC * fMuBD * fMuCD);

  fBeta  = fTmp1 * (fMuAC + fMuAD - fMuCD) +
     fTmp2 * (fMuAD + fMuCD - fMuAC) +
           fTmp3 * (fMuAC + fMuCD - fMuAD) - (2 * fMuAC * fMuAD * fMuCD);

  fGamma = fTmp1 * (fMuAD + fMuBD - fMuAB) +
     fTmp2 * (fMuAD + fMuAB - fMuBD) +
           fTmp3 * (fMuAB + fMuBD - fMuAD) - (2 * fMuAB * fMuAD * fMuBD);

  fDelta = fTmp1 * (fMuAC + fMuBC - fMuAB) +
     fTmp2 * (fMuAB + fMuBC - fMuAC) +
           fTmp3 * (fMuAB + fMuAC - fMuBC) - (2 * fMuAB * fMuAC * fMuBC);

  fSum = fAlpha + fBeta + fGamma + fDelta;
  fTmp = fSum / 288.0;
  if(fTmp > 0.0) {
    fVolume = (float) sqrt((double) fTmp);
  }
  else {
    fVolume = - (float) sqrt((double) -fTmp);
  }

  if(fabs(fVolume) < fEpsilon) {
    fprintf(stderr, "MESG(%s):\n", pcFName);
    fprintf(stderr, "\tTiny Volume of Tet = %lf\n", fVolume);
    if(fSum < 0.0) {
      fAlpha = - fAlpha;
      fBeta  = - fBeta;
      fGamma = - fGamma;
      fDelta = - fDelta;
    }
  }
  else {
    fAlpha /= fSum;
    fBeta  /= fSum;
    fGamma /= fSum;
    fDelta /= fSum;
  }
  /* fSigma  = fAlpha + fBeta + fGamma + fDelta; */
#if HIGH_MEM
  pRed->fQuadCoord[0] = fAlpha;
  pRed->fQuadCoord[1] = fBeta;
  pRed->fQuadCoord[2] = fGamma;
  pRed->fQuadCoord[3] = fDelta;
  pRed->fTetVolume = fVolume;
#endif
  pShockData->fGeom = fabs(fVolume);
  pShockData->fBaryCoord[0] = fAlpha;
  pShockData->fBaryCoord[1] = fBeta;
  pShockData->fBaryCoord[2] = fGamma;
  pShockData->fBaryCoord[3] = fDelta;

  iFlag = 0;
  if(fAlpha < 0.0) iFlag++;
  if(fBeta  < 0.0) iFlag++;
  if(fGamma < 0.0) iFlag++;
  if(fDelta < 0.0) iFlag++;

  switch(iFlag) {
  case 0:
    pRed->isType = RED_I; /* Circum inside Tet : All flows Inward */
    break;
  case 1:
    pRed->isType = RED_II; /* Circum Outside if one face: 1 flow out */
    break;
  case 2:
  case 3:
  case 4:
    pRed->isType = RED_III; /* Circum Out of 2 faces: 2 flows out */
    break;
  default:
    fprintf(stderr, "WARNING(%s): Quad Coords =\n", pcFName);
    fprintf(stderr, "\t(%lf , %lf , %lf , %lf)\n",
      fAlpha, fBeta, fGamma, fDelta);
    fprintf(stderr, "\tGeneA: %d: (%lf , %lf , %lf)\n", iGeneA, fAx,fAy,fAz);
    fprintf(stderr, "\tGeneB: %d: (%lf , %lf , %lf)\n", iGeneB, fBx,fBy,fBz);
    fprintf(stderr, "\tGeneC: %d: (%lf , %lf , %lf)\n", iGeneC, fCx,fCy,fCz);
    fprintf(stderr, "\tGeneD: %d: (%lf , %lf , %lf)\n", iGeneD, fDx,fDy,fDz);
    fprintf(stderr, "\tVolume = %lf \n", fVolume);
#if HIGH_MEM
    fprintf(stderr, "\tCircumCenter coords = (%lf , %lf , %lf)\n",
      pRed->CircumSphere.fPosX, pRed->CircumSphere.fPosY,
      pRed->CircumSphere.fPosZ);
#endif
    return(FALSE);
    break;
  }
  return(TRUE);
}

/*------------------------------------------------------------*\
 * Function: GetTriCoordOfCurve
 *
 * Usage: Evaluates the trilinear coordinates of the shock
 *    curve source with respect to its associated triplet
 *    of generators.
 *
\*------------------------------------------------------------*/

int
GetTriCoordOfCurve(InputData *pInData, ShockCurve *pGreen,
       ShockData *pShockData)
{
  char    *pcFName = "GetTriCoordOfCurve";
  int    iGeneA,iGeneB,iGeneC, iFlag, iGreen;
  static double  dEpsilon = (double) E_EPSILON;
  double  dAx,dAy,dAz, dBx,dBy,dBz, dCx,dCy,dCz, dSum;
  double  dTmpX,dTmpY,dTmpZ, dMuAB,dMuAC,dMuBC, dTmp;
  double  dAlpha,dBeta,dGamma, dAreaSq;
  InputData  *pGeneA, *pGeneB, *pGeneC;
  Pt3dCoord  *pGeneCoordA, *pGeneCoordB, *pGeneCoordC;

  /* Preliminaries */

  iGeneA = pGreen->iGene[0];
  iGeneB = pGreen->iGene[1];
  iGeneC = pGreen->iGene[2];

  pGeneA = pInData+iGeneA;
  pGeneCoordA  = &(pGeneA->Sample);
  dAx = (double) pGeneCoordA->fPosX;
  dAy = (double) pGeneCoordA->fPosY;
  dAz = (double) pGeneCoordA->fPosZ;

  pGeneB = pInData+iGeneB;
  pGeneCoordB  = &(pGeneB->Sample);
  dBx = (double) pGeneCoordB->fPosX;
  dBy = (double) pGeneCoordB->fPosY;
  dBz = (double) pGeneCoordB->fPosZ;

  pGeneC = pInData+iGeneC;
  pGeneCoordC  = &(pGeneC->Sample);
  dCx = (double) pGeneCoordC->fPosX;
  dCy = (double) pGeneCoordC->fPosY;
  dCz = (double) pGeneCoordC->fPosZ;

  /* Compute the Mu's : Squares of diameters linking pairs of sources */

  dTmpX = dBx - dAx; /* Distance AB */
  dTmpX *= dTmpX;
  dTmpY = dBy - dAy;
  dTmpY *= dTmpY;
  dTmpZ = dBz - dAz;
  dTmpZ *= dTmpZ;

  dMuAB = dTmpX + dTmpY + dTmpZ;
  /* dAB = sqrt(dMuAB); */

  dTmpX = dCx - dAx; /* Distance AC */
  dTmpX *= dTmpX;
  dTmpY = dCy - dAy;
  dTmpY *= dTmpY;
  dTmpZ = dCz - dAz;
  dTmpZ *= dTmpZ;

  dMuAC = dTmpX + dTmpY + dTmpZ;
  /* dAC = sqrt(dMuAC); */

  dTmpX = dCx - dBx; /* Distance BC */
  dTmpX *= dTmpX;
  dTmpY = dCy - dBy;
  dTmpY *= dTmpY;
  dTmpZ = dCz - dBz;
  dTmpZ *= dTmpZ;

  dMuBC = dTmpX + dTmpY + dTmpZ;
  /* dBC = sqrt(dMuBC); */

  /* Find Longest side */
  if(dMuAB > dMuAC) {
    if(dMuAB > dMuBC) {
      pGreen->fMaxSideLengthSq = (float) dMuAB;
      if(dMuAC < dMuBC)
  pGreen->fMinSideLengthSq = (float) dMuAC;
      else
  pGreen->fMinSideLengthSq = (float) dMuBC;
    }
    else {
      pGreen->fMaxSideLengthSq = (float) dMuBC;
      pGreen->fMinSideLengthSq = (float) dMuAC;
    }
  }
  else if(dMuAC > dMuBC) {
    pGreen->fMaxSideLengthSq = (float) dMuAC;
    if(dMuAB < dMuBC)
      pGreen->fMinSideLengthSq = (float) dMuAB;
    else
      pGreen->fMinSideLengthSq = (float) dMuBC;
  }
  else {
    pGreen->fMaxSideLengthSq = (float) dMuBC;
    pGreen->fMinSideLengthSq = (float) dMuAB;
  }

  pGreen->fPerimeter = (float) (sqrt(dMuAB) + sqrt(dMuAC) + sqrt(dMuBC));


  /* Compute the (signed) trilinear coordinates */

  dAlpha = dMuAC + dMuAB - dMuBC;
  dBeta  = dMuBC + dMuAB - dMuAC;
  dGamma = dMuBC + dMuAC - dMuAB;

  dTmp = fabs(dMuBC * dAlpha + dMuAC * dBeta + dMuAB * dGamma);
  if(dTmp < dEpsilon) {
    pGreen->fTriArea = 0.0;
    dAlpha *= dMuBC;
    dBeta  *= dMuAC;
    dGamma *= dMuAB;
    /* Sum of coord. should be equal to one */
    dSum = fabs(dAlpha + dBeta + dGamma - 1.0);
#if FALSE
    fprintf(stderr, "WARNING(%s): 16 AreaSq = %lf for Shock curve %d\n",
      pcFName, (float) dTmp, iGreen);
    if(dSum > 0.01) {
      fprintf(stderr, "\tTri Coords = (%lf , %lf , %lf), Sum = %lf .\n",
        dAlpha, dBeta, dGamma, (dAlpha+dBeta+dGamma));
    }
#endif
  }
  else {
    dAreaSq = dTmp / 16.0;
    pGreen->fTriArea = (float) sqrt(dAreaSq);
    dAlpha *= (dMuBC / dTmp);
    dBeta  *= (dMuAC / dTmp);
    dGamma *= (dMuAB / dTmp);
    /* Sum of coord. should be equal to one */
    dSum = fabs(dAlpha + dBeta + dGamma - 1.0);
    if(dSum > 0.01) {
      /* If sum is near 0: likely that genes are co-linear */
      fprintf(stderr, "WARNING(%s): Shock curve with Tri Coords =\n",
        pcFName);
      fprintf(stderr, "\t(%lf , %lf , %lf) , Sum = %lf .\n",
        dAlpha, dBeta, dGamma,  (dAlpha+dBeta+dGamma));
    }
  }

  pGreen->fTriCoord[0] = (float) dAlpha;
  pGreen->fTriCoord[1] = (float) dBeta;
  pGreen->fTriCoord[2] = (float) dGamma;

  pShockData->fGeom = (float) sqrt(dAreaSq);
  pShockData->fBaryCoord[0] = (float) dAlpha;
  pShockData->fBaryCoord[1] = (float) dBeta;
  pShockData->fBaryCoord[2] = (float) dGamma;

  iFlag = 0;
  if(dAlpha < 0.0) iFlag++;
  if(dBeta  < 0.0) iFlag++;
  if(dGamma < 0.0) iFlag++;

  switch(iFlag) {
  case 0:
    pGreen->isType = GREEN_I;
    break;
  case 1:
    pGreen->isType = GREEN_II;
    break;
  default:
    fprintf(stderr, "ERROR(%s): Too many neg. Tri Coords:\n", pcFName);
    fprintf(stderr, "\t(%lf , %lf , %lf)\n",
      dAlpha, dBeta, dGamma);
    return(FALSE);
    break;
  }

  return(TRUE);
}
#endif
