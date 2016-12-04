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
        if ( /\bbpro/ || /\bbvis/ ||/\bvidpro/ || /\bdbvis/ || /\bdbvidpro/ )
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
	$backup =~ s/bpro_/bpro1_/;
	$backup =~ s/bvis_/bvis1_/;
	$backup =~ s/vidpro_/vidpro1_/;
	$backup =~ s/dbvidpro_/dbvidpro1_/;
	$backup =~ s/dbvis_/dbvis1_/;
	$backup =~ s/vorl_/vorl1_/;
	$backup =~ s/vorl\./vorl1\./;
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
