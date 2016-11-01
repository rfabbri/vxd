# Some ideas to make it easier to work on shared VXL + VXD + internal Project

We call the superproject VPE: Vision Programming Environment.
It would be a way of working in tandem, to try to make some things easier for
people that use and modify both VXL and VXD at the source level.

Here are some concrete ideas and Git experiments on how it could work out.


# Idea: shareable super repository VPE by adding subtrees

We're in VPE, and want to add VXD and VXL

## Adding plugin example from [2], for reference

    git remote add plugin ../remotes/plugin
    git fetch plugin
    git read-tree --prefix=vendor/plugins/demo -u plugin/master
    git commit -m "Added demo plugin subtree in vendor/plugins/demo"

## Adding VXL to VPE

    git remote add vxl ../remotes/vxl
    git fetch vxl
    git read-tree --prefix=vxl -u vxl/master # an alternative would be -u vxl-master
    git commit -m "Added vxl subtree in vxl/"
    git checkout -b vxl-master vxl/master  # keeps vxl history across git clones
    git push origin vxl-master

## Adding VXD to VPE
    git remote add vxd ../remotes/vxd
    git fetch vxd
    git read-tree --prefix=vxd -u vxd/master
    git commit -m "Added vxd subtree in vxd/"
    git checkout -b vxd-master vxd/master  # keeps vxd history across git clones
    git push origin vxl-master

## Pulling in changes from VXL

    git fetch vxl
    git merge -s subtree vxl/master  # this will join histories
    # git merge -X subtree=vxl/ vxl/master  # if the above goes crazy
    # git merge -s subtree --squash vxl/master # this will not join, untraceable
    # git commit -m "FROM VXL: update"
    git checkout vxl-master  # just to make it point to the right place
    git merge vxl/master
    git checkout master
    git push origin vxl-master

## Pulling in changes from VXD

    git fetch vxd
    git merge -s subtree vxd/master  # this will join histories
    # git merge -X subtree=vxd/ vxd/master  # if the above goes crazy
    # git merge -s subtree --squash vxd/master # this will not join, untraceable
    # git commit -m "FROM VXD: update"
    git checkout vxd-master  # just to make it point to the right place
    git merge vxd/master
    git checkout master
    git push origin vxd-master

## Checked out VPE fresh, made changes to VXL, Push upstream to VXL

When VPE is checked out, VXL's remote is not there. Add it if you want to
push/pull directly without passing through VPE

    git remote add ...

## Making changes to VXL/VXD within VPE


Edit vxl/ normally

    # eg:  echo '// test' >> vxl/CMakeLists.txt   # an existing VXL file is edited

    # if we want that change to be backported to VXL, we prepend TO VXL:
    git ci -am "FROM VPE: cmakelists"  # this message shows up on upstream

Keep doing other commits to anywhere in the tree.  When backporting, we have to
cherry-pick when the original team has made free commits anywhere in the tree.
If you yourself are working on the tree, and separate your commits to vxl/ and
vxd/ folders in separate branches merged to your master, this becomes a merge
instead of cherrypicking.

    git checkout vxl-master
    # usually works: 
    # git cherry-pick -x --strategy=subtree master
    # safer and more consistent:
    git cherry-pick -x --strategy=subtree -Xsubtree=vxl/ master

    # check if that generates a commit with the wrong prefix, if so,
    # undo the commit by resetting HEAD and use:
    # git cherry-pick -x --strategy=subtree -Xsubtree=vxl/ master

    # use '-e' flag to cherry-pick to edit the commit message befor passing upstream

    # --strategy=subtree (-s means something else in cherry-pick) also helps to make sure
    # changes outside of the subtree (elsewhere in container code) will get quietly
    # ignored. 

    # if merging, use the same strategies.
    #
    # the -x in these commands annotate the commit message with the SHA1 of VPE
    # Use the following to make sure files outside of the subtree (elsewhere in container code) 
    # will get quietly ignored. This may be useful when cherypicking a rename, since move is rm+add

    git push  # to push the vxl-master branch
    git push vxl HEAD:master

