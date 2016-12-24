// This is dbgl/algo/internals/arcspline.h

#ifndef arcspline_h_
#define arcspline_h_

//:
// \file
// \brief C functions to interpolate a set of 2D points with a circular arc spline.
// This code is originated written by Xunnian Yang. It is an implementation of the paper:
// Xunnian Yang, "Efficient circular arc interpolation based on active tolerance control", 
// Computer-Aided Design 34(2002) 1037-1046
// 
// \author Nhon Trinh (ntrinh@lems.brown.edu)
// \date April 14, 2008
//
// \verbatim
//  Modifications
//    Nhon Trinh  4/15/2008   Fix bugs in the original code
// \endverbatim


enum boolean {FALSE = 0, TRUE = 1};

#define ZeroEps 1e-8
#define SptEps 1e-8
#define PI 3.14159265358979323846 

typedef double Real;
typedef  struct   linesmt    LineSmt;
typedef  struct   arcsmt     ArcSmt;
typedef  struct   curvesmt   CurveSmt;
typedef  struct   profile    Profile;

typedef struct  {
    Real        x, y, z;
} Point3D;

typedef    Point3D    Vector3D;


typedef struct  {
    Real        x, y, z, w;
} Vector4D;

struct   linesmt    {           /* line segment in 3D space   */
    Point3D       s_point;      /* start point of a segmebt   */
    Point3D       e_point;      /* end point of a segment     */
};   

struct   arcsmt     {           /* arc segment in 2D plane     */
    int           wise;         /* clockwie or counter_clockwise */
    Point3D       s_point;      /* start point of an arc segmt */
    Point3D       e_point;      /* end point of an arc segment */
    Real          s_angle;      /* start angle of an arcsmt    */
    Real          e_angle;      /* 0<=s_, e_engle ,=360        */
    Real          radius;       /* the radius of an arc        */
    Point3D       center;       /* the center of an arc        */
};   

struct curvesmt     {           /* a curve segment             */
    int           lnac;         /* is a curvesmt a line (0) or arc (1) */
    LineSmt       *line_smt;    /* a line  segment             */
    ArcSmt        *arc_smt;     /* an arc  segment             */
    CurveSmt      *prevc;       /* pnter to a previous segment */
    CurveSmt      *nextc;       /* pnter to a next segment     */
    Profile       *prfl;        /* the farther prfile         */
};

struct profile      {            /* structure for a prfile     */
    CurveSmt      *curve_smt;   /* the start segment of a prfile */
};


int   RealZero (Real re, Real eps);   /* Is |re| <= eps ? */
void  CopyVec3D (Vector3D  * newV, Vector3D  * oldV);      /* new = old */
void  SetVec3D (Vector3D   *v, Real x, Real y, Real z);     /*  v = (x, y, z)  */
void  SetVec4D (Vector4D *v, Real x, Real y, Real z, Real w);     /*  v = (x, y, z, w)  */
void  AddVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2);      /* v = v1 + v2  */
void  SubVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2);       /* v = v1 - v2  */
void  ScaleVec3D (Vector3D  *v, Real coef, Vector3D  *v1);   /*  v = coef * v1  */
int   Vec3DOverScale (Vector3D  *v, Vector3D  *v1, Real dm);  /* v = v1 / dm  */
void  CombVec3D (Vector3D  *v, Real c1, Vector3D  *v1, Real c2, Vector3D  *v2);  /* v = c1*v1 + c2*v2 */
void  DotVec3D (Real *dot, Vector3D  *v1, Vector3D  *v2);     /*  dot = v1 * v2  */
void  CrossVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2);    /*  v = v1 x v2   */
int   LenVec3D (Real        *len, Vector3D  *v);         /* len = |v| */
int   DistVec3D (Real    *len, Point3D *v1, Point3D *v2);
int   UnitVec3D (Vector3D    *v);     /* v = v/|v|   */
int   ZeroVec3D (Vector3D    *v);    /* Is v zerovector ?  */
int   SameVec3D (Vector3D    *v1, Vector3D    *v2);    /* v1 == v2 ?  */
int   SamePnt3D (Point3D    *v1, Point3D    *v2);    /* v1 == v2 ?  */

///* approximate linear approach */
//DistPntArc(&tm, Pn, arc1);
// TODO
int DistPntArc(Real* distance, Point3D P, ArcSmt* arc);

// Allocate memory
char  *MyMalloc (unsigned bytes);

/*
** insert a new segment after an old one in a curve 
*/
void  Insert(CurveSmt *ocur, CurveSmt *icur, Profile  *ppf);


// Function added by Nhon
// Not sure what the original function is since it is missing from the code
char* AllocCurveSmt(CurveSmt* smt);

// Function added by Nhon
// Not sure what the original function is since it is missing from the code
char* AllocPrfile( );

/*
**  del a segment in a curve chain
*/
void   Del(CurveSmt    *smt, Profile      *ppf);

/*
** Free a profile curve
*/
void  FreeProfile(Profile *ppf);

/*
**  Inverse a chain
*/
void   InverseProf(Profile  *ppf);

/*
**  Calculate the start_angle and end_angle of an arc
*/
void   ArcAngCalculation(ArcSmt  *arcs);


/*
** simplify an arc spline curve within a given tolerance
*/
void  UnfArcSplineReduction(Real  tol, Profile   *prf);

void  ZGF0(Real V[], int a, int b, Real A[], Real B[], Real C[], Real H[]);

/*
** Construct a biarc panel with given two points associated with two tangents
*/
void   BiarcPanel (Profile   *biarcs, Vector3D T, Vector3D N, Point3D  P0, Point3D  P1, Real afa, Real beta, Real fai);

/*
** Computing the optimal tangents based on minimal arc curve model
*/
void  OptimalTangent(Real  *afa, Real  *beta, Real  *dtheta, Real  *len, int n, Point3D *P, Real afa1, Real betan);

  
/*
** Compute all angles for an arc spline
*/
void    GetAngles(Profile  *ppf);

/*
** Fair curve reconstruction using biarc curve form
*/
Profile *FairCurReconstruct(int n, Point3D* P, Real afa1, Real betan);


#endif arcspline_h_

