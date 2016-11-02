# Some ideas to make it easier to work on shared VXL + VXD + internal Project

We call the superproject VPE: Vision Programming Environment.
It would be a way of working in tandem, to try to make some things easier for
people that use and modify both VXL and VXD at the source level.

The basic idea would be that the super repo would replicate a repo hierarchy
most VXD developers would have:
    VPE
      vxl
      vxd
      scripts
  
Here are some concrete ideas and Git experiments on how it could work out.

TL;DL : the conclusion is that:
- Our own scripting of a unified environment would work best
- In practice, the principle of always having the working repos synced to master
  is the new equivalent of making a tarball release. Striving to sync to master
  is much simpler than trying to share branches, though this may be needed for
  hacks and work arounds. Strive to fix 'master'! its a lot easier and imensely
  more useful than studying heavy strategies for propagating patches around.
- Submodules complicate the regular git workflow when we want to hack into the
  submodule.  It is well-defined, but requires a great deal of training. If
  people wouldn't hack into vxl all that much, and stay with top level changes,
  then it could work. But most people will hack away both vxl and vxd, and can't
  affor to lose changes. Submodules have some good ideas, though, such as
  vpe/ locking specific commits in vxl/ and vxd/ so that people that pull VPE get
  the same state in vxl/ and vxd/. Also, moving stuff across VPE/VXL/VXD doesn't
  really get tracked. Submodules are a monster whose overhead may pay off for
  projects requiring a huge number of repos for the developer to work on.
- Subtrees almost work, but still have problems with history which breaks
  regular git workflow. You simply can't even see the history inside vxl/ or
  vxd/ from vpe. But subtrees seem to be nice in that you can merge and track
  changes across the projects. This is very cool.
- Android repo tool doesn't have good documentation, and seems to demand that
  all sub repos be pulled from the same remote. It does bring some good ideas,
  such as having a manifest repository for meta-information of what repositories
  and branches the project uses/used for vxl/vxd etc at a particular commit.


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

## Making changes to VXL/VXD within VPE

### Checked out VPE fresh, made changes to VXL, Push upstream to VXL
When VPE is checked out, VXL's remote is not there. Add it if you want to
push/pull directly without passing through VPE

    git remote add ...

### VXL/VXD change flow
Edit vxl/ normally

    # eg:  echo '// test' >> vxl/CMakeLists.txt   # an existing VXL file is edited

    # if we want that change to be backported to VXL, we prepend TO VXL:
    git ci -am "FROM VPE: cmakelists"  # this message shows up on upstream

Keep doing other commits to anywhere in the tree.  When backporting, we have to
cherry-pick when the original team has made free commits anywhere in the tree.
If you yourself are working on the tree, and separate your commits to vxl/ and
vxd/ folders in separate branches merged to your master, this becomes a rebase
instead of cherrypicking (see similar approach 2 below).

    git checkout vxl-master
    # usually works: 
    # git cherry-pick -x --strategy=subtree master
    # safer and more consistent:
    git cherry-pick -x --strategy=subtree -Xsubtree=vxl/ master

    # check if that generates a commit with the wrong prefix, if so,
    # undo the commit by resetting HEAD and give up.

    # use '-e' flag to cherry-pick to edit the commit message before passing upstream

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
If you use the subtree variant below, it works a bit better.

    git log -- '*filename'          # from the toplevel

This views each commit that touched a file whose name ends with filename. It
won't follow actual renames, and may show false positive if you have several
files with the same basename [1].

## Caveats
- problem 1: history of subtree not visible with git log. This is under
  development by the Git team for `git log --follow`.

## Benefits
The toplevel VPE team can edit any submodule and propagate changes to VPE peers,
using regular git workflow. The vxl-master and vxd-master branches track where
we are at these subprojects. Once in a blue-moon, we push to the subprojects.

Worst case scenario:
If we need to heavily sync to the subproject-specific remotes, two cases:

1. Experienced users (1 or 2 people in team)
Pushing often to the subproject is infeasible
- We'll make wild commits in VPE on a branch, push to VPE regularly.
- Once in a good while, we carefully push to the subproject.

2. Collaborators which only care to know regular Git (most of team)
- Work normally on VPE and push changes done everywhere in the repo to VPE
- Never update submodule directly; rely on experienced users to do so.
- Pulling in an updated VXL necessarily relies on VPE remote having done so.
  They will never do this independently. They rarely do anyways. If they really
  want to do so, then they learn how to do it. Which means we make it difficult
  (but far from impossible) for them to diverge from the VPE team on which VXL
  or VXD is being used.

# VPE repository as scripts over regular Git
A script that builds the VPE environment for a beginner or collaborator.
Something very much along the lines of https://github.com/github/scripts-to-rule-them-all

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
- one could have a "share" script which would go inside a separate or father
  repo, look up the SHA1 and currently checked out branch of the checked-in
  repos, and upload that. This little repo or file would track people's history
  of branches, and it could be user-specific, just so other people know where
  you were when you comitted.

Advantages
- current people keep working as is, just adding VXD to the loop

# Submodules
- well supported
- bad: all users need to learn a lot of submodule lingo
- good: if the repo maintainer updates the sub-repos and pushes, users are
  locked to that sub-repo version.
  - If your peers do a mere git pull, it will pull-in the work VXL.
  - If your peers update vxl, everyone gets vxl
  - In that way, vxl is updated equally for everyone in the team,
  instead of each pulling a different vxl master. You sync to the team repo,
  rather than to the vxl team repo.
  (? can we track multiple versions of vxl?)
