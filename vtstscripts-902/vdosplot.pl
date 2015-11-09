eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}' && eval 'exec perl -S $0 $argv:q' if 0;
#;-*- Perl -*-

$gnufile="dosplot.gnu";
foreach $DOSFILE (@ARGV){
  open GNUPLOT, ">$gnufile";
  print GNUPLOT "set grid\n";
  print GNUPLOT "set pointsize 2\n";
  print GNUPLOT "set xlabel \"Energy [eV]\"\n";
  print GNUPLOT "set ylabel \"DOS\"\n";
  print GNUPLOT "set nokey\n";
  print GNUPLOT "set terminal postscript eps color\n";
  print GNUPLOT "set output \"dosplot.eps\"\n";
  print GNUPLOT "plot \"$DOSFILE\" u 1:2 w lp lt 1 lw 2.0 pt 7 ps 0.6";
  close GNUPLOT;
  system("gnuplot < $gnufile");
  system("gv -scale 4 dosplot.eps \&");
  system("rm $gnufile");
}

