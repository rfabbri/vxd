// This is dbgl/algo/internals/arcspline.cxx

//:
// \file

#include "arcspline.h"


#include <vcl_cmath.h>
#include <vcl_memory.h>
#include <vcl_cstdio.h>
#include <vcl_cstdlib.h>


int   RealZero (Real re, Real eps)   /* Is |re| <= eps ? */
{
    if (re <= eps && re >= -eps)  return  TRUE;
    else   return  FALSE;
}

void  CopyVec3D (Vector3D  * newV, Vector3D  * oldV)      /* new = old */
{
    newV->x = oldV->x;
    newV->y = oldV->y;
    newV->z = oldV->z;
}




void  SetVec3D (Vector3D   *v, Real x, Real y, Real z)     /*  v = (x, y, z)  */
{
    v->x = x;    v->y = y;   v->z = z;
}


void  SetVec4D (Vector4D *v, Real x, Real y, Real z, Real w)     /*  v = (x, y, z, w)  */
{
    v->x = x;    v->y = y;   v->z = z;  v->w = w;
}





void  AddVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2)      /* v = v1 + v2  */
{
    v->x = v1->x + v2->x;
    v->y = v1->y + v2->y;
    v->z = v1->z + v2->z;
}




void  SubVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2)       /* v = v1 - v2  */
{
    v->x = v1->x - v2->x;
    v->y = v1->y - v2->y;
    v->z = v1->z - v2->z;
}




void  ScaleVec3D (Vector3D  *v, Real coef, Vector3D  *v1)   /*  v = coef * v1  */
{
    v->x = coef * v1->x;
    v->y = coef * v1->y;
    v->z = coef * v1->z;
}




int   Vec3DOverScale (Vector3D  *v, Vector3D  *v1, Real dm)  /* v = v1 / dm  */
{
    if (RealZero (dm, ZeroEps)) return FALSE;
    v->x = v1->x/dm;
    v->y = v1->y/dm;
    v->z = v1->z/dm;
    return  TRUE;
}




void  CombVec3D (Vector3D  *v, Real c1, Vector3D  *v1, Real c2, Vector3D  *v2)  /* v = c1*v1 + c2*v2 */
{
    v->x = c1*v1->x + c2*v2->x;
    v->y = c1*v1->y + c2*v2->y;
    v->z = c1*v1->z + c2*v2->z;
}




void  DotVec3D (Real *dot, Vector3D  *v1, Vector3D  *v2)     /*  dot = v1 * v2  */
{
    *dot = v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}




void  CrossVec3D (Vector3D   *v, Vector3D   *v1, Vector3D   *v2)    /*  v = v1 x v2   */
{
    v->x = v1->y*v2->z - v1->z*v2->y;
    v->y = v1->z*v2->x - v1->x*v2->z;
    v->z = v1->x*v2->y - v1->y*v2->x;
}




int   LenVec3D (Real        *len, Vector3D  *v)         /* len = |v| */
{
    Real    a, b, c;

    a = fabs (v->x);   b = fabs (v->y);   c = fabs (v->z);
    if (a >= b && a >=c) {
        if (RealZero (a, ZeroEps))  *len = 0.0;
        else {
            b /= a;   c /= a;
            *len = a * sqrt (1.0 + b*b + c*c);
        }
    }
    else if (b >= a && b >= c) {
        if (RealZero (b, ZeroEps))  *len = 0.0;
        else {
            a /= b;   c /= b;
            *len = b * sqrt (1.0 + a*a + c*c);
        }
    }
    else {
        if (RealZero (c, ZeroEps))  *len = 0.0;
        else {
            a /= c;   b /= c;
            *len = c * sqrt (1.0 + a*a + b*b);
        }
    }

    if (RealZero (*len, ZeroEps))  return  FALSE;
    return  TRUE;
}



int   DistVec3D (Real    *len, Point3D *v1, Point3D *v2)
{
    Point3D  v;

    SubVec3D (&v, v1, v2);
    return  LenVec3D (len, &v);
}

        


int   UnitVec3D (Vector3D    *v)     /* v = v/|v|   */
{
    Real    len;

    LenVec3D (&len, v);
    if (RealZero (len, ZeroEps))  return FALSE;
    v->x /= len;
    v->y /= len;
    v->z /= len;
    return  TRUE;
}




int   ZeroVec3D (Vector3D    *v)    /* Is v zerovector ?  */
{
    if (RealZero (fabs (v->x)+fabs (v->y)+fabs (v->z), ZeroEps))
              return (TRUE);
    else    return (FALSE);
}




