#include "sdetd_load_edg.h"
#include <vcl_cstring.h>
#include <vcl_locale.h>

#include <vul/vul_file.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <sdet/sdet_edgemap.h>
#include <sdet/sdet_sel_utils.h>

#ifdef HAS_BOOST
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#endif

bool sdetd_load_edg(vcl_string input_file, bool bSubPixel, bool blines, double scale,
                    vcl_vector< vsol_spatial_object_2d_sptr > &edgels)
{
  double x, y;
  char lineBuffer[1024];
  int numGeometry, w=0, h=0;
  int ix, iy;
  double idir, iconf, dir, conf;
  
  //1)If file open fails, return.
  vcl_ifstream infp(input_file.c_str(), vcl_ios::in);

  if (!infp){
    vcl_cout << " Error opening file  " << input_file.c_str() << vcl_endl;
    return false;
  }

  bool file_valid = false;

  //2)Read in the header block
  for (int j=0; j<9; j++){

    //read next line
    infp.getline(lineBuffer,1024);

    //read the line with the file type
    if (!vcl_strncmp(lineBuffer, "# EDGE_MAP", sizeof("# EDGE_MAP")-1)){
      file_valid = true;
      continue;
    }

    //ignore all other comment lines and empty lines
    if (vcl_strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " WIDTH=", sizeof(" WIDTH=")-1)){
      sscanf(lineBuffer," WIDTH=%d",&(w));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "WIDTH=", sizeof("WIDTH=")-1)){
      sscanf(lineBuffer,"WIDTH=%d",&(w));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " HEIGHT=", sizeof(" HEIGHT=")-1)){
      sscanf(lineBuffer," HEIGHT=%d",&(h));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "HEIGHT=", sizeof("HEIGHT=")-1)){
      sscanf(lineBuffer,"HEIGHT=%d",&(h));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " EDGE_COUNT=", sizeof(" EDGE_COUNT=")-1)){
      sscanf(lineBuffer," EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "EDGE_COUNT=", sizeof("EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
  }
  
  //make sure the file is valid
  if (!file_valid){
    vcl_cout << "Invalid .edg file (vsol/ascii)" << vcl_endl;
    return false;
  }

  // make sure w and h are reasonable
  if (w==0 && h==0) { w=1000; h=1000; }

  //read the data block
  while (infp.getline(lineBuffer,1024)) {

    //there are two variations of this file in existence
    if (!vcl_strncmp(lineBuffer, "EDGE : ", sizeof("EDGE : ")-1))
      sscanf(lineBuffer,"EDGE :  [%d, %d]    %lf %lf   [%lf, %lf]   %lf %lf",&(ix), &(iy),
        &(idir), &(iconf), &(x), &(y), &(dir), &(conf));
    else
      sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf",&(ix), &(iy),
        &(idir), &(iconf), &(x), &(y), &(dir), &(conf));

    //scale the edges
    x *= scale;
    y *= scale;

    ix = (int)(ix*scale);
    iy = (int)(iy*scale);

    if (bSubPixel){
      if ( blines){
        vsol_line_2d_sptr newLine = new vsol_line_2d(vgl_vector_2d<double>(vcl_cos(dir)/2.0, vcl_sin(dir)/2.0), vgl_point_2d<double>(x,y));
        edgels.push_back(newLine->cast_to_spatial_object());
      }
      else {
        vsol_point_2d_sptr newPt = new vsol_point_2d(x,y);
        edgels.push_back(newPt->cast_to_spatial_object());
      }
    }
    else { //pixel edges
      if (blines){
        vsol_line_2d_sptr newLine = new vsol_line_2d(vgl_vector_2d<double>(vcl_cos(idir), vcl_sin(idir)), vgl_point_2d<double>((double)ix, (double)iy));
        edgels.push_back(newLine->cast_to_spatial_object());
      }
      else {
        vsol_point_2d_sptr newPt = new vsol_point_2d((double)ix, (double)iy);
        edgels.push_back(newPt->cast_to_spatial_object());
      }
    }
  }
  infp.close();

  return true;

}

