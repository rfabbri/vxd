use strict;
use File::Find ();
use File::Copy ();

# Set the variable $File::Find::dont_use_nlink if you're using AFS,
# since AFS cheats.


sub convert {
    my ($src, $dest)= @_;
    open(OUTFILE,">$dest") or die "Can't open $src";
    open(INFILE,"<$src") or die "Can't open $dest";

    while (<INFILE>)
    {
# CMake
# Match either a trailing space or parenthesis with leading space
s!\b\ vidpro\ !\ vidpro2\ !g;
s!\b\ vidpro\)!\ vidpro2\)!g;
s!\b\ bpro\ !\ bpro2\ !g;
s!\b\ bpro\)!\ bpro2\)!g;
s!\b\ bvis\ !\ bvis2\ !g;
s!\b\ bvis\)!\ bvis2\)!g;
# Match either a trailing space or parenthesis with leading parenthesis
s!\b\(vidpro\ !\(vidpro2\ !g;
s!\b\(vidpro\)!\(vidpro2\)!g;
s!\b\(bpro\ !\(bpro2\ !g;
s!\b\(bpro\)!\(bpro2\)!g;
s!\b\(bvis\ !\(bvis2\ !g;
s!\b\(bvis\)!\(bvis2\)!g;

# Includes
s!bvis\/!bvis2\/!g;
s!bpro\/!bpro2\/!g;
s!vidpro\/!vidpro2\/!g;

# Classes
s!\bvidpro_!vidpro2_!g;
s!\bbpro_!bpro2_!g;
s!\bbvis_!bvis2_!g;

        print OUTFILE;
    }
    close OUTFILE;
    close INFILE;

}


sub mentions_breye {
    my ($filename)= ($_);
    open(INFILE,"<$filename") or die "Can't open $filename";

    while (<INFILE>)
    {
        if (/\bbpro_/ || /\bbvis_/ ||/\bvidpro_/)
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
    if ($filename =~ /\~/) {return;}
    if ($filename =~ /\#/) {return;}
# conditionally replace breye->breye2 naming    
    if (mentions_breye($filename))
    {
        print "breye in $filename\n";
#	$backup = "$filename.old";
#        rename $filename, $backup or die "Unable to create backup $backup";
#        convert $backup, $filename;
    }
}


print "Searching $ARGV[0]\n";
# Traverse desired filesystems
File::Find::find({ wanted => \&wanted, follow => 0, no_chdir => 1 } , $ARGV[0]);
exit;