int   SameVec3D (Vector3D    *v1, Vector3D    *v2)    /* v1 == v2 ?  */
{
    Vector3D  v;
    SubVec3D (&v, v1, v2);
    if (ZeroVec3D (&v))   return (TRUE);
    else    return (FALSE);
}



int   SamePnt3D (Point3D    *v1, Point3D    *v2)    /* v1 == v2 ?  */
{
    if (fabs (v1->x - v2->x) + fabs (v1->y - v2->y) +
        fabs (v1->z - v2->z) < SptEps)  return  TRUE;
    return   FALSE;
}


// Distance between a point an an arc
// assume the arc is less than half a circle
int DistPntArc(Real* distance, Point3D P, ArcSmt* arc)
{
  if (arc == NULL)
    return FALSE;

  ArcAngCalculation(arc);
  Vector3D OP;
  Real angle_P, angle_start, angle_end;
  Point3D center;
  Real d1, d2;

  center = arc->center;
  SetVec3D(&OP, P.x - center.x, P.y - center.y, P.z - center.z);
  angle_P = atan2(OP.y, OP.x);
  if (angle_P < 0.0)    angle_P = angle_P + PI*2;

  angle_start = arc->s_angle;
  angle_end = arc->e_angle;

  // modify the angle so that we can determine their orders using their values
  angle_end = (angle_end >= angle_start) ? angle_end : (angle_end + 2*PI);
  angle_P = (angle_P >= angle_start) ? angle_P : (angle_P + 2*PI);

  // if P is outside the range of the circular arc, the closest distance
  // is the smaller between distances to the two end points.
  if (angle_P > angle_end) 
  {
    DistVec3D(&d1, &P, &arc->s_point);
    DistVec3D(&d2, &P, &arc->e_point);

    *distance = (d1 < d2) ? d1 : d2;
  }
  // if P is inside the range of the circular arc, the closest distance to the arc
  // is also closest distance to the circle
  else
  {
    LenVec3D(&d1, &OP);
    *distance = abs(d1 - arc->radius);
  }
  
  return 0;
}



/*
**
*/
char  *MyMalloc (unsigned bytes)
{
    char *tmp = (char *)malloc (bytes);
    if (!tmp) {
        fprintf (stderr, "\n Space overflowed !!!");
        exit (-1);
    }
    return tmp;
}


/*
** insert a new segment after an old one in a curve 
*/
void  Insert(CurveSmt *ocur, CurveSmt *icur, Profile  *ppf)
{
  CurveSmt *fcur;

  if (ocur!=NULL)   fcur = ocur->nextc;
  else  fcur = (CurveSmt *)ppf->curve_smt;

  if (ocur!=NULL)  ocur->nextc = icur; 
  icur->prevc = ocur;
  icur->nextc = fcur;
  icur->prfl  = ppf;
  if (fcur!=NULL)  fcur->prevc = icur;

  if (ocur==NULL)   ppf->curve_smt = icur;
}






// Function added by Nhon
// Not sure what the original function is since it is missing from the code
char* AllocCurveSmt(CurveSmt* smt)
{
  CurveSmt* new_smt = (CurveSmt*)MyMalloc(sizeof(CurveSmt));
  new_smt->line_smt = NULL;
  new_smt->arc_smt = NULL;
  new_smt->lnac = 0;

  // Put new_smt after smt
  if (smt == NULL)
  {
    new_smt->nextc = NULL;
    new_smt->prevc = NULL;
    new_smt->prfl = NULL;
  }
  else
  {
    Insert(smt->prevc, new_smt, smt->prfl);
  }

  return (char* ) new_smt;
}


// Function added by Nhon
// Not sure what the original function is since it is missing from the code
char* AllocPrfile( )
{
  Profile* profile = (Profile* ) MyMalloc(sizeof(Profile));
  profile->curve_smt = NULL;
  return (char* ) profile;
}