#ifdef HAS_BOOST
static bool sdetd_load_edg_gzip(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map);
static bool sdetd_save_edg_gzip(vcl_string filename, sdet_edgemap_sptr edgemap);
#endif

static bool sdetd_load_edg_ascii(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map);

static bool sdetd_save_edg_ascii(vcl_string filename, sdet_edgemap_sptr edgemap);

bool sdetd_load_edg(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map)
{
  vcl_string ext = vul_file::extension(input_file);
  bool ret;

  char * cur_locale, * dup_locale;
  cur_locale = setlocale(LC_NUMERIC, NULL);
  dup_locale = strdup(cur_locale);
  setlocale(LC_NUMERIC, "C");

  if (ext == ".gz") {
#ifdef HAS_BOOST
    ret = sdetd_load_edg_gzip(input_file, bSubPixel, scale, edge_map);
#else
    vcl_cerr << "Error: .gz compressed edg file was provided, but boost wasn't found\n";
#endif
  } else
    ret = sdetd_load_edg_ascii(input_file, bSubPixel, scale, edge_map);

  setlocale(LC_NUMERIC, dup_locale);
  free(dup_locale);

  return ret;
}

bool sdetd_save_edg(vcl_string filename, sdet_edgemap_sptr edgemap)
{
  vcl_string ext = vul_file::extension(filename);
  bool ret;

  char * cur_locale, * dup_locale;
  cur_locale = setlocale(LC_NUMERIC, NULL);
  dup_locale = strdup(cur_locale);
  setlocale(LC_NUMERIC, "C");

  if (ext == ".gz") {
#ifdef HAS_BOOST
    ret = sdetd_save_edg_gzip(filename, edgemap);
#else
    vcl_cerr << "Error: .gz compressed output edg file requested, but boost wasn't found\n";
#endif
  } else
    ret = sdetd_save_edg_ascii(filename, edgemap);

  setlocale(LC_NUMERIC, dup_locale);
  free(dup_locale);

  return ret;
}

