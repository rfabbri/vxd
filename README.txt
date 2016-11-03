<img src="vxd-logo.gif" width="215" height="84" />


## Introduction: What is VXD?

VXD (the Vision-something-Development libraries) is a collection of development/experimental
C++ libraries for computer vision based on [VXL](http://vxl.sourceforge.net).

VXD is a collection of code that builds against VXL but doesn't contain VXL.
However, a higher level repository is maintained alongside this project, called
[VPE](http://github.com/rfabbri/vpe) (Vision Programming Environment), which
comes with reference copies of VXL and VXD, making things simpler for
common use cases, such as distributing your project codebase and for tracking changes
across VXL and VXD simultaneously. The average user can just work off VPE as a toplevel
project, but one can always use VXD and VXL as separate git repositories.

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
step prior to pushing code to lean VXL. 

VXD is to be a common repository that
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
BRL is itself recursively layered similarly to VXL core, but each library has a 'b' prefix:

1. brl/bbas: basic libraries, analogous to vxl/core (e.g., bil, bvgl)
2. other brl/b\* : higher level libraries  (eg, brl/bseg/sdet for edge detection)

```	
Sublayers of BRL under VXL Level 3:   3.1             3.2              3.3         3.4
                                      bbas/bil       bbas/bil_algo    seg/sdet   seg/sdet/algo
                                      bbas/bvgl      bbas/bvgl_algo   b*l        b*l_pro
                                      bbas/b*l_io    bbas/b*l_io
                                      bbas/b*l_pro
```
For instance, whenever someone adds something to bvgl, and it matures, it may
be moved to core/vgl.

In practice, in the more complex world of contrib/,
libraries within the same layer may sometimes call siblings in the same layer.
As a rule of thumb, lower layers must not depend on higher layers to compile.
The rules relax as you go up the layers, in practice.

This translates in to the following compile workflow:

- Configure VXL using CMake
- Don't build everything, but only what you need. Start by typing 'make' to
  build only the higher level libraries you need. For instance, if I only needed edge
  detection, I'd only type 'make' inside 'brl/sdet'
- If compile fails, just backtrack along the layers. First get the lowest
  level libraries you need to build, then move on to compiling the higher ones:
  - Build core. If that fails, say, in vgl, first build vgl without vgl\_algo.
    You can do this by building vgl/tests, which won't depend on vgl/algo, then move
    to vgl_algo.
  - If core builds, then go to brl/bbas and type 'make'. If that doesn't work, 
  backtrack within that. If it works, then go to seg/sdet. Proceed in the same way.

This process of isolating higher level bugs and changes from lower level ones, enabling you
to build code in stages, is enabled by the layering strategy of VXL.

contrib/ libraries follow the same general idea, although each may have their idiosyncrasies.

##### Examples:

Can brl/bbas/bvgl call oxl/vdgl? No. 

Can brl/bbas/bvgl\_algo call oxl/vdgl? Usually, yes. People won't think much
about layers here; they work as if an '\*\_algo' library can depend on anything, if
no cycle happens. In later stages of development people get more selective.

### Layers across repositories: VXL, VXD and internal code

How can we organize external code to maximize consistency with VXL?
Beyond VXL, there is layering within VXD, and layering within the internal
library collection of a team, referred here as "Internal". Within each of these
there may be different policies, but the basic idea remains the same as
described above. We give an example of a team as Brown University, but this
could serve as any other team working with VXL.

In VXL/contrib/brl/bbas, for instance, there is BNL for contributions to VNL.  
Teams may have, in turn, an internal or closed-source version of BNL, which is called DBNL
in the case of LEMSVXL, Brown's internal library.

In VXD, the team may wish to publish somethings in between. For instance, VXD
may contain VNL development changes that are open-sourced, but under a more
laidback policy than VXL (more so than BNL and VNL), and at the same time
slightly more strict or standardized than the Internal repositories for which anything is valid
(after all, open-sourcing by itself is a big step towards higher quality).

Since the internal libs at Brown already prepend a 'd', as in 'dbnl', VXD
*appends* a 'd', like 'bnld' and 'vnld'.  As a general rule, VXD appends a \*d to only the
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
| VXD            | core                      | vnld        | vnld\_algo | vxd/core/vxd/vnld/
| VXD contrib    | BRL/bbas                  | bnld        | bnld\_algo | vxd/contrib/brl/bbas/bnld/
| Internal       | BRL/basic (team-specific) | dbnl        | dbnl\_algo | internal/basic/dbnl/

One sensible way to differentiate between vxd/core/vxd/vnld/ and vxd/contrib/brl/bbas/bnld/ 
is to designate bnld for team-specific changes, while vnld is more commonly
developed code. But vnld doesn't have to exist, if only team-specific code is
needed. Whether it is useful to have a common vnld will depend on whether we
want a subset of the code to be independently compilable and linkable. Layering
is only needed to tame different levels of development intensity, isolating
potential mistakes and management overhead.

Note that an internal app may use any library, or all vnl variants
simultaneously.  The VXD and Internal trees are to be constructed as if VXL, VXD
and Internal were co-existing in the same file tree, without conflict, except
that in reality we have three separately managed repositories.  VXD could be
thought as a folder under VXL, ie,  vxl/contrib/vxd, but managed in a separate
repository and recursively having similar structure as VXL.  These rules
can also be relaxed for more unstable portions of VXD and Internal code.

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
In practice, vsold may not exist, unless any of these occur: either the GE team
itself wanted its own vsol development library in VXD, or the VXD team wants to
use contrib/gel/vsold as a common library to start merging all teams vsol
changes. If only the Brown team is publishing development changes to VSOL, then only bsold will likely exist.
But the Brown team may also want to push code directly to VXL, bypassing the bsold folder.
VXD enters the picture only if pushing directly to VXL is less convenient.

So the process of placing VNL-related code, for instance, would be along three repository layers.

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

We strive to to make everything in VXD always consistent with VXL while
apending a 'd' to the lib names. This is a good idea since it is a tiny but
good initial step towards making internal private code available in VXL: you
first name it and place it properly, following the VXL hierarchy and
principles, then you mature the code and one day it may make it to the
corresponding folder in VXL. Making a new library name and path correspond to a
hypothetical place in VXL in a clear way is the first requirement to place code
in the main part of VXD, while there are secondary folders designated for incubating
non-conforming code and for storing obsolete code that may be useful.

Includes for both vxl vsol and vxd vsold can be done simultaneously, and would look like
```
#include <vsol/vsol_some_class.h>
#include <vsold/vsold_some_class.h>
```

In CMake, we'd have
```
include_directories( ${VXL_GEL_INCLUDE_DIR} )
include_directories( ${VXD_GEL_INCLUDE_DIR} )
```

There are no conflicts, even if both libraries are loosely maintained and duplicate a lot of code.

### Misc. Workflow Remarks

- VXL, VXD and the internal VXL-related libraries usually conform to VXL's philosophy and
  coding standards, including CMake, but are by no means limited to VXL. Many use other third
  party computer vision libraries such as OpenCV, Boost, etc.

### Migrating code across the separate layers and repositories

All is well under the sun, until code starts being modified in one version of a
library and gets out of sync with the others. Not to mention that each
repository can have multiple git branches in addition to layers.

#### Can we improve the following scenario?

_"Here, try our new vision system, all you have to do is use VXL branch D, VXD
branch X and Internal branch Y, set all these CMake variables
exactly as I say, and use the same operating system, with the same installed
libraries. To make your VXL-based code available to us, there's this little
README in VXD..."_

Syncronizing code is crucial in order to work collaboratively at different levels, 
as well as for consolidating versions done by a single person for higher quality
and scalability. Moving code from one layer (or any sub-layering) to a more
strict layer is called "promoting" code, and is key in the VXL approach to code
quality and maintenance.  Git helps merging between branches, although this can
get messy if care is not taken. However, Git does not automate syncing between
layers across different repositories, although it helps organize things.
Each team must have discipline to sync their code against various level of
common branches and repositories, and to remove any duplicates.

Questions arise:

1. **Layers across different repositories:** How are we to keep track of
   code changes in a feasible way, if separate repositories are not under a
   single Git repository? How do we track variants of the same library (eg, vnl,
   bnl, bnld, dbnl, etc)?

2. **History across different repositories:** If code gets promoted from a certain layer
   to a more strict layer, across different repositories, is history lost (git
   log)?  Should we migrate history?

3. **Cleanup after Promoting:** Should we delete code from the original layers or repositories when promoting
   or merging? Or should we keep multiple instances around in some cases?

4. **Where to work from:** When starting a new project, making new changes, where do
   we work from? Layering offers a guide for each library change, but, overall, we
   have to pick where to develop the project from. Do we create branches of VXL
   and work off that? Do we create a branch off VXD and work off that? Or do we
   create a private repository dependent on VXD and VXL? Do we use VXL and VXD
   as git submodules, or track them separately?

5. **Maintaining variants of code across repositories:** Suppose you are working
   on a new vision system, in an Internal repository.  You then decide to
   publish your changes in VXD, and then some stable subset of these changes
   gets promoted to VXL.  You now have versions of your system in all three
   repositories, Internal, VXD, and VXL.  Where are you going to work from? If
   your code in VXD is largely redundant with what you have in Internal, and has
   been slightly improved to VXL standards, should you abandon Internal forever
   and work off a private branch in VXD?

   Should you choose to stay with Internal, say, for management purposes. Then
   the vision system code that has a largely redundant public and private
   version starts diverging.  Then you have to track and merge changes back and
   forth VXD and Internal by hand, since they will have different names and are
   under different repositories.  Say goodbye to the wonders of Git. Is this
   feasible?

6. **I/O compatibility across repos:** should I/O be compatible between VXL, VXD
   and Internal variants, eg, vnl, bnl, bnld, dbnl? GUI rendering compatibility?
   Is this necessary?

A solution of copying code around, cleaning up duplicate code, and keeping track
of changes by hand seems to be the only one available, taking advantage of
layered organization to make the problem more well defined. Still, it is
fundamentally a manual solution.

Some ideas on having a development environment repository to help keep
everything in place for a team are given in [VPE orVision Programming
Environment)](http://github.com/rfabbri/vpe). 

#### Example case: small scale

At Brown we have a state of the art edge detector and contour grouper as basis for many
systems. It is an example of small/medium scale code across VXL layers, representative
of many situations. Larger scale systems stress these considerations to another
level, and will be analyzed in the next section.

This is currently in both VXL and LEMSVXL (Brown's Internal repository):

    vxl/contrib/brl/bseg/sdet
    lemsvxl/seg/dbdet

The version in VXL has changes from LEMSVXL up to june 2014.  After publishing
these changes in VXL, the team decided to keep performing experimental research
changes in the private LEMSVXL, keeping the VXL version only for widely usable
code. Meanwhile, the VXL team has hardened the corresponding VXL code by fixing
warnings and improving overall code quality.

Changes between two codes are manually tracked.

Do they actually get tracked? The answer is, rarely. In practice, if a major
research change in the internal repository becomes usable enough, one has to
manually port it back to the public VXL code, while changing the names etc.
This is seldom done, but may be feasible if a team is very actively working
on a feature, and really cares about pushing code out. But the team has to
maintain two code bases and the really active one, which everyone would want to
work on and maintain, remains private.

The team at Brown has proposed the following policy to maintain the private LEMS
(dbdet) and the public VXL (sdet) versions of the edge extraction technology:

- As sdet and dbdet diverge in the future, the interested parties manage
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
    versions of the code, so the team can detect significant changes and API
    compatibility.

- **GUI and I/O:** Processes in dbdet/pro for sdet are added in LEMSVXL, so that a
  LEMSVXL GUI can have both dbdet and sdet versions, the latter being suffixed
  with 'vxl'.  In order to avoid rewriting visualizers / tableaux renderers for
  sdet\_edgemap and other structures, the VXL version of the processes will
  *convert* the structures to dbdet versions on the fly. That is, we will not
  create sdet versions of dbdet_edgemap_storage, dbdet_sel_storage,
  dbdet_edgemap_tableau, etc, for now.

- The team encourages new users to use the more stable sdet library
  unless there is significant experimental functionality in dbdet to be used
  (and there is).  However, in practice everyone with access to the internal
  repository has just used dbdet.

- The main changes between sdet and dbdet have carried out by hand, eg, analyzing
API differences and inserting appropriate research changes from dbdet in a case
by case fashion.
- some useful pre and post-processing that was present in the symbolic edge linker
  process from dbdet/pro were put inside a dbpro-independent class sdet_symbolic_edge_linker class.
- certain internal depencencies to lems that was generally useful were
    promoted  to VXL, namely:
    - brl/bbas/bvgl/bvgl_param_curve, bvgl_eulerspiral, bvgl_biarc

Guidelines
- **Cleanup and harden the original code**: When promoting code, try to remove whats left
  behind and update the original codebase to use as much as possible of the new
  code. That way you minimize the parts of the original code that can diverge,
  and it keeps benefiting from a hardened basis.
- **Sync to master frequently**: Syncing to master is today's equivalent of
  making a release. You should strive to keep all builds clean against the
  master branches of all involved repositories.

#### Example case: large scale

Does the manual strategy used by the above team scale to larger systems?

If there is a large system in a private repository, can we feasibly track
changes back and forth between the public and private versions?
  - Guideline: manually annotate your move commits with the source and
    destination SHA1, in all repositories.
  - Using a super-repo [VPE] may help.

##### Some considerations

Here are some of the issues with a VXL programming environment, from experience.

- *What are the potential problems with VXL or C++ programming in general?*
    - *Dependencies?* Having your code mingled with other libraries and
      repositories which get out of sync with our peers? 

- **Building stand-alone code for sharing:** If we need to build binaries,
  just work with the VXL team to improve this aspect, since thi is common
  build infrastructure.  But both VXL and
  a completely stand alone edge linking C++ library can coexist, though manually
  maintained.  After all, its all C++ and the effort of writing one way or
  another does not surpass the real difficulty which is the project itself.

- **Not an issue: VXL helps scaling up vision systems.** In the real world the edge
  detector/linker will be only one vision module among many. The code needs to
  be well organized and follow good programming practices to be able to scale
  and be part of much bigger vision systems. Programming on an internal
  repository based on VXL often looks messy to average developers, demanding
  tracking of dependencies which quickly get out of sync with their peers. But this
  problem does not mean they should give up VXL programming and
  throw away the core technology which actually helps their code scale, but
  they should build on it and tackle the real problems.

- **Not an issue: Build/Compile** VXL is C++ with CMake which is a leading industry standard to get
  your C++ code to build and compile in other platforms. Leveraging this
  existing infrastructure helps your code also build in other platforms,
  plus the VXL community helps in solving compile bugs etc. No large-scale vision
  code in C++ will be easy to compile. If you want users to download and run your code,
  you need to package it for each system. This is not java.

- **Not an issue: Too much C++** If you use the VXL philosophy the right way, it
  will only benefit you and make things easier to compile and share. Not
  everything needs to be in C++/VXL, but only core technology. The rest is
  probably more effectively developed in Matlab, Scilab, etc.

- **Repositories of sync** Some internal groups end up having multiple versions of VXL around, because
  they don't keep up with the main VXL.  Code sharing becomes a hard
  process. The issue, however, is a matter of discipline and having active
  team members to make internal code build against VXL master.

- **Git branching** Git branching is also a big discipline problem, where code
  diverges and each person doesn't sync their branches and end up with
  incompatible programming environments, only sharing a similar culture but out
  of sync.

## Download and install

- [License](https://sourceforge.net/p/vxl/git/ci/master/tree/core/vxl_copyright.h)

## Problems and where to get help

The [VXL mailing lists](http://sourceforge.net/mail/?group_id=24293) are at SourceForge. You can find list descriptions, subscription information, and archives there. [vxl-users](http://lists.sourceforge.net/lists/listinfo/vxl-users) is the best list for bug reports, etc. Please read the [vxl-users list policy](http://vxl.sourceforge.net/vxl-users-policy.html) before posting.

## Dashboard

The dashboards (at [cdash.org](http://www.cdash.org/CDash/index.php?project=vxl) and at [rpi.edu](http://www.cs.rpi.edu/research/groups/vxl/Testing/Dashboard/MostRecentResults-Nightly/Dashboard.html)) are a summary (collated from different machines at different sites in different timezones) of the current build status of VXL. More builds are welcome; just follow the [instructions for submitting a build](http://vxl.sourceforge.net/submit-build.html).

## Reporting bugs

If you think you have found a bug, or if you have an installation or build problem, please submit a bug report on the [vxl-users mailing list](http://lists.sourceforge.net/lists/listinfo/vxl-users). Please check the [vxl-users list policy](http://vxl.sourceforge.net/vxl-users-policy.html) to find out what information will be helpful in sorting out your bug.

Github issues are also used.

## Maintainers
[Ric Fabbri](http://rfabbri.github.io) is currently the principal mainainer,
but please keep all discussion to the mailing lists.

## Citing VXD

If you are using the multiview curve stereo code, please cite

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
## What is VXL?
VXL (the Vision-something-Libraries) is a collection of C++ libraries designed for computer vision research and implementation. It was created from TargetJr and the IUE with the aim of making a light, fast and consistent system. VXL is written in ANSI/ISO C++ and is designed to be portable over many platforms. The core libraries in VXL are:

- vnl (numerics): Numerical containers and algorithms. e.g. matrices, vectors, decompositions, optimisers.
- vil (imaging): Loading, saving and manipulating images in many common file formats, including very large images.
- vgl (geometry): Geometry for points, curves and other elementary objects in 1, 2 or 3 dimensions.
- vsl (streaming I/O), vbl (basic templates), vul (utilities): Miscellaneous platform-independent functionality.

As well as the core libraries, there are libraries covering numerical algorithms, image processing, co-ordinate systems, camera geometry, stereo, video manipulation, structure recovery from motion, probability modelling, GUI design, classification, robust estimation, feature tracking, topology, structure manipulation, 3d imaging, and much more.

Each core library is lightweight, and can be used without reference to the other core libraries. Similarly, the non-core libraries don't depend on more than absolutely necessary, so you can compile and link just the libraries you really need.

For more details, see the [VXL website](http://vxl.sf.net)


# Disclaimer: VXD is under development. The stable part is kept in VXL.