/*
**  del a segment in a curve chain
*/
void   Del(CurveSmt    *smt, Profile      *ppf)
{
  CurveSmt   *cur;

  cur = ppf->curve_smt;
  if (cur == NULL)  return;

  if (cur==smt)   
  {
    ppf->curve_smt = cur->nextc;
    free ((char *)cur);
  }
  else  
  {
    while ((cur!=smt)&&(cur!=NULL))  
    { 
      cur = cur->nextc;
    }

    if (cur == NULL)  
    { 
      printf("Cann't find this curvsmt in the profile!\n");
      free ((char *)smt);
      return;
    }
    else   
    {
      if (cur->prevc!=NULL)  cur->prevc->nextc = cur->nextc;
      if (cur->nextc!=NULL)  cur->nextc->prevc = cur->prevc;
      cur->prevc = NULL;
      cur->nextc = NULL;
      if (cur->line_smt!=NULL)  free ((char *)cur->line_smt);
      if (cur->arc_smt!=NULL)   free ((char *)cur->arc_smt);
  
      // HACK
      // Temporarily commented out by Nhon
      // Need to find out what they mean
      //if (cur->rbscr!=NULL)     FreeRBspCr3D (cur->rbscr);
      //if (cur->rbzcr!=NULL)     FreeRBezCr3D (cur->rbzcr);
      free ((char *)cur);
    }
  }
}
             

/*
** Free a profile curve
*/
void  FreeProfile(Profile *ppf)
{
  CurveSmt *cur, *cur0;

  if (ppf==NULL)  return;
  
  cur = ppf->curve_smt;
  while (cur != NULL) {
       cur0 = cur->nextc;
       Del(cur, ppf);
       cur  = cur0;
  }
  
  free ((char *)ppf);
}




/*
**  Inverse a chain
*/
void   InverseProf(Profile  *ppf)
{
    CurveSmt   *cur,  *cur0;

    /* find the last segment of a chain */
    cur = ppf->curve_smt;
    while (cur->nextc != NULL)  
        cur = cur->nextc;    
    ppf->curve_smt = cur;

    while (cur != NULL)   {
  cur0       = cur->nextc;
        cur->nextc = cur->prevc;
        cur->prevc = cur0;
  cur = cur->nextc;
    }
}

/*
**  Calculate the start_angle and end_angle of an arc
*/
void   ArcAngCalculation(ArcSmt  *arcs)
{
   Point3D Op,   P0,  P1;
         Real    x,    y,  dm;
         Real    sa,   ea, tem;

   if (arcs==NULL)  return;

   Op   = arcs->center;
         P0   = arcs->s_point;
         P1   = arcs->e_point;

         x    = P0.x - Op.x;
         y    = P0.y - Op.y;
         sa   = atan2(y, x);
         if (sa < 0.0)    sa = sa + PI*2;

         x    = P1.x - Op.x;
         y    = P1.y - Op.y;
         ea   = atan2(y, x);
         if (ea < 0.0)    ea = ea + PI*2;
      
       /* is the arc a full circle?  */
         dm = ea - sa;
   if (RealZero(dm, ZeroEps))  ea = ea + PI*2.0;

       /*
       ** is the arc clockwise or not
       ** 0__anticlockwise  1__clockwise
       */
         if (arcs->wise==1) {
             tem = sa;
             sa  = ea;
             ea  = tem;  
         }
   if (ea < sa)   ea = ea + PI*2.0;

         arcs->s_angle = sa;
   arcs->e_angle = ea;
}