bool sdetd_load_edg_ascii(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map)
{
  double x, y;
  char lineBuffer[1024];
  int numGeometry, w=0, h=0;
  int ix, iy;
  double idir, iconf, dir, conf, uncer=0;
  
  //1)If file open fails, return.
  vcl_ifstream infp(input_file.c_str(), vcl_ios::in);

  if (!infp){
    vcl_cout << " Error opening file  " << input_file.c_str() << vcl_endl;
    return false;
  }

  bool file_valid = false;
  int ver = 0;

  //2)Read in the header block
  for (int j=0; j<9; j++){

    //read next line
    infp.getline(lineBuffer,1024);

    //read the line with the file type
    if (!vcl_strncmp(lineBuffer, "# EDGE_MAP", sizeof("# EDGE_MAP")-1)){
      file_valid = true;

      if (!vcl_strncmp(lineBuffer, "# EDGE_MAP v2.0", sizeof("# EDGE_MAP v2.0")-1))
        ver = 2;
      else if (!vcl_strncmp(lineBuffer, "# EDGE_MAP v3.0", sizeof("# EDGE_MAP v3.0")-1))
        ver = 3;
      else
        ver = 1;
      
      continue;
    }

    //ignore all other comment lines and empty lines
    if (vcl_strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " WIDTH=", sizeof(" WIDTH=")-1)){
      sscanf(lineBuffer," WIDTH=%d",&(w));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "WIDTH=", sizeof("WIDTH=")-1)){
      sscanf(lineBuffer,"WIDTH=%d",&(w));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " HEIGHT=", sizeof(" HEIGHT=")-1)){
      sscanf(lineBuffer," HEIGHT=%d",&(h));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "HEIGHT=", sizeof("HEIGHT=")-1)){
      sscanf(lineBuffer,"HEIGHT=%d",&(h));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " EDGE_COUNT=", sizeof(" EDGE_COUNT=")-1)){
      sscanf(lineBuffer," EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "EDGE_COUNT=", sizeof("EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
  }
  
  //make sure the file is valid
  if (!file_valid){
    vcl_cout << "Invalid .edg file (ascii)" << vcl_endl;
    return false;
  }

  // make sure w and h are reasonable
  if (w==0 && h==0) { w=1000; h=1000; }

  // edge_map 
  edge_map = new sdet_edgemap(static_cast<int>(w*scale), static_cast<int>(h*scale));

  edge_map->edgels.reserve(numGeometry);

  //read the data block
  while (infp.getline(lineBuffer,1024)) {

    //there are two variations of this file in existence
    if (ver<3){
      int a = 0;
      if (!vcl_strncmp(lineBuffer, "EDGE : ", sizeof("EDGE : ")-1)){
        a=sscanf(lineBuffer,"EDGE :  [%d, %d]    %lf %lf   [%lf, %lf]   %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf));
      }else{
        a=sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf));}
      if(a!=8) {
         vcl_cout << "sdetd_load_edg_ascii: input error argument" << a << " from line: \"" << lineBuffer << "\"" << vcl_endl;
      }
    }
    else {
      int a=sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf), &(uncer));

      if (a!=9) {
        vcl_cout << "sdetd_load_edg_ascii: input error argument" << a << " from line: \"" << lineBuffer << "\"" << vcl_endl;
      }
    }

    //scale the edges
    x *= scale;
    y *= scale;

    ix = (int)(ix*scale);
    iy = (int)(iy*scale);

    if (bSubPixel){
      //create an edgel token
//      if (ix !=0 && iy !=0) //trustworthy coordinates
//        edge_map->insert(new sdet_edgel(vgl_point_2d<double>(x, y), dir, conf, 0.0, uncer), ix, iy);
//      else
        edge_map->insert(new sdet_edgel(vgl_point_2d<double>(x, y), dir, conf, 0.0, uncer));
    }
    else { //pixel edges  
      //create an edgel token
      edge_map->insert(new sdet_edgel(vgl_point_2d<double>(ix, iy), idir, iconf, 0.0, uncer));
    }
  }
  infp.close();

  return true;
}

