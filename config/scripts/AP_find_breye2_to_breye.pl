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
s!\ vidpro2\ !\ vidpro\ !g;
s!\b\ vidpro2\ !\ vidpro\ !g;
s!\b\ vidpro2\)!\ vidpro\)!g;
s!\ dbvidpro2\ !\ dbvidpro\ !g;
s!\b\ dbvidpro2\ !\ dbvidpro\ !g;
s!\b\ dbvidpro2\)!\ dbvidpro\)!g;

s!\b\ bpro2\ !\ bpro\ !g;
s!\ bpro2\ !\ bpro\ !g;
s!\b\ bpro2\)!\ bpro\)!g;
s!\ bvis2\ !\ bvis\ !g;
s!\b\ bvis2\ !\ bvis\ !g;
s!\b\ bvis2\)!\ bvis\)!g;
s!\ dbvis2\ !\ dbvis\ !g;
s!\b\ dbvis2\ !\ dbvis\ !g;
s!\b\ dbvis2\)!\ dbvis\)!g;
s!\ vorl2\ !\ vorl\ !g;
s!\b\ vorl2\ !\ vorl\ !g;
s!\b\ vorl2\)!\ vorl\)!g;

# Match either a trailing space or parenthesis with leading parenthesis
s!\b\(vidpro2\ !\(vidpro\ !g;
s!\b\(vidpro2\)!\(vidpro\)!g;
s!\b\(dbvidpro2\ !\(dbvidpro\ !g;
s!\b\(dbvidpro2\)!\(dbvidpro\)!g;

s!\b\(bpro2\ !\(bpro\ !g;
s!\b\(bpro2\)!\(bpro\)!g;
s!\b\(bvis2\ !\(bvis\ !g;
s!\b\(bvis2\)!\(bvis\)!g;
s!\b\(dbvis2\)!\(dbvis\)!g;
s!\b\(vorl2\)!\(vorl\)!g;

s!vorl2\.!vorl\.!g;
# Includes
s!bvis2\/!bvis\/!g;
s!dbvis2\/!dbvis\/!g;
s!\bbpro2\/!bpro\/!g;
s!vidpro2\/!vidpro\/!g;
s!dbvidpro2\/!dbvidpro\/!g;
s!vorl2\/!vorl\/!g;
s!breye2/!breye\/!g;
s!\/breye2 !\/breye\ !g;
s!\/breye2\)!\/breye\)!g;
s!\b\"bvis2_!\"bvis_!g;
s!\b\"bpro2_!\"bpro_!g;
s!\b\"vidpro2_!\"vidpro_!g;
s!\b\"vorl2_!\"vorl_!g;
s!\b\"dbvis2_!\"dbvis_!g;
s!\b\"dbvidpro2_!\"dbvidpro_!g;
# Classes
s!\bvidpro2_!vidpro_!g;
s!\bdbvidpro2_!dbvidpro_!g;
s!\bbpro2_!bpro_!g;
s!\bbvis2_!bvis_!g;
s!\bdbvis2_!dbvis_!g;
s!vorl2_!vorl_!g;

        print OUTFILE;
    }
    close OUTFILE;
    close INFILE;

}


sub mentions_breye {
    my ($filename)= ($_);
#    print "Checking $filename\n";
    open(INFILE,"<$filename") or die "Can't open $filename";

    while (<INFILE>)
    {
        if (/\bbpro2/ || /\bbvis2/ ||/\bvidpro2/ ||/\bdbvis2/ ||/\bbreye2/)
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
#    print "Found1 $filename\n";
    if (! -f $filename)  {return;}
#    print "Found $filename\n";
    if ($filename =~ /CVS/) {return;}
    if ($filename =~ /\.old/) {return;}
    if ($filename =~ /\~/) {return;}
    if ($filename =~ /\#/) {return;}
#    print "Found3 $filename\n";
    
    if (mentions_breye($filename))
    {
        print "Breye in $filename\n";
	$backup = "$filename.old";
        rename $filename, $backup or die "Unable to create backup $backup";
        # Copy backup into original file
        convert $backup, $filename;
    }
}


print "Searching $ARGV[0]\n";
# Traverse desired filesystems
File::Find::find({ wanted => \&wanted, follow => 0, no_chdir => 1 } , $ARGV[0]);
exit;