/*
** simplify an arc spline curve within a given tolerance
*/
void  UnfArcSplineReduction(Real  tol, Profile   *prf)
{
  short   order, ord0, flag;
  CurveSmt *cur0, *cur1, *cur2;
  //CurveSmt *cur3;
  ArcSmt   *arc0, *arc1, *arc2, *arct;
  //ArcSmt   *arc3;
  Point3D  O0, O1, O2, Oa, Ob;
  //Point3D  O3;
  Point3D   P0, P1, P2, P3, Pn;
  Vector3D V0, V2, Vn, D;
  Real   r0, r1, r2, ra, rb;
  Real   X1, Y1, t0, t1, t2, t3, t4;
  Real   t, u, tb, ub, uc, tc;
  Real   tal, tl, tm, scl;  /* active tolerance */
  Real   tbl, tcl, d0, dn;
  Real   da, db, sl, tdl, tul;
  //Vector3D N0, Na;
  int   i, n, k;
  Point3D   *AP, *BP;
  

  ord0 = -1;
  tal  = tol;
  n    = 0;
  tdl  = tol;
  cur1 = prf->curve_smt;
  cur2 = cur1->nextc;
        arct = (ArcSmt  *)MyMalloc (sizeof(ArcSmt));
  AP   = (Point3D *)MyMalloc (sizeof(Point3D)*220);
  BP   = (Point3D *)MyMalloc (sizeof(Point3D)*220);
  flag = TRUE;


  while (cur2->nextc != NULL)  
  {
    cur0 = cur1;
    cur1 = cur2;
    cur2 = cur2->nextc;

    if ((cur0->lnac==1)&&(cur1->lnac==1)&&(cur2->lnac==1)) 
    {
      arc0 = cur0->arc_smt;
      arc1 = cur1->arc_smt;
      arc2 = cur2->arc_smt;
      if ( (arc0->wise==arc1->wise) &&(arc1->wise==arc2->wise) ) 
      {
        P0 = arc0->s_point;
        P1 = arc1->s_point;
        P2 = arc2->s_point;
        P3 = arc2->e_point;
        r0 = arc0->radius;
        r1 = arc1->radius;
        r2 = arc2->radius;
        O0 = arc0->center;
        O1 = arc1->center;
        O2 = arc2->center;

        order= 0;
        if ((r0<r1)&&(r1<r2))      order = 1;
        if ((r0>r1)&&(r1>r2))    order = 2;
        if ((r0>r1)&&(r2>r1))    order = 3;
        if ((r0<r1)&&(r2<r1))    order = 4;

        if (ord0!=order)   tal = tol; 
        ord0 = order;

        switch (order)  
        {
        case 1:
          /* compute the parameters for the new arcs */
          SubVec3D (&D, &O2, &O0);
          SubVec3D (&V0,&O0, &P0);
          SubVec3D (&V2,&O2, &P3);
          UnitVec3D(&V0);
          UnitVec3D(&V2);

          SubVec3D (&Vn, &P2, &O0);
          UnitVec3D(&Vn);
          DotVec3D (&t0, &Vn, &V0);
          X1 = 2.0*r0*tal + tal*tal;
          Y1 = ((r0+tal)*t0 + r0)*2.0; 
          if (Y1<0.0) tb = 0.0;
          else        tb = X1/Y1;
  
          t0 = r2 - r0;
          DotVec3D (&t1, &D, &D);
          DotVec3D (&t2, &D, &V0);
          DotVec3D (&t3, &D, &V2);
          DotVec3D (&t4, &V0,&V2);


          /* relaxing the first arc and recompute u */
          Y1 = (t0 - t2)*2.0;
          X1 = t0*t0 - t1;
          tc = X1/Y1;
          t  = (tc<tb)?tc:tb;
          if (t<0.0)  t = 0.0;

          X1 = t0*t0 - t1 + 2*t2*t - 2.0*t0*t;
          Y1 = 2.0*t0 - 2.0*t3 + 2.0*t4*t - 2.0*t;
          u  = X1/Y1;
          CombVec3D (&Oa, 1.0, &O0, t, &V0);
          CombVec3D (&Ob, 1.0, &O2, -u, &V2);
          ra = r0 + t;
          rb = r2 - u;
          SubVec3D (&Vn, &Oa, &Ob);
          UnitVec3D(&Vn);
          CombVec3D(&Pn, 1.0, &Ob, rb, &Vn);

          DistVec3D (&da, &Pn, &O1);
          da = da - r1;
          DistVec3D (&db, &P2, &Ob);
          db = db - rb; 

          CopyVec3D (&AP[n], &P2);
          n++;

          arct->center  = Oa;
          arct->radius  = ra;
          arct->s_point = P0;
          arct->e_point = Pn;
          arct->wise    = arc1->wise;
          ArcAngCalculation(arct);

          /* approximate linear approach */
          DistVec3D (&t0, &P1, &Oa);
          DistPntArc(&tm, Pn, arc1);
          sl  = arc0->e_angle - arc0->s_angle;
          d0  = sl * r0;
          sl  = arct->e_angle - arct->s_angle;
          dn  = sl * ra;

          scl = dn/d0;  
          tl  = ra - t0;
          tbl = (tal - tl)*scl;
          tcl = tol - tm;
          tul = (tbl<tcl)?tbl:tcl;


          /*the new point is on the first arc segment */
          DistVec3D (&dn, &Pn, &O0);
          d0  = dn - r0;
          if (abs(d0)<ZeroEps)  da = ZeroEps;

          /* recompute the maximum deviation*/
          tdl = 0.0;
          k   = 0;
          if (db>ZeroEps)  
          {  
            for (i=0; i<n; i++)  
            {
              DistVec3D (&d0, &AP[i], &Ob);
              dn = d0 - rb;
              SubVec3D (&V0, &AP[i], &Pn);
              SubVec3D (&V2, &P3, &Pn);
              DotVec3D (&tl, &V0, &V2);
                                            
              if ((tdl<dn)&&(tl>0.0)) tdl = dn;
              if (tl>0.0)  
              {
                CopyVec3D (&(BP[k]), &(AP[i]));
                k++;
              }
            }
          }
        
          for (i=0; i<k; i++)
          {
            CopyVec3D (&(AP[i]), &(BP[i]));
          }
          n = k;


          /* set up two new arc segments */
          if (tdl<tol) 
          { 
            arc1->center  = Oa;
            arc1->radius  = ra;
            arc1->s_point = P0;
            arc1->e_point = Pn;
            arc2->center  = Ob;
            arc2->radius  = rb;
            arc2->s_point = Pn;
            arc2->e_point = P3;
            ArcAngCalculation(arc1);
            ArcAngCalculation(arc2);
            tal = tul;
          
            Del (cur0, prf); 
          }
          else  
          {
            //printf("case1, new arc\n");
            tal = tol;
            n   = 0;
          }
          
          break;

        case 2:
          SubVec3D (&D, &O2, &O0);
          SubVec3D (&V0,&O0, &P0);
          SubVec3D (&V2,&O2, &P3);
          UnitVec3D(&V0);
          UnitVec3D(&V2);

          SubVec3D (&Vn, &P1, &O2);
          UnitVec3D(&Vn);
          DotVec3D (&t0, &Vn, &V0);
          X1 = 2.0*r0*tal + tal*tal;
          Y1 = (r0*t0 + r0 + tal)*2.0;
          if (Y1<0.0) ub = 0.0;
          else        ub = X1/Y1;
  
          t0 = r0 - r2;
          DotVec3D (&t1, &D, &D);
          DotVec3D (&t2, &D, &V0);
          DotVec3D (&t3, &D, &V2);
          DotVec3D (&t4, &V0,&V2);


          /* relaxing the first arc and recompute u */
          X1 = t0*t0 - t1;
          Y1 = (t0 + t2)*2.0;
          uc = X1/Y1;
          u  = (uc<ub)?uc:ub;

          X1 = t1 - t0*t0 + 2.0*(t2 + t0)*u;
          Y1 = 2.0*(1.0-t4)*u - 2.0*(t0+t3);
          t  = X1/Y1;

          CombVec3D (&Oa, 1.0, &O0, -u, &V0);
          CombVec3D (&Ob, 1.0, &O2, t, &V2);
          ra = r0 - u;
          rb = r2 + t;
          SubVec3D (&Vn, &Ob, &Oa);
          UnitVec3D(&Vn);
          CombVec3D(&Pn, 1.0, &Ob, rb, &Vn);


          DistVec3D (&da, &Pn, &O1);
          DistVec3D (&db, &P2, &Ob);
          da = r1 - da;
          db = db - rb;
          db = abs(db);

          CopyVec3D (&AP[n], &P2);
          n++;
      
          arct->center  = Oa;
          arct->radius  = ra;
          arct->s_point = P0;
          arct->e_point = Pn;
          arct->wise    = arc1->wise;
          ArcAngCalculation(arct);
      
          /* approximate linear approach */
          
          /*DistPntArc(&tm, Pn, arc1); */
          DistVec3D (&tm, &O1, &Pn);
                      tm = tm - r1;
          DistVec3D (&t0, &P1, &Oa);
          DistVec3D (&d0, &P0, &P1);
          DistVec3D (&dn, &P0, &Pn);
          scl = dn/d0;    scl = 1.0;
          tl  = t0 - ra;
          tbl = (tal - tl)*scl;
          tcl = tol - tm;
          tul = (tbl<tcl)?tbl:tcl;
          if (tul<0.0)  tul = 0.0;

          tdl = 0.0;
          k   = 0;
                                
          if (db>ZeroEps)  
          { 
            for (i=0; i<n; i++)  
            {
              DistVec3D (&d0, &AP[i], &Ob);
              dn = abs(rb - d0);
              SubVec3D (&V0, &AP[i], &Pn);
              SubVec3D (&V2, &P3, &Pn);
              DotVec3D (&tl, &V0, &V2);
                                              
              if ((tdl<dn)&&(tl>0.0)) tdl = dn;
                
              if (tl>0.0)  
              {
                CopyVec3D(&(BP[k]), &(AP[i]));
                k++;
              }
            }
          } 
        
          for (i=0; i<k; i++) 
          {
            CopyVec3D(&(AP[i]), &(BP[i]));
          }
          n = k;


          /* set up two new arc segments */
          if (tdl<tol) 
          {  
            arc1->center  = Oa;
            arc1->radius  = ra;
            arc1->s_point = P0;
            arc1->e_point = Pn;
            arc2->center  = Ob;
            arc2->radius  = rb;
            arc2->s_point = Pn;
            arc2->e_point = P3;
            ArcAngCalculation(arc1);
            ArcAngCalculation(arc2);
            tal = tul;
          
            Del (cur0, prf); 
          }
          else  
          {
            tal = tol; 
            n   = 0;
            //printf("case2, new arc\n");
          }
          break;

        case 3:
          //printf("case3, new arc\n");
          n = 0;
          break;

        case 4:
          //printf("case4, new arc\n");
          n = 0;
          break;

        default:
          break;
        }  /* end of switch */
      }  /* end of if arc-wise */
      else 
      {
        tal = tol;
        //printf("sign change!  tal=tol\n");
      }
    }  /* end of if */
  }  /* end of while */

  free ((char *)arct);
  free ((char *)AP);
  free ((char *)BP);
}