#ifdef HAS_BOOST
//: Load .edg.gz file compressed with zlib, gzip style.
// TODO: use templating and/or istream inheritance to avoid duplicating almost
// identical code to bsold_load_cem_ascii
bool sdetd_load_edg_gzip(vcl_string input_file, bool bSubPixel, double scale, sdet_edgemap_sptr &edge_map)
{
  double x, y;
  char lineBuffer[1024];
  int numGeometry, w=0, h=0;
  int ix, iy;
  double idir, iconf, dir, conf, uncer=0;
  
  //1)If file open fails, return.
  vcl_ifstream infp_orig(input_file.c_str(), vcl_ios::in  | vcl_ios::binary);

  if (!infp_orig){
    vcl_cout << " Error opening file  " << input_file.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_istream infp;
  infp.push(boost::iostreams::gzip_decompressor());
  infp.push(infp_orig);

  bool file_valid = false;
  int ver = 0;

  //2)Read in the header block
  for (int j=0; j<9; j++){

    //read next line
    infp.getline(lineBuffer,1024);

    //read the line with the file type
    if (!vcl_strncmp(lineBuffer, "# EDGE_MAP", sizeof("# EDGE_MAP")-1)){
      file_valid = true;

      if (!vcl_strncmp(lineBuffer, "# EDGE_MAP v2.0", sizeof("# EDGE_MAP v2.0")-1))
        ver = 2;
      else if (!vcl_strncmp(lineBuffer, "# EDGE_MAP v3.0", sizeof("# EDGE_MAP v3.0")-1))
        ver = 3;
      else
        ver = 1;
      
      continue;
    }

    //ignore all other comment lines and empty lines
    if (vcl_strlen(lineBuffer)<2 || lineBuffer[0]=='#')
      continue;

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " WIDTH=", sizeof(" WIDTH=")-1)){
      sscanf(lineBuffer," WIDTH=%d",&(w));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "WIDTH=", sizeof("WIDTH=")-1)){
      sscanf(lineBuffer,"WIDTH=%d",&(w));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " HEIGHT=", sizeof(" HEIGHT=")-1)){
      sscanf(lineBuffer," HEIGHT=%d",&(h));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "HEIGHT=", sizeof("HEIGHT=")-1)){
      sscanf(lineBuffer,"HEIGHT=%d",&(h));
      continue;
    }

    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, " EDGE_COUNT=", sizeof(" EDGE_COUNT=")-1)){
      sscanf(lineBuffer," EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
    //read the line with the edge count info
    if (!vcl_strncmp(lineBuffer, "EDGE_COUNT=", sizeof("EDGE_COUNT=")-1)){
      sscanf(lineBuffer,"EDGE_COUNT=%d",&(numGeometry));
      continue;
    }
  }
  
  //make sure the file is valid
  if (!file_valid){
    vcl_cout << "Invalid .edg file (gzip)" << vcl_endl;
    return false;
  }

  // make sure w and h are reasonable
  if (w==0 && h==0) { w=1000; h=1000; }

  // edge_map 
  edge_map = new sdet_edgemap(static_cast<int>(w*scale), static_cast<int>(h*scale));

  edge_map->edgels.reserve(numGeometry);

  //read the data block
  while (infp.getline(lineBuffer,1024)) {

    //there are two variations of this file in existence
    if (ver<3){
      int a = 0;
      if (!vcl_strncmp(lineBuffer, "EDGE : ", sizeof("EDGE : ")-1))
      {
        a = sscanf(lineBuffer,"EDGE :  [%d, %d]    %lf %lf   [%lf, %lf]   %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf));
      }
      else
      {
        a = sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf));
      }
      if (a!=8) {
        vcl_cout << "sdetd_load_edg_ascii: input error argument" << a << " from line: \"" << lineBuffer << "\"" << vcl_endl;
      }
    }
    else
    {
      int a = sscanf(lineBuffer," [%d, %d]   %lf %lf  [%lf, %lf]  %lf %lf %lf",&(ix), &(iy),
          &(idir), &(iconf), &(x), &(y), &(dir), &(conf), &(uncer));
      if (a!=9) {
        vcl_cout << "sdetd_load_edg_ascii: input error argument" << a << " from line: \"" << lineBuffer << "\"" << vcl_endl;
      }
    }

    //scale the edges
    x *= scale;
    y *= scale;

    //XXX hack: recompute assuming pixel buckets, avoiding roundoff
    //inconsistencies, because we will need later on in other times go back from
    //x,y to buckets anyways, and if we read off from file we have roundoff
    //inconsistency.
    //
//    ix = sdetd_round(x);
//    iy = sdetd_round(y);

    if (bSubPixel){
      //create an edgel token
//      if (ix !=0 && iy !=0) //trustworthy coordinates
//        edge_map->insert(new sdet_edgel(vgl_point_2d<double>(x, y), dir, conf, 0.0, uncer), ix, iy);
//      else
        edge_map->insert(new sdet_edgel(vgl_point_2d<double>(x, y), dir, conf, 0.0, uncer));
    }
    else { //pixel edges  
      //create an edgel token
      edge_map->insert(new sdet_edgel(vgl_point_2d<double>(ix, iy), idir, iconf, 0.0, uncer));
    }
  }
  return true;
}
#endif