### Original example on which the above is based, showing the kinds of edits
    echo '// Now super fast' >> vendor/plugins/demo/lib/index.js
    git ci -am "[To backport] Faster plugin"
    date >> main-file-1
    git ci -am "Container-only work"
    date >> vendor/plugins/demo/fake-work
    date >> main-file-2
    git ci -am "[To backport] Timestamping (requires container tweaks)"
    echo '// Container-specific' >> vendor/plugins/demo/lib/index.js
    git ci -am "Container-specific plugin update"

## VXL/VXD -> VPE file move within VPE

    Works, just integrate the commit to VXL/VXD as the above directions for making
    any other type of change.

    History tracing:

## VPE -> VXL/VXD file move within VPE
    Works, just integrate the commit to VXD as the above directions for making
    any other type of change.

    History tracing:

## VXD -> VXL file move within VPE
    Works, just integrate the commit to VXD and VXL as the above directions for making
    any other type of change.

    History tracing:
  

## Remove VXD or any subtree

It’s just a directory in your repo. A good ol’ git rm will do.

    git rm -r vxl
    git commit -m "Removing vxl subtree"
    git remote rm vxl

    # perhaps optimzie git / todo: test tree size
    git gc  # repo still huge
    git gc --prune=now --aggressive  # till, 103M!


## History lookup

    git log --follow filename

This will show all logs, including renames and moves. It doesn't work
across subtrees, but is being fixed by the Git team [5].

    git log -- '*filename'          # from the toplevel

This views each commit that touched a file whose name ends with filename. It
won't follow actual renames, and may show false positive if you have several
files with the same basename [1].

## Links

[1] https://git-scm.com/book/en/v2/Git-Tools-Advanced-Merging
[2] https://medium.com/@porteneuve/mastering-git-subtrees-943d29a798ec#.sjbirxm4y
[3] https://saintgimp.org/author/saintgimp/
[4] http://paste.ubuntu.com/11732805/
[5] http://stackoverflow.com/questions/10918244/git-subtree-without-squash-view-log/40349121#40349121

## TODO
- always upgrade git. use > 2
- vpe: my folder is full of crap, clone again.

## Caveats
- problem 1: history of subtree not visible with git log. This is under
  development by the Git team for `git log --follow`.

# VPE repository as scripts over regular Git
A script that builds the VPE environment for a beginner or collaborator.

- creates everything in a toplevel vpe/ folder
- pulls in all repos, including recommended utility ones (utils-macambira and boost sources if need be)
- mkdir all bin 
- does an apt-get or port install for common packages
- install common sh aliases and git aliases
- moves between repos are history-tracked only by being disciplined and including the
  origin SHA1 in every cherrypick or merge
- have a bin/ or scripts/ subfolder for scripts
- to share your dev environment:
  - integrate and push to master in each, or:
  - push each to a separate branch in person's github for each
  - write in the vpe-build script or config file the corresponding branches
  - send the script to the person.

Advantages
- current people keep working as is, just adding VXD to the loop

# Submodules
They seem weird and cumbersome, and no cross-history.
Why do people use them, eg, in libreoffice?

# Subtree variant
This follows link [5]'s alternative in the response.

## Adding vxl/vxd to vpe

    git init
    > deleteme.txt
    git add .
    git commit -m 'initial dummy commit'

    git remote add -f vxl ../remotes/vxl
    # tested with git 2.10
    git merge --allow-unrelated-histories vxl/master

    git rm deleteme.txt
    git commit -m 'cleaning up initial file'
    mkdir vxl
    git mv `ls|grep -v vxl` vxl
    git commit -m 'moved vxl to vxl/ folder'

    # do the same for vxd
    git remote add -f vxd ../remotes/vxd
    # tested with git 2.10
    git merge --allow-unrelated-histories vxd/master
    mkdir vxd
    git mv `ls|grep -v vxd | grep -v vxl` vxd
    git commit -m 'moved vxd to vxd/ folder'

    # histories look perfect

## Merging changes from vxl/vxd, keeping it up to date

    # do it in steps to make sure whats going on
    git fetch vxl
    git checkout -b optional-branch  # useful if you're fetching non-master branch
    git merge -s optional-branch-to-merge -Xsubtree=vxl vxl/master    # vxl/anybranch
    git checkout master
    git merge feature-in-progress

    # you could also just merge directly!
    git fetch
    git merge -s recursive -Xsubtree=vxl vxl/master

## Updating the remote

## Caveats
- git log --follow doesn't work for sub folders to trace down to the orig repo
