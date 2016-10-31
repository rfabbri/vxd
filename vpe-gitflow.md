# Adding subtrees

We're in VPE, and want to add VXD and VXL

## Adding plugin example

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
    git cherry-pick -x --strategy=subtree master
    #
    # check if that generates a commit with the wrong prefix, if so,
    # undo the commit by resetting HEAD and use:
    # git cherry-pick -x --strategy=subtree -Xsubtree=vxl/ master

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
    git push
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

## VXD -> VXL file move within VPE
    Works, just integrate the commit to VXD and VXD as the above directions for making
    any other type of change.


## VPE -> VXL/VXD file move within VPE

  

# Remove VXD or any subtree

It’s just a directory in your repo. A good ol’ git rm will do.

    git rm -r vxl
    git commit -m "Removing vxl subtree"
    git remote rm vxl

    # perhaps optimzie git / todo: test tree size
    git gc  # repo still huge
    git gc --prune=now --aggressive  # till, 103M!

# Links

- https://git-scm.com/book/en/v2/Git-Tools-Advanced-Merging
- https://medium.com/@porteneuve/mastering-git-subtrees-943d29a798ec#.sjbirxm4y
- https://saintgimp.org/author/saintgimp/
- http://paste.ubuntu.com/11732805/

# TODO
- always upgrade git. use > 2
- vpe: my folder is full of crap, clone again.

# Caveats
- problem 1: history of subtree not visible with git log. This is under
  development by the Git team for `git log --follow`.