bool sdetd_save_edg_ascii(vcl_string filename, sdet_edgemap_sptr edgemap)
{
  //1) If file open fails, return.
  vcl_ofstream outfp(filename.c_str(), vcl_ios::out);

  if (!outfp){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  //2) write out the header block
  outfp << "# EDGE_MAP v3.0" << vcl_endl << vcl_endl;
  outfp << "# Format :  [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Strength Uncer" << vcl_endl;
  outfp << vcl_endl;
  outfp << "WIDTH=" << edgemap->width() << vcl_endl;
  outfp << "HEIGHT=" << edgemap->height() << vcl_endl;
  outfp << "EDGE_COUNT=" << edgemap->num_edgels()  << vcl_endl;
  outfp << vcl_endl << vcl_endl;

  //save the edgel tokens
  for (unsigned row=0; row<edgemap->edge_cells.rows(); row++){
    for (unsigned col=0; col<edgemap->edge_cells.cols(); col++){
      for (unsigned k=0; k<edgemap->edge_cells(row, col).size(); k++){

        sdet_edgel* edgel = edgemap->edge_cells(row, col)[k];

        double x = edgel->pt.x();
        double y = edgel->pt.y();

        unsigned ix = col; //preserve the original pixel assignment
        unsigned iy = row;
        
        double idir = edgel->tangent, iconf = edgel->strength;
        double dir= edgel->tangent, conf= edgel->strength, 
//               d2f=edgel->deriv, 
               uncer=edgel->uncertainty;

        outfp << "[" << ix << ", " << iy << "]    " << idir << " " << iconf << "   [" << x << ", " << y << "]   " << dir << " " << conf << " " << uncer << vcl_endl;
      }
    }
  }

  outfp.close();

  return true;
}

#ifdef HAS_BOOST
bool sdetd_save_edg_gzip(vcl_string filename, sdet_edgemap_sptr edgemap)
{
  //1) If file open fails, return.
  vcl_ofstream outfp_orig(filename.c_str(), vcl_ios::out  | vcl_ios::binary);

  if (!outfp_orig){
    vcl_cout << " Error opening file  " << filename.c_str() << vcl_endl;
    return false;
  }

  boost::iostreams::filtering_ostream outfp;
  outfp.push(boost::iostreams::gzip_compressor());
  outfp.push(outfp_orig);

  //2) write out the header block
  outfp << "# EDGE_MAP v3.0" << vcl_endl << vcl_endl;
  outfp << "# Format :  [Pixel_Pos]  Pixel_Dir Pixel_Conf  [Sub_Pixel_Pos] Sub_Pixel_Dir Strength Uncer" << vcl_endl;
  outfp << vcl_endl;
  outfp << "WIDTH=" << edgemap->width() << vcl_endl;
  outfp << "HEIGHT=" << edgemap->height() << vcl_endl;
  outfp << "EDGE_COUNT=" << edgemap->num_edgels()  << vcl_endl;
  outfp << vcl_endl << vcl_endl;

  //save the edgel tokens
  for (unsigned row=0; row<edgemap->edge_cells.rows(); row++){
    for (unsigned col=0; col<edgemap->edge_cells.cols(); col++){
      for (unsigned k=0; k<edgemap->edge_cells(row, col).size(); k++){

        sdet_edgel* edgel = edgemap->edge_cells(row, col)[k];

        double x = edgel->pt.x();
        double y = edgel->pt.y();

        unsigned ix = col; //preserve the original pixel assignment
        unsigned iy = row;
        
        double idir = edgel->tangent, iconf = edgel->strength;
        double dir= edgel->tangent, conf= edgel->strength, 
//               d2f=edgel->deriv, 
               uncer=edgel->uncertainty;

        outfp << "[" << ix << ", " << iy << "]    " << idir << " " << iconf << "   [" << x << ", " << y << "]   " << dir << " " << conf << " " << uncer << vcl_endl;
      }
    }
  }

  return true;
}
#endif
