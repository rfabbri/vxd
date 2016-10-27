<img src="vxd-logo.gif" width="215" height="84" />


## Introduction: What is VXD?

VXD (the Vision-something-Development libraries) is a collection of development/experimental
C++ libraries for computer vision and image understanding based on [VXL](http://vxl.sourceforge.net).
It is a collection of code that builds against VXL but doesn't contain VXL.

VXL (the Vision-something-Libraries) is a collection of C++ libraries designed
for computer vision research and implementation.  VXL is kept lean and stable by
the community, to be used by several companies, universities, and other libraries, so VXD has
been created to cater for less stable, more experimental code based on VXL while
closely mirroring its layout.

The idea is to release large (and possibly clunky) computer vision systems in
VXD, which can slowly mature to one day make it into VXL or even VXL core. Some
"big" code by nature don't even have to ever make it to VXL strict standards or
granularity, but can still be very useful.

The idea came up because most research and industry labs having internal code
that use VXL do not have an intermediate, common and more laidback open source
step prior to pushing code to lean VXL. So VXD is to be a common repository that
can be seen as closely coordinated with VXL and that can keep up (at least
partially) with VXL changes and be built on official dashboards. 

VXD is stratified into core/basic, larger development libraries, contrib folder
with multiple people and/or entities, and so on.  The VXD team maintains at
least basic libraries conforming to the latest vxl at all times. VXD is easier
to contribute to, and is less strict (thus messier) than VXL in terms of code
stability.


## Developer Workflows

A typical setup will have three VXL codebases:  VXL, VXD and an internal
version, which we refer to as "Internal" in this discussion (e.g., LEMSVXL at Brown).
The workflow and organization of these libraries follow from the basic layering
principles of VXL, reviewed below. 

This section is not required to compile and use VXD.
Only developers, contributors and maintainers should read this.

### Basics of VXL layering

In VXL, we have basically two top levels of code maturity:

1. core: for very stable code of wide interest. Examples: core/vgl, core/vnl
2. contrib: for less stable or more specialized code (but still stable).
  Examples: contrib/brl or "Brown Libraries" for Brown University contributions,
  contrib/gel or "General Electrics Libraries", etc. 
  

Both core and contrib have been very closely maintained by the community using
high standards, while keeping it as lean as possible,
to reduce maintenance burden and to serve as a reliable basis for other software systems.

Moreover (from the VXL website): Each core library is lightweight, and can be used without
reference to the other core libraries. Similarly, the non-core libraries don't
depend on more than absolutely necessary, so you can compile and link just the
libraries you really need.

Within each top layer, there are other layering rules.

Within core, we have this figure from the VXL book:

```	
Level:   0         1         2          3
         vcl       vbl       vnl_algo   v*l (eg, contrib)
                   vnl       vil_algo
                   vil       vgl_algo
                   vgl       v*l_io
                   vul
                   vsl
```
Libraries in one layer may not call their siblings, even if this means code copying.
In practice, the above layering rules is served as reference for more
complex libraries outside core, for which one doesn't always have to be so
strict, say, about level 3 contrib libraries calling their siblings. 

Code in contrib/ is usually intended to one day be part of core, and is
roughly laid out for this purpose. To be promoted to core is a tendency used as an
organizing principle, even if this almost never happens. 


### VXL layering outside core

Let us examine the example of contrib/brl.  It is a set of libraries inside VXL designed by Brown University
researchers to hold open source versions of their research code. 
BRL is itself layered similar to VXL, but each library has a 'b' prefix:

1. brl/bbas: basic libraries, analogous to vxl/core (eg, bil, bvgl)
2. brl/b\* : higher level libraries  (eg, brl/bseg/sdet for edge detection)

```	
Sublayers of BRL under VXL Level 3:   3.1             3.2              3.3          3.4
                                      bbas/bil       bbas/bil        seg/sdet     seg/sdet/algo
                                      bbas/bvgl      bbas/bvgl_algo   b*l         b*l_pro
                                      bbas/b*l_io    bbas/b*l_io
                                      bbas/b*l_pro
```
For instance, whenever someone adds something in to bvgl, and it matures, it may
be moved to core/vgl.

In practice, in the more complex world of contrib/,
libraries within the same layer may sometimes call siblings in the same layer.
As a rule of thumb, lower layers must not depend on higher layers to compile.
The rule relaxes as you go up the layers.

This translates in the following compile workflow:

- Configure VXL using CMake
- Don't build everything, but only what you need. Start by typing 'make' to
  build only the higher level libraries you need. For instance, if I only needed edge
  detection, I'd only type 'make' inside 'brl/sdet'
- If compile fails, just backtrack along the layers. First build the lowest
  level libraries you need, then move on to compiling the higher ones:
  - Build core. If that fails, say, in vgl, first build vgl without vgl\_algo.
    I do this by building vgl/tests, which won't depend on vgl/algo, then I move
    to vgl_algo.
  - If core builds, then go to brl/bbas and type 'make'. If that doesn't work, 
  backtrack within that. If it works, then go to seg/sdet. Proceed in the same way.

This process of isolating higher level bugs from lower level ones, enabling you
to build code in stages, is enabled by the layering strategy of VXL.

contrib/ libraries follow the same general idea, although each may have their idiosyncrasies.

##### Examples:

Can brl/bbas/bvgl call oxl/vdgl? No. 

Can brl/bbas/bvgl\_algo call oxl/vdgl? Usually, yes. People won't think much
about layers here; an '\*\_algo' library can depend on anything, if no cycle happens.

### Layers across VXL, VXD and internal code

How can we organize external code to maximize consistency with VXL?
Beyond VXL, there is layering within VXD, and layering within the internal
library collection of a team, referred here as "Internal". Within each of these
there may be different policies, but the basic idea remains the same as
described above. We give an example of a team as Brown University, but this
could serve as any other team working with VXL.

In VXL/contrib/brl/bbas, for instance, there is BNL for contributions to VNL.  
Teams may have, in turn, an internal or closed-source version of BNL, which is called DBNL
in the case of LEMSVXL, Brown's internal library.

In VXD, the team may wish to publish something in between. For instance, VXD
may contain VNL development changes that are open-sourced, but under a more
laidback policy than VXL (more so than BNL and VNL), and at the same time
slightly more strict than the Internal repositories for which anything is valid
(after all, open-sourcing by itself is a step towards higher quality).

Since the internal libs at Brown already prepend a 'd', as in 'dbnl', VXD
*appends* a 'd', like 'bnld'.  As a general rule, VXD appends a \*d to only the
last path/library name. For instance: vxl/contrib/brl/bbas/libname would map to
vxd/contrib/brl/bbas/libnamed (same lib, ending with a \*d) as possible.
vxl/contrib/gel/vsol would map to vxd/contrib/gel/vsold, etc.
VXD will only have libraries that actually have public code that
is not ready or reviewed for VXL-quality yet. If we have development code to be
open sourced, we create the \*d libraries as needed. 

The layering for VNL variants may look like (from lower to higher layers):

| Overall Layer  | Sub layer                 |  Sub layer  | Sub layer  | Path reflecting all layers
| -------------- | ------------------------- | ----------- | ---------  | ----------------------
| VXL            | core                      | vnl         | vnl\_algo  | vxl/core/vnl/
| VXL contrib    | BRL/bbas                  | bnl         | bnl\_algo  | vxl/contrib/brl/bbas/bnl/
| VXD contrib    | BRL/bbas                  | bnld        | bnld\_algo | vxd/contrib/brl/bbas/bnld/
| Internal       | BRL/basic (team-specific) | dbnl        | dbnl\_algo | internal/basic/dbnl/

Note that an internal app may use any library, or all vnl variants simultaneously.
The VXD and Internal trees should be constructed as if VXL, VXD and Internal
were co-existing in the same file tree, without conflict, except that in reality we have
three separately managed repositories.

As another example, the layering for contrib/gel/vsol (which does not exist in
Core in any form, but variants exist in many contrib folders) may look like:

| Overall Layer  | Sub layer                 |  Sub layer  | Sub layer   | Path reflecting all layers
| -------------- | ------------------------- | ----------- | ---------   | ----------------------
| VXL contrib    | GEL                       | vsol        |  -none-     | vxl/contrib/gel/vsol/
| VXL contrib    | BRL                       | bsol        |  -none-     | vxl/contrib/brl/bbas/bsol/
| VXD contrib    | GEL                       | vsold       |  -none-     | vxd/contrib/gel/vsold/
| VXD contrib    | BRL/bbas                  | bsold       | bsold\_algo | vxd/contrib/brl/bbas/bsold/
| Internal       | BRL/basic (team-specific) | dbsol       | dbsol\_algo | internal/basic/dbnl/

In this case, VXD splits contributions to VSOL into two possible layers: vxd/gel/vsold and brl/bbas/bsold.
In practice, vsold may not exist, unless the GE team itself wanted its own vsol development library in VXD.
If only the Brown team is publishing development changes to VSOL, then only bsold may exist.
But the Brown team may also want to push code directly to VXL, bypassing the bsold folder.
VXD enters the picture only if pushing directly to VXL is less convenient.

So the process of placing code would be along three inter-repository layers

1. VXL: core/vnl for stable vnl, and contrib/bbas/bnl for stable additions from Brown
2. VXD: BRL/bbas/bnld for less stable additions from Brown, open sourced
3. Closed-sourced code improving bnl (as in LEMSVXL): basic/dbnl or anything, really.

This naming convention is important as we want to keep many versions of the same
library around and compiling, one in VXL, one in VXD and another one in Internal.
The idea is that VXD can build at the same time as VXL and private VXL-dependent
libraries from other companies/universities, without conflict, even if this
means some code duplication (each library version having potentially similar
code but a different maturity level). So it is a parallel hierarchy as
much as possible, with different naming.  


Includes for vxd/contrib/gel/vsold would look like
```
#include <vsold/vsold_some_class.h>
```

In CMake, we'd have
```
include_directories( ${VXD_GEL_INCLUDE_DIR} )
```

We strive to to make everything in VXD always consistent with VXL while
apending a 'd' to the lib names. This is a good idea since it is a tiny but
good initial step towards making internal private code available in VXL: you
first name it and place it properly, following the VXL hierarchy and
principles, then you mature the code and one day it may make it to the
corresponding folder in VXL. Making a new library name and path correspond to a
hypothetical place in VXL in a clear way is the first requirement to place code
in the main part of VXD, while there are secondary folders designated for incubating
non-conforming code and for storing obsolete code that may be useful.

### Misc. Workflow Remarks

- VXL, VXD and the internal VXL-related libraries usually conform to VXL's philosophy and
  coding standards, including CMake, but are by no means limited to VXL. Many use other third
  party computer vision libraries such as OpenCV, Boost, etc.

### Migrading code across the separate layers and repositories

All is well under the sun, until code starts being modified in one version of a
library and gets out of sync with the others. Moving code from one layer (or
any sub-layering) to a more strict layer is called "promoting" code, and is key 
in the VXL approach to code quality and maintenance.

Questions arise:

1. How are we to keep track of code changes in a feasible way, if separate
  repositories are not under a single Git repository for tracking different
  libraries (eg, vnl, bnl, bnld, dbnl, etc)?

2. If code gets promoted from a certain layer to a more strict layer, 
  across different repositories, is history lost (git log)?  Should we migrate history?

3. Should we delete code from the original layers when promoting it? Or should we keep multiple instances around?

4. When starting a new project or new changes, where do we work from? Do we alter all layers in parallel?
   Do we create a branch off VXL and work off that? Do we create a branch off VXD and work off that? 
   Do we fork VXL, fork VXD? Or do we create a private repository?

5. Suppose you are working on a new vision system, in an Internal repository.
   You then decide to publish your changes in VXD, and then some stable subset of
   these changes gets promoted to VXL.  You now have versions of your system in
   all three repositories, Internal, VXD, and VXL.  Where are you going to work
   from? If your code in VXD is largely redundant with what you have in
   Internal, and has been slightly improved to VXL standards, should you
   abandon Internal forever and work off a private branch in VXD?

   Should you choose to stay with Internal, code starts diverging.  Then you
   have to track and merge changes back and forth VXD and Internal by hand,
   since they will have different names and are under different repositories.
   Say goodbye to the wonders of Git. Is this feasible?

6. I/O compatibility between, eg, vnl, bnl, bnld, dbnl? GUI rendering
   compatibility? Is this necessary?

A solution of copying code around and keeping track of changes by hand seems to
be the only one available, taking advantage of layered organization to make the
problem more well defined. Still, it is a manual solution.

#### Example case: small scale

At Brown we have a state of the art edge detector and contour grouper as basis for many
systems. It is an example of small/medium scale code across VXL layers, representative
of many situations. Larger scale systems stress these considerations and will be
analyzed in the next section.

This is currently in VXL and LEMSVXL (Brown's Internal repository):

vxl/contrib/brl/bseg/sdet

lemsvxl/seg/dbdet

The version in VXL changes from LEMSVXL up to june 2014.  After publishing
these changes in VXL, the team decided to keep performing experimental research
changes in the private LEMSVXL, keeping the VXL version only for the usable changes.
Meanwhile, the VXL team has hardened the corresponding VXL code by fixing warnings
and improving overall code quality.

Changes between two codes are manually tracked.

Do they get tracked? The answer is, rarely. In practice, if a major research change in the internal repository
becomes usable enough, one has to manually port it back to the public VXL code, while changing the names etc.
This is is seldom done, but may be feasible if a team is very actively working on a feature, and is worried about pushing code out. But the team has to maintain two code bases and the really active one, which everyone would want to work on and maintain, remains private.

The team at Brown has propose the following policy to maintain the private LEMS (dbdet) and the public VXL (sdet)
versions of the edge extraction technology:

- Both sdet and dbdet will diverge in the future. The interested parties manage
  this as follows:
  - Improvements made to sdet are typically of the incremental kind which may
    introduce cleanups and API improvements. By monitoring these changes in git
    we can backport them to dbdet manually, when needed. (One must be really
    interested in the code for this to work)
  - Improvements made to dbdet are typically experimental, so by monitoring
    the internal code we know when something new works well and should
    be promoted to sdet
  - Both of these situations require a closely monitored and hand-crafted
    maintanence approach which is only made feasible since the stable sdet code
    is useful on its own (even if the cross-merging of changes doesn't get
    carried out); these are not two git branches but are separate repositories
    tracking similar code.
  - cross-library tests in dbdet/algo/tests compare the sdet and dbdet
    versions of the code, so we can detect significant changes and API
    compatibility.

- GUI and I/O: Processes in dbdet/pro for sdet are added in LEMSVXL, so that a
  LEMSVXL GUI can have both dbdet and sdet versions, the latter being suffixed
  with 'vxl'.  In order to avoid rewriting visualizers / tableaux renderers for
  sdet\_edgemap and other structures, the VXL version of the processes will
  *convert* the structures to dbdet versions on the fly. That is, we will not
  create sdet versions of dbdet_edgemap_storage, dbdet_sel_storage,
  dbdet_edgemap_tableau, etc, for now.

- The team encourages new users to use the more stable sdet library
  unless there is significant experimental functionality in dbdet to be used.
  However, in practice everyone with access to the internal repository has just used
  dbdet.

- The main changes between sdet and dbdet have carried out by hand, eg, analyzing
API differences and inserting appropriate research changes from dbdet in a case
by case fashion.
- some useful pre and post-processing that was present in the symbolic edge linker
  process from dbdet/pro were put inside a dbpro-independent class sdet_symbolic_edge_linker class.
- certain internal depencencies to lems that was generally useful were
    promoted  to VXL, namely:
    - brl/bbas/bvgl/bvgl_param_curve, bvgl_eulerspiral, bvgl_biarc

#### Example case: large scale

Does the manual strategy used by the above team scale to larger systems?

If there is a large system in a private repository, can we feasibly track
changes back and forth between the public and private versions?
Is the benefit of open sourcing only to make a huge effort to publish code so
others can use, but the team itself doesn't use it, but only tracks it? The team
has now two code bases to maintain, even if the changes are incremental and easy
to merge by hand.

##### Some considerations

Here are some of the issues with a VXL programming environment, from experience.

- What are the potential problems with VXL? Dependencies? Having your code
  mingled with many other libraries? In a large system I don't see a way out,
  and at least VXL is layered.

- Building stand-alone code for sharing: working on a VXL contrib
  library is pretty stand alone for a developer.  If we need to build binaries,
  just work with VXL to improve this aspect. Otherwise lots of common build
  infrastructure will have to be coded again and again.  But both VXL and a
  completely stand alone edge linking C++ library can coexist, though manually
  maintained.  After all, its all C++ and the effort of writing one way or
  another does not surpass the real difficulty which is coming up with good
  methods anyways.

- We want our code to scale up. In the real world the edge detector/linker will
  be only one vision module in many. The code needs to be well organized and
  follow certain good programming practices to be able to scale and be part of
  much bigger vision systems. The situation with an internal repository based on
  VXL often looks messy with a lot of dependencies, but that's just we having to
  adapt, *not* throw away our core technology which actually helps us
  scale, but build on it.

- VXL is C++ with CMake which is a leading industry standard to get
  your C++ code to build and compile in other platforms. Leveraging this
  existing infrastructure helps your code also build in other platforms,
  plus the VXL community helps in solving compile bugs etc. No large-scale vision
  code will be easy to compile.

- If you use the VXL philosophy the right way, it will only benefit you and make
  things easier to compile and share. Not everything needs to be in C++/VXL,
  but only core technology. The rest is probably more effectively developed in
  Matlab, Scilab, etc.

- Some internal groups end up having multiple versions of VXL around, because
  they don't keep up with the main VXL.  Code sharing becomes a hard
  process. The issue, however, is a matter of discipline and having active
  team members to make internal code build against VXL master.

- Git branching is also a big discipline problem, where code diverges and each
  person doesn't sync their branches and end up with incompatible programming
  environments, only sharing a similar culture but out of sync.



#### Todo

Some things to think about

- How do people actually manage changes across multiple Git repositories? Do we really have to do it all by hand?


## What is VXL?
VXL (the Vision-something-Libraries) is a collection of C++ libraries designed for computer vision research and implementation. It was created from TargetJr and the IUE with the aim of making a light, fast and consistent system. VXL is written in ANSI/ISO C++ and is designed to be portable over many platforms. The core libraries in VXL are:

- vnl (numerics): Numerical containers and algorithms. e.g. matrices, vectors, decompositions, optimisers.
- vil (imaging): Loading, saving and manipulating images in many common file formats, including very large images.
- vgl (geometry): Geometry for points, curves and other elementary objects in 1, 2 or 3 dimensions.
- vsl (streaming I/O), vbl (basic templates), vul (utilities): Miscellaneous platform-independent functionality.

As well as the core libraries, there are libraries covering numerical algorithms, image processing, co-ordinate systems, camera geometry, stereo, video manipulation, structure recovery from motion, probability modelling, GUI design, classification, robust estimation, feature tracking, topology, structure manipulation, 3d imaging, and much more.

Each core library is lightweight, and can be used without reference to the other core libraries. Similarly, the non-core libraries don't depend on more than absolutely necessary, so you can compile and link just the libraries you really need.

For a more detailed description of the libraries see the [VXL book](http://public.kitware.com/vxl/doc/release/books/core/book.html).

VXL is developed and used by an [international](http://vxl.sourceforge.net/developers.html) team from academia and industry, including some of the world's leading computer vision experts.

## Documentation
Documentation on the whole of VXL
There are two main references for information on the whole of VXL:

- The VXL package documentation includes auto-generated descriptions of every class and function in VXL.
    - [Latest release](http://public.kitware.com/vxl/doc/release/index.html).
    - Current development version (Git snapshot) (hosted at [Kitware](http://public.kitware.com/vxl/doc/development/index.html), [Brown](http://www.lems.brown.edu/vision/vxl_doc/html/index.html) and [Manchester](http://paine.wiau.man.ac.uk/pub/doc_vxl/) ).
- The VXL book is a single reference for all high-level documentation on VXL.
    - [Latest release](http://public.kitware.com/vxl/doc/release/books/core/book.html).
    - [Current development version (Git snapshot)](http://public.kitware.com/vxl/doc/development/books/core/book.html).

You can also [download](http://sourceforge.net/projects/vxl/) the release 1.17 documentation.

## VXL guides
These are short guides to specific areas in VXL:

- xcv is an application built to demonstrate the functionality of the VXL libraries.
    - There is a [guide to using xcv](http://vxl.sourceforge.net/manuals/xcv/newxcv.html) (also available in [postscript format](http://vxl.sourceforge.net/manuals/xcv.ps.gz)).
    - Download [xcv for Windows](http://vxl.sourceforge.net/manuals/xcv.zip), [xcv for solaris](http://vxl.sourceforge.net/manuals/xcv-SunOS5.bin.gz), and [xcv for linux](http://vxl.sourceforge.net/manuals/xcv-linux.bin.gz)
    - Here is some [example data](http://vxl.sourceforge.net/manuals/xcvdata.tar.gz) suitable for use with xcv.

- vgui is the VXL graphical user interface library.
    - For a simple introduction to using VGUI there is the [VGUI users guide](http://vxl.sourceforge.net/manuals/vgui_users_guide/index.html) (also in [postscript format](http://vxl.sourceforge.net/manuals/vgui_users_guide.ps.gz)). Here is the [example code](http://vxl.sourceforge.net/manuals/examples-tutorial.tar.gz) used in the guide.
    - The [VGUI reference manual](http://vxl.sourceforge.net/manuals/vgui_ref_manual/index.html) (also in [postscript format](http://vxl.sourceforge.net/manuals/vgui_ref_manual.ps.gz)) is designed for developers of VGUI.

xcv and vgui have not changed a lot since the last release of VXL so these guides should be useful for both the release and development versions of VXL.

- The [MUL book](http://public.kitware.com/vxl/doc/release/books/contrib/mul/book.html) is an overview of libraries concerned with probability modelling, classification, and 3d and registered images.

## Download and install

- [VXL License](https://sourceforge.net/p/vxl/git/ci/master/tree/core/vxl_copyright.h)

You can download the latest official release vxl-1.17

- [Download VXL-1.17](http://sourceforge.net/projects/vxl/)
- [Installing VXL-1.17 version](http://vxl.sourceforge.net/releases/install-release.html)

Or get the latest development version from the Git repository at SourceForge.

- [VXL Git Repository Instructions](https://sourceforge.net/p/vxl/wiki/Git/)
- [Installing latest development version of VXL](http://vxl.sourceforge.net/releases/install-latest.html)

## Problems and where to get help
### FAQs

There is a [vxl-users FAQ](http://vxl.sourceforge.net/vxl-users-faq.html).

There are other FAQs in the [VXL book](http://public.kitware.com/vxl/doc/release/books/core/book.html).

### Mailing list

The [VXL mailing lists](http://sourceforge.net/mail/?group_id=24293) are at SourceForge. You can find list descriptions, subscription information, and archives there. [vxl-users](http://lists.sourceforge.net/lists/listinfo/vxl-users) is the best list for bug reports, etc. Please read the [vxl-users list policy](http://vxl.sourceforge.net/vxl-users-policy.html) before posting.

### Dashboard

The dashboards (at [cdash.org](http://www.cdash.org/CDash/index.php?project=vxl) and at [rpi.edu](http://www.cs.rpi.edu/research/groups/vxl/Testing/Dashboard/MostRecentResults-Nightly/Dashboard.html)) are a summary (collated from different machines at different sites in different timezones) of the current build status of VXL. More builds are welcome; just follow the [instructions for submitting a build](http://vxl.sourceforge.net/submit-build.html).

### Reporting bugs to the mailing list

If you think you have found a bug, or if you have an installation or build problem, please submit a bug report on the [vxl-users mailing list](http://lists.sourceforge.net/lists/listinfo/vxl-users). Please check the [vxl-users list policy](http://vxl.sourceforge.net/vxl-users-policy.html) to find out what information will be helpful in sorting out your bug.



<!--





This is a realistic professional-grade high-quality archviz dataset with full 3D ground
truth, including curve and hard edge ground truth, control over illumination and
ground truth camera models for three video sequences under different conditions.

Images and explanations of this ground truth are provided in:
http://Multiview-3d-Drawing.sf.net

The ground truth curves were manually marked, and are represented as dense
samples traced on the original meshes, together with local connectivity
information (ie, which points are linked to the other). Note that global
orientation for the curves is not available. Think of the 3D ground-truth curves
as an undirected graph. After all, they are subsets of a mesh which also doesn't
have global orientation or global connectivity information.


## Files

```
images/{midday,night,sunset}   100 640x360 video frames for each illumination condition

cameras/   3x4 cameras for each video frame, in text format

3d/   blender.org files with the full 3D ground truth

3d/original   the original professional files with editable 3D objects and
  render settings. Open 3d/original/3d/pavillon_barcelone_v1.2.blend in Blender.

3d/full/pavillon_barcelone_v1.2-ungrouped02-separate_datablocks-no_modifiers-fixed-010.blend
  .blend file with the 3D curves browseable in Blender

3d/full/pavillon_barcelone_v1.2-ungrouped02-separate_datablocks-no_modifiers-fixed-010.dae
  .dae (collada) XML with the curve objects (points/edges) corresponding to the
  above .blend. You can open the .blend file in Blender, right-click a curve to
  get its name (shown in the top right, eg. Cube.030), then search this XML file
  for the object.
  
3d/full/pavillon_barcelone_v1.2-ungrouped02-separate_datablocks-no_modifiers-fixed-010-overlay.blend
  the ground truth curves overlayed on top of the original 3D models, for checking
  
3d/full/pavillon_barcelone_v1.2-full_mesh_for_PLY_export-ungrouped-all_modifiers-joined-001.ply.bz2
  scene in one full (huge) dense mesh PLY file to be opened in Meshlab, etc. No
  semantic info, as all objects were joined in Blender into one prior to export.
  Useful for computing ground truth error. The mesh is huge, make sure to have a
  very powerful computer.

3d/full/pavillon_barcelone_v1.2-cam-baked-007.blend
  The file used to render the chair scenes. Open the file in blender and click
  on the timeline to see the video preview on the top-right window. To see the
  camera path, change the scene to sunset.

3d/cropped/pavillon_barcelone_v1.2-ungrouped02-separate_datablocks-no_modifiers-fixed-bounding_box-no_edges-010-002.blend
  cropped version of the scene curves in near a bounding box, to reduce compute load

3d/cropped/pavillon_barcelone_v1.2-ungrouped02-separate_datablocks-no_modifiers-fixed-bounding_box-010-002.blend
  open in blender to see a depiction of the bounding box used to generate the cropped vesion above

3d/src/parse_collada.sce   code to parse the .da XML files and produce global 3D points.

3d/cropped/{gt-points.txt,gt-edges.txt}  all points and local connectivity
  information for the marked curves, in txt format for the cropped scene. For
  the full scene, you have to generate these .txt files yourself from the .dae,
  see below for more information.

  gt-points.txt   x y z coordinates for sample points, one point per line.
                  lets call this the 'allpts' nx3 array

  gt-edges.txt    indices of points that are connected by a vertex.
                  The txt file seems to have floats, but they are ints.
                  Connect a 3D point if allpts( alledg(i-1:i) , :).
                  See the end of parse_collada.sce for an example of how to
                  plot the ground truth in scilab/matlab.
```

## Getting 3D points and edge info using the .dae Collada file

To get 3D points and connectivitiy information, use the Scilab.org script
src/parse_collada.sce (matlab is similar, but just download the free Scilab and
run this script once).  We provide these for the cropped version of the
dataset only, but you can easily generate them for the full dataset.

First edit parse_collada.sce and put the name of the .dae file in the "d"
variable. Then open scilab and type "exec path/to/parse_collada.sce", adjusting the
path acoordingly.

The script will output two ASCII files with the curve information:
  /tmp/gt-points.txt  x y z coordinates for sample points, one point per line.
                      lets call this the 'allpts' nx3 array
  /tmp/gt-edges.txt   indices of points that are connected by a vertex.
                      The txt file seems to have floats, but they are ints.
                      Connect a 3D point if allpts( alledg(i-1:i) , :).
                      See the end of parse_collada.sce for an example of how to
                      plot the ground truth in scilab/matlab.

When using the XML .dae COLLADA file in a custom way, you will have to transform
from object coordinates to world coordinates prior to using the cameras. 
The code below illustrates how to do this transform in Matlab/Scilab:

```matlab
%------------------------
% Auxiliary function to convert from euler angles to 3x3 matrix
function mat = eul_to_mat3(eul)
	ci = cos(eul(1));
	cj = cos(eul(2));
	ch = cos(eul(3));
	si = sin(eul(1));
	sj = sin(eul(2));
	sh = sin(eul(3));
	cc = ci * ch;
	cs = ci * sh;
	sc = si * ch;
	ss = si * sh;

	mat(1)(1) = (cj * ch);
	mat(2)(1) = (sj * sc - cs);
	mat(3)(1) = (sj * cc + ss);
	mat(1)(2) = (cj * sh);
	mat(2)(2) = (sj * ss + cc);
	mat(3)(2) = (sj * cs - sc);
	mat(1)(3) = -sj;
	mat(2)(3) = (cj * si);
	mat(3)(3) = (cj * ci);

%------------------------
% For example, the .dae has the object Curve_000,
% with the following transforms:
%       <translate sid="location">1.166548 -1.320713 2.108471</translate>
%       <rotate sid="rotationZ">0 0 1 -46.07418</rotate>
%        <rotate sid="rotationY">0 1 0 -67.88113</rotate>
%        <rotate sid="rotationX">1 0 0 -0.7029412</rotate>
%        <scale sid="scale">0.136427 0.1467155 0.1364271</scale>
% These are mapped in matlab/scilab to:

loc = [1.166548 -1.320713 2.108471]'
Rz=[0 0 1 -46.07418]
Ry=[0 1 0 -67.88113]
Rx=[1 0 0 -0.7029412]
scale=diag([0.136427 0.1467155 0.1364271])

% Generate the object rotation matrix
R = eul_to_mat3([Rx(4), Ry(4), Rz(4)]*%pi/180)'

% Assuming the N object points as 3xN vector 'po',
% Tranform to world coordinates like this:
p = R*scale*po + loc*ones(1,size(po,2)) 

% Then you can project p using the 3x4 P matrix from *.projmatrix as usual.
```

## Generating the PLY of the scene as one huge mesh

* open the original .blend
* select the objects you don't want to clutter the scene and delete them: trees,
  cameras and lights (right mouse button to select -> x -> delete)
* select all objects
* unlink all groups to make the meshes direct: Ctrl-Shift-A 
* select all objects
* apply all modifyers: Alt-C
* join all meshes into one: Ctrl-J (for some reason pressing f first
helped.. unsure)
* export PLY

## Generating your own video sequence with camera ground truth

You can generate another video sequence or re-render at higher resolution to
generate your own ground truth.   Beware! Rendering these videos can take
*several* days. Use multple threads, use your GPU if it has more than 2GB memory
and you can also set up a blender render farm for distributed rendering in a
network. This is all basically required in case you need higher resolution
videos.

Open the file 3d/full/pavillon_barcelone_*-cam-baked-*.blend (stars mean widlcard)
in Blender. Then, to the right there will be a pane box with multiple tabs with
icons. Click on the tab with the camera icon. Then under "Dimensions" set your
render resolution (50% was used for the chair sequence). Under "Performance",
tune the number of threads, play with the tile size. Then under "Render", click
the Animation button. It will render the entire video.

Ask [Ric Fabbri](http://rfabbri.github.io) for further details.


## Version

Dataset produced and tested with Blender 2.76 on Mac OSX and Linux.

## Authors

[Ricardo Fabbri](http://rfabbri.github.io) built the dataset.
Further authors include Anil Usumezbas and Benjamin Kimia.

## Citing the dataset

Please cite the dataset as

```bibtex
@inproceedings{Usumezbas:Fabbri:Kimia:ECCV16,
    Author         = {Anil Usumezbas and Ricardo Fabbri and Benjamin B. Kimia},
    Booktitle      = {Proceedings of the European Conference on Computer Vision},
    Crossref       = {ECCV2016},
    Title          = {From multiview image curves to {3D} drawings},
    Year           = {2016}
}

@proceedings{ECCV2016,
  title     = {Computer Vision - ECCV 2016, 14th European Conference on
               Computer Vision, Amsterdam, Netherlands, October 8-16,
               2016, Proceedings},
  booktitle = {14th European Conference on
               Computer Vision, Amsterdam, Netherlands, October 8-16,
               2016},
  publisher = {Springer},
  series    = {Lecture Notes in Computer Science},
  year      = {2016}
}
```

## Credits

This dataset was built on top of E-Mirage studios's dataset downloaded from
https://www.blender.org/download/demo-files, for which the unmodified version
is distributed under the 3d/original folder.

We also acknowledge FAPERJ/Brazil and NSF support.

## Links

More Blender demo files that can serve as ground truth www.blender.org/download/demo-files

E-Mirage Studios designer Hamza Cheggour www.emirage.org 


Ric Fabbri's posts on how to convert to/from Blender cameras to standard 3x4
format for your own projects with blender:
* http://blender.stackexchange.com/questions/38009/3x4-camera-matrix-from-blender-camera
* http://blender.stackexchange.com/questions/40650/blender-camera-from-3x4-matrix

Images and explanations of this ground truth are provided in:
http://Multiview-3d-Drawing.sf.net

-->

# Disclaimer: VXD is under development