void  ZGF0(Real V[], int a, int b, Real A[], Real B[], Real C[], Real H[])
{
  Real    *f,  *e;
  Real    tmp;
  int     i;

  f = (Real *)MyMalloc ((b+2)*sizeof (Real));
  e = (Real *)MyMalloc ((b+2)*sizeof (Real));

  tmp    = 1.0/B[a-1];
  f[a-1] = C[a-1]*tmp;
  V[a-1] = H[a-1]*tmp;
  for (i=a; i<=b; i++)   {
    e[i] = B[i] - A[i]*f[i-1];
    tmp  = 1.0/e[i];
    f[i] = C[i]*tmp;
    V[i] = tmp*H[i] - tmp*A[i]*V[i-1];
  }

  tmp = 1.0/(B[b+1]-A[b+1]*f[b]);
  V[b+1] = tmp*H[b+1] - tmp*A[b+1]*V[b];
  for (i=b; i>=a-1; i--)
    V[i] = V[i] - f[i]*V[i+1];

  free ((char *)f);
  free ((char *)e);
}


/*
** Construct a biarc panel with given two points associated with two tangents
*/
void   BiarcPanel (Profile   *biarcs, Vector3D T, Vector3D N, Point3D  P0, Point3D  P1, Real afa, Real beta, Real fai)
{
  CurveSmt  *cur, *cur1, *cur2;
  ArcSmt    *arc1, *arc2;
  LineSmt   *ln1,  *ln2;
  Point3D   A, B, C;
  Vector3D  U, V, G, W, L, Q, L1;
  Vector3D  T1;
  Real      ang, aa, ab;
  //Real      start, end;
  Real      t, r1, r2;
  Real      va1, va2, len;
  Real      cval, d1, d2;
  //int       ix,  iy;



  aa = abs(afa);  ab = abs(beta);
  if ((aa<ZeroEps)&&(ab<ZeroEps))  cval = -1.0;
  else 
  {
    /*
    if (ab<ZeroEps) t = 70.0*PI/180.0;
    else {
      if (((afa>ZeroEps)&&(beta>ZeroEps))
        ||((afa<(-ZeroEps))&&(beta<(-ZeroEps))))
        t = afa;
      else t = (afa*3.0 - beta)/2.0;
    }
    */


                if (((afa>ZeroEps)&&(beta>ZeroEps))
                        ||((afa<(-ZeroEps))&&(beta<(-ZeroEps))))
                        t = afa;
                else t = (afa*3.0 - beta)/2.0;

    ang = abs(afa+beta);
    DistVec3D (&len, &P0, &P1);
    SetVec3D (&U, cos(fai+t), sin(fai+t), 0.0);
    SetVec3D (&V, -U.y, U.x, 0.0);
    SetVec3D (&T1, cos(fai+afa+beta), sin(fai+afa+beta), 0.0);
    SubVec3D (&G,  &U, &T1);
    UnitVec3D (&G);
    SubVec3D (&W,  &N, &V);
    SubVec3D (&L,  &P1, &P0);

    DotVec3D (&va1, &L, &G);
    DotVec3D (&va2, &W, &G);
    /*
    if ((va2>0.000001)||(va2<-0.000001))
      r1 = va1/va2;
    else    r1 = 0.0;
    */
    r1 = va1/va2;
    if (ang<ZeroEps)  r1 = len/(4.0*sin(afa));
    CombVec3D (&C, 1.0, &P0, r1, &W);
    CombVec3D (&A, 1.0, &P0, r1, &N);

    SubVec3D (&Q, &P1, &C);
    DotVec3D (&va1, &Q, &Q);
    DotVec3D (&va2, &Q, &V);
    /*
    if ((va2>0.000001)||(va2<-0.000001))
      r2 = va1/va2;
    else  r2 = 0.0;
    */
      r2 = va1/va2;
    r2 = r2/2.0;
    if (ang<ZeroEps)  r2 = len/(4.0*sin(beta));
    CombVec3D (&B, 1.0, &C, r2, &V);

    /* 
    ** to check if C is between two points P0 & P1
    */
    SubVec3D (&U, &C, &P0);
    SubVec3D (&V, &C, &P1);
    SubVec3D (&L1,&P0,&P1);
    LenVec3D (&d1, &U);
    LenVec3D (&d2, &V);
    if (d1 < d2)
      DotVec3D (&cval, &U, &L);
    else
      DotVec3D (&cval, &V, &L1);
  }

  cval = cval;

  if (cval < 0.0)  {
    /*
             **  Alloc space for one new curve segment
             */
    cur  = (CurveSmt *)biarcs->curve_smt;
    cur1 = (CurveSmt *)AllocCurveSmt(cur);
    if (cur == NULL) {
      cur1->prfl        = biarcs;
      biarcs->curve_smt = cur1;
    }
    ln1  = (LineSmt *)MyMalloc (sizeof (LineSmt));
    ln1->s_point  = P0;
    ln1->e_point  = P1;

    cur1->lnac    = 0;
    cur1->line_smt= ln1;
  }
  else {
    /*
       **  Alloc space for two new curve segment
       */
    cur  = (CurveSmt *)biarcs->curve_smt;
    cur1 = (CurveSmt *)AllocCurveSmt(cur);
    if (cur == NULL) {
      cur1->prfl        = biarcs;
      biarcs->curve_smt = cur1;
    }
    cur2 = (CurveSmt *)AllocCurveSmt(cur1);

    /*
    ** if the radius of an arc is greater than 10000
    ** then the arc is regarded as a line_smt
    */
    if (r1<100000.0&&r1>-100000.0)  {
      arc1 = (ArcSmt *)MyMalloc (sizeof (ArcSmt));
      arc1->center  = A;
      arc1->wise    = 0;
      arc1->s_point = P0;
      arc1->e_point = C;
      if (r1 < 0.0)   arc1->wise = 1;
      if (r1 < 0.0)   r1 = r1 * (-1.0);
      arc1->radius  = r1;

      cur1->lnac    = 1;
      cur1->arc_smt = arc1;
    }
    else {
      ln1  = (LineSmt *)MyMalloc (sizeof (LineSmt));
      ln1->s_point  = P0;
      ln1->e_point  = C;

      cur1->lnac    = 0;
      cur1->line_smt= ln1;
    }

    if (r2<100000.0&&r2>-100000.0)  {
      arc2 = (ArcSmt *)MyMalloc (sizeof (ArcSmt));
      arc2->center  = B;
      arc2->wise    = 0;
      arc2->s_point = C;
      arc2->e_point = P1;
      if (r2 < 0.0)   arc2->wise = 1;
      if (r2 < 0.0)   r2 = r2 * (-1.0);
      arc2->radius  = r2;

      cur2->lnac    = 1;
      cur2->arc_smt = arc2;
    }
    else {
      ln2  = (LineSmt *)MyMalloc (sizeof (LineSmt));
      ln2->s_point  = C;
      ln2->e_point  = P1;

      cur2->lnac    = 0;
      cur2->line_smt= ln2;
    }
  }   /* end of if cval>=0.0   */

}