- in the simple use case of only editing and pushing to the container repo,
  naive users may not be aware vxl/ folders are not separate git repos
- basically the workflow has no base case for simple usage. It is always going
  to be complex
- advantage: much more well-defined and well-maintaned than subtrees if you
  know the lingo
- advantage: for advanced users it may provide a good way to share code in a
  predictable way, locking the subprojects to the commits that people are
  working at the moment.
  - separate repositories can achieve this by:
    - setting up a post-commit hook to write the commit SHA of the submodules to
      the parent repo. (basically, the .gitmodules is the actual good thing, to
      let users know what the maintainer is using).
    - the maintainer could have a script: mark-as-build, to populate a file with
      the id of the commit of the dependant repos plus the remote and branches being used (ouch).



##  Pulling in changes from VPE

    git spull   # alias or script. reminds me of the scripted superrepo approach

    git pull
    git submodule update     # always.. massive danger if you don’t, as 
                             # your next container commit will regress the submodule

#### If the submodule url changed, everyone has to:
    git pull
    git submodule sync
    git submodule update     # always.. massive danger if you don’t, as 
                             # your next container commit will regress the submodule

Spull alias cover all cases:

    git config --global alias.spull '!git pull && git submodule sync --recursive && git submodule update --init --recursive'

##  Pulling in changes from VXL/VXD

    cd vxl
    git fetch
    git merge origin/master
    cd .. 
    git diff --submodule 
    # you can see that the submodule was updated and get a list of commits that were added to it. 

    git commit -am
    git push
   

## Updating VXL/VXD within VPE

    # first, make sure update submodule master to remote master,
    # or, more generally, that you have a branch tracking the submodule

    cd vxl

    git status      # it will show detached state
    git checkout vxl-master     # if it doesn't exist, create it
    git merge origin/master

    # if you don't want to use that branch, create any other, as long as you are
    # inside the submodule, branches will be local.

    # Now make your changes and commit normally, within vxl/
    git push origin vxl-master

    cd ..

    # Part 2:  update VPE's repo to see if we have other branches or our own
    # vxl-master updated in VPE's repo
    git submodule update --remote --merge

    # If you forget the --rebase or --merge, Git will just update the submodule
    # to whatever is on the server and reset your project to a detached HEAD state.
    # Then you have to checkout vxl-master again and redo the work

    # on the toplevel, you won't use git push, but first
    git push --recurse-submodules=check

    



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

## Pulling in changes from VXL/VXD

    # do it in steps to make sure whats going on
    git fetch vxl
    git checkout -b optional-branch  # useful if you're fetching non-master branch
    git merge -s recursive optional-branch-to-merge -Xsubtree=vxl vxl/master    # vxl/anybranch
    git checkout master
    git merge feature-in-progress

    # you could also just merge directly!
    git fetch
    git merge -s recursive -Xsubtree=vxl vxl/master

## Updating the remote
    git fetch vxl
    git checkout -b vxl-integration vxl/master
    # merge changes from master using subtree
    git cherry-pick -x --strategy=subtree -Xsubtree=vxl/ master

    # Or, if you organized your VXL commits directly into eg vxl-integration, just rebase

    git branch master-reb master # master or any other branch tip to rebase
    git rebase -s subtree -Xsubtree=vxl --onto vxl-integration feature-in-progress master-reb
    git checkout vxl-integration
    git merge master-reb
    git push  # to push the vxl-master branch to toplevel VPE
    git push vxl HEAD:master
    git branch -D master-reb

    Rebase is nice, since rebasing interactively means that you have a chance to
    edit the commits which are rebased (inserting move-related info such as the
    origin sha1 etc). You can reorder the commits, and you can
    remove them (weeding out bad or otherwise unwanted patches).

### Conclusion
  - same as before

## Conclusion

The 

- Caveat: git log --follow doesn't work for sub folders to trace down to the orig repo

- mere users only push to main repo, but will never ever push directly to the
  subrepos, unless they are advanced users. this may be a good thing, as
  day-to-day workflow gets centralized and synced a lot easier (one repo) than
  before.

# Submodules

Submodules differ from subtrees:
- submodules are separate git repositories inside a a bigger one, yet are not
  managed as usual git repositories within the bigger one. 
  - git log works nicely, with separate histories for each
- subtrees look more like a subfolder uniformly managed by git, but with tiny annoying quirks

- for submodules, collaborators won't automatically see updates to submodules—if
  you update a submodule, you may need to remind your colleagues to run git
  submodule update or they will likely see odd behavior.


For VPE, submodules are not attractive since:
- people's usual git workflow is broken
- commits made to sub projects may be lost.

# Android Repo tool



# Links

[1] https://git-scm.com/book/en/v2/Git-Tools-Advanced-Merging
[2] https://medium.com/@porteneuve/mastering-git-subtrees-943d29a798ec#.sjbirxm4y
[3] https://saintgimp.org/author/saintgimp/
[4] http://paste.ubuntu.com/11732805/ (tested, didn't work)
[5] http://stackoverflow.com/questions/10918244/git-subtree-without-squash-view-log/40349121#40349121

# TODO
- always upgrade git. use > 2
- vpe: my folder is full of crap, clone again.


