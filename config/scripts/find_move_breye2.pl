#!/bin/sh
# -*- perl -*-
exec perl -w -x $0 ${1+"$@"}
#!perl
#line 6
# If Windows barfs at line 3 here, you will need to run perl -x this_file.pl
# You can set up as a permanent file association using the following commands
#  >assoc .pl-PerlScript
#  >ftype PerlScript=Perl=C:\Perl\bin\Perl.exe -x "%1" %*


# usage: convertvil2tovil.run_me_second.pl file_or_directory_name
# Modifies files so as to deal with vil2 to vil renaming.
# Only run this script after running convertviltovil1.run_me_first.pl

use strict;
use File::Find ();
use File::Copy ();

# Set the variable $File::Find::dont_use_nlink if you're using AFS,
# since AFS cheats.


sub mentions_breye {
    my ($filename)= ($_);
    open(INFILE,"<$filename") or die "Can't open $filename";

    while (<INFILE>)
    {
        if ( /\bbpro2/ || /\bbvis2/ ||/\bvidpro2/ || /\bdbvis2/ || /\bdbvidpro2/ )
        {
          close INFILE;
          return 1;
        }
    }
    close INFILE;
    return 0;
}



sub wanted {
    my ($filename) = $File::Find::name;
    my $backup;
    my $scr;
    if (! -f $filename)  {return;}
    if ($filename =~ /CVS/) {return;}
    if ($filename =~ /\.old/) {return;}
    if ($filename =~ /\.txt/) {return;}
    if ($filename =~ /\~/) {return;}
    if ($filename =~ /\#/) {return;}
# conditionally replace breye->breye2 naming    
    if (mentions_breye($filename))
    {
        print " $filename before\n";
	$backup = "$filename";
	$backup =~ s/bpro2_/bpro_/;
	$backup =~ s/bvis2_/bvis_/;
	$backup =~ s/vidpro2_/vidpro_/;
	$backup =~ s/dbvidpro2_/dbvidpro_/;
	$backup =~ s/dbvis2_/dbvis_/;
	$backup =~ s/vorl2_/vorl_/;
	$backup =~ s/vorl2\./vorl\./;
	print " $backup after\n";
	rename $filename, $backup or die "rename failed";
#	$backup = "$filename.old";
#        rename $filename, $backup or die "Unable to create backup $backup";
#        convert $backup, $filename;
    }
}


print "Searching $ARGV[0]\n";
# Traverse desired filesystems
File::Find::find({ wanted => \&wanted, follow => 0, no_chdir => 1 } , $ARGV[0]);
exit;