/*
** Computing the optimal tangents based on minimal arc curve model
*/
void  OptimalTangent(Real  *afa, Real  *beta, Real  *dtheta, Real  *len, int n, Point3D *P, Real afa1, Real betan)
{
  int   i, j;
  Real   *dlen;
  Real     *A, *B, *C, *H;
  Vector3D L0, L1, Lv0;
  Real     vau, val, lt, nmt;
  //Real lu, lv;


  dlen   = (Real *)MyMalloc((n+2)*sizeof (Real ));
  A      = (Real *)MyMalloc((n+2)*sizeof (Real ));
  B      = (Real *)MyMalloc((n+2)*sizeof (Real ));
  C      = (Real *)MyMalloc((n+2)*sizeof (Real ));
  H      = (Real *)MyMalloc((n+2)*sizeof (Real ));

  if (n > 1)  {
      dtheta[0] = afa1; 
      for (i=1; i<n; i++)    {
    SubVec3D (&L0, &P[i], &P[i-1]);
    SubVec3D (&L1, &P[i+1], &P[i]);
    SetVec3D (&Lv0, -L0.y, L0.x, 0.0);
    DotVec3D (&vau, &L1, &Lv0);
    DotVec3D (&val, &L1, &L0);
    dtheta[i] = atan2(vau, val);
      }
      dtheta[n] = betan;
  }
  
  /*
        ** Compute the basic parameters for the equation group
        */
        for (i=1; i<=n; i++)   {
                DistVec3D (&lt, &(P[i]), &(P[i-1]));
                len[i]  = lt;  
                dlen[i] = lt*lt;
        }

  
  nmt  = 1.6;
        H[0] = H[1] = 0.0;
        H[2] = (1.0+nmt)*dtheta[1]/len[1] + nmt*dtheta[2]/len[2]
                - nmt*afa1/len[1];
        if (n>3)    {
                for (i=3; i<n; i++)
                        H[i] = (1.0+nmt)*dtheta[i-1]/len[i-1]
                                + nmt*dtheta[i]/len[i];
                H[n] = (1.0+nmt)*dtheta[n-1]/len[n-1] + nmt*betan/len[n];
        }


        for (j=3; j<=n; j++)    A[j] = nmt/len[j-1];

        for (j=2; j<=n; j++)    B[j] = (1.0+nmt)*(1.0/len[j-1]+1.0/len[j]);

        for (j=2; j<n; j++)     C[j] = nmt/len[j];
  

  if (n > 1)   {
      ZGF0 (afa, 3, n-1, A, B, C, H);
      afa[1] = afa1;
      for (j=1; j<n; j++)  
    beta[j] = dtheta[j] - afa[j+1];
  }
  beta[n] = betan;

  free ((char *)dlen);
  free ((char *)A);
  free ((char *)B);
  free ((char *)C);
  free ((char *)H);
}


  
/*
** Compute all angles for an arc spline
*/
void    GetAngles(Profile  *ppf)
{
         CurveSmt  *cur;
         ArcSmt    *arcs;

         cur = ppf->curve_smt;
         while (cur!=NULL) {
             if (cur->lnac == 1)  {
                  arcs = cur->arc_smt;
                  ArcAngCalculation(arcs);
             }

             cur = cur->nextc;
         }
}



/*
** Fair curve reconstruction using biarc curve form
*/
Profile *FairCurReconstruct(int n, Point3D* P, Real afa1, Real betan)
{
  int   i;
  Profile   *fcur;
  Real     *afa, *beta, *dtheta, *len;
  Real   vau, val, fai;
  Vector3D UnitV, UnitV0, L1, T, N;


  if (n<=1)  return NULL;
  fcur   = (Profile *)AllocPrfile( );

  afa    = (Real *)MyMalloc((n+2)*sizeof (Real ));
  beta   = (Real *)MyMalloc((n+2)*sizeof (Real ));
  dtheta = (Real *)MyMalloc((n+2)*sizeof (Real ));
  len    = (Real *)MyMalloc((n+2)*sizeof (Real ));
  
  OptimalTangent(afa, beta, dtheta, len, n, P, afa1, betan);
  

    SetVec3D (&UnitV, 1.0, 0.0, 0.0);
   SetVec3D (&UnitV0,0.0, 1.0, 0.0);
  SubVec3D (&L1, &P[1], &P[0]);
   DotVec3D (&vau, &L1, &UnitV0);
   DotVec3D (&val, &L1, &UnitV);
   fai  = atan2(vau, val);
   fai  = fai - afa[1];


  // bug fix by Nhon (changed "i<=n" to "i<n")
   for (i=1; i<n; i++)   {
     SetVec3D (&T, cos(fai), sin(fai), 0.0);
     SetVec3D (&N, -T.y, T.x, 0.0);
     BiarcPanel (fcur, T, N, P[i-1], P[i], afa[i], beta[i], fai);
     fai = fai + afa[i] + beta[i];
   }

   GetAngles(fcur);
  InverseProf(fcur);
  
  free ((char *)afa);
  free ((char *)beta);
  free ((char *)dtheta);
  free ((char *)len);

  return (fcur);
}


