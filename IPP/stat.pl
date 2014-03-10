#!/usr/bin/perl -w
#
# File:         stat.pl
# Project:      IPP/2007L
# Date:         22.4.2007
# Authors:      Libor Polcak, xpolca03@stud.fit.vutbr.cz
#               Boris Prochazka, xproch63@stud.fit.vutbr.cz
#               Martin Rapavy, xrapav00@stud.fit.vutbr.cz
#               Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Description:  C++ source statistical script

# Modules
use strict;

# Constants
use constant FALSE => 0;
use constant TRUE => 1;

# Config variables
my $source_files_path = "src";
my $indent_spaces = 10;
my $total_text = "OVERALL:";

my $max_length = 0;

# Procedures

# Lists C++ source files in directory
sub get_source_files {
	# List files
	my @files;
	@files = glob "$_[0]/*.cpp";
	@files = (@files, glob "$_[0]/*.h");

	# Return listed files
	return @files;
}

# Prints indent spaces
sub print_spaces {
	for (my $i = 0; $i < $max_length - ($_[0] + $_[1]) + $indent_spaces; $i++) {
		print " ";
	}
}

# Keywords
my %keywords = map {$_ => TRUE} qw(
	auto const double float int short struct unsigned break continue else for
	long signed switch void	case default enum goto register sizeof typedef
	volatile char do extern if return static union while export
	asm dynamic_cast namespace reinterpret_cast try bool explicit new static_cast
	typeid catch false operator template typename class friend private this using
	const_cast inline public throw virtual delete mutable protected true wchar_t
);

# Operators
my %operators = map {$_ => TRUE} qw(
	and bitand compl not_eq or_eq xor_eq and_eq bitor not or xor
	. ! ~ + - & * sizeof new delete / % < > | =
	-> :: ++ -- .* ->* << >> <= >= == != && || *= /= %= += -= &= |=
	<<= >>=
), ",";

# Help message
my $usage = "Name: \n" .
		"    stat - prints statistical information about source files\n\n" .
		"Synopsis:\n" .
		"    $0 PARAMETER\n\n" .
		"Description:\n" .
		"    PARAMETER must be one of the following\n" .
		"    \n" .
		"    -k     prints the number of keywords in every source file\n" .
		"           and overall\n" .
		"    -o     prints the number of operators in every source file\n" .
		"           and overall\n" .
		"    -i     prints the number of identifiers in every source file\n" .
		"           and overall (contains keywords)\n" .
		"    -ik    prints the number of identifiers in every source file\n" .
		"           and overall (does not contain keywords)\n" .
		"    -w <pattern> searches text string pattern in all source files\n" .
		"                 and prints the number of appearence in every file\n" .
		"                 and overall\n" .
		"    --help display this help and exit\n\n" .
		"Authors:\n" .
		"    Libor Polcak, xpolca03\@stud.fit.vutbr.cz\n" .
		"    Boris Prochazka, xproch63\@stud.fit.vutbr.cz\n" .
		"    Martin Rapavy, xrapav00\@stud.fit.vutbr.cz\n" .
		"    Petr Zemek, xzemek02\@stud.fit.vutbr.cz\n";


# Check program parameters
die "$usage" if $#ARGV > 1 || $#ARGV == -1 || ($#ARGV == 1 && $ARGV[0] ne "-w");

my $opt_keywords = FALSE;
my $opt_operators = FALSE;
my $opt_identifiers = FALSE;
my $opt_pattern = FALSE;
my $pattern = "";

# Parse program parameters
if ($ARGV[0] eq "-k") {
	$opt_keywords = TRUE;
}
elsif ($ARGV[0] eq "-o") {
	$opt_operators = TRUE;
}
elsif ($ARGV[0] eq "-i") {
	$opt_identifiers = TRUE;
	$opt_keywords = TRUE;
}
elsif ($ARGV[0] eq "-ik") {
	$opt_identifiers = TRUE;
}
elsif ($ARGV[0] eq "-w") {
	$opt_pattern = TRUE;
	if ($#ARGV == 0 || $ARGV[1] eq "") {
		print "$0: pattern cannot be an empty string\n";
		print "See `$0 --help` for more information.\n";
		exit 1;
	}
	$pattern = $ARGV[1];
}
elsif ($ARGV[0] eq "--help") {
	print $usage;
	exit 0;
}
else {
	print "Unknown option: $ARGV[0]\nSee script usage: $0 --help\n";
	exit 1;
}

# Total number of occurences (in all source files)
my $total = 0;

# Get list of C++ sources and headers
my @source_files = get_source_files($source_files_path);
push @source_files, get_source_files("$source_files_path/configs");
push @source_files, get_source_files("$source_files_path/gui");
push @source_files, get_source_files("$source_files_path/objects");

# Create `hash` to achieve "fancy" output style easier
# and reset all source file counters
my %src_file_counts = map { $_ => 0 } @source_files;

if ($opt_pattern) {
	# Search source files for pattern
	foreach my $file (keys %src_file_counts) {
		# Open source file
 		open(FD, "<$file")
 			or die ("Cannot open source file: " . $file . "\n");

		$src_file_counts{$file} = grep(/$pattern/, <FD>);
	}
}
else {
	# Analyze source files
	foreach my $file (keys %src_file_counts) {
		# Reset keyword counter
		my $keywords_count = 0;

		# Open source file
 		open(FD, "<$file")
 			or die ("Cannot open source file: " . $file . "\n");

		# Load whole source file into scalar
		my $cpp_source = do { local $/; <FD>; };

		# Strip string literals
 		$cpp_source =~ s/\".*?\"/\"\"/sg;

		# Strip character literals
 		$cpp_source =~ s/\'\\?.\'/\'\'/sg;

		# Strip comments
		$cpp_source =~ s/\/\*.*?\*\// /sg; # Block comments
		$cpp_source =~ s/\/\/.*$/ /mg; # Line comments

		# Strip preprocessor directives
		$cpp_source =~ s/#.*?$/ /mg;

		# Pack all whitespace characters into single space
		$cpp_source =~ s/\s+/ /sg;

		# Split source into array (for easier scanning)
		my @norm_source = split(/\b| /, $cpp_source);

		# Identifiers searching
		if ($opt_identifiers) {
			$src_file_counts{$file} = grep(/[_a-zA-Z]\w*/, @norm_source);
			$keywords_count = grep($keywords{$_}, @norm_source);
			$src_file_counts{$file} -= $keywords_count;
		}

		# Keywords searching
		if ($opt_keywords) {
			# Check if keywords haven't been already counted (-i parameter)
			if (!$opt_identifiers) {
				$keywords_count = grep($keywords{$_}, @norm_source);
			}
			$src_file_counts{$file} += $keywords_count;
		}

		# Operators searching
		if ($opt_operators) {
			$src_file_counts{$file} = grep($operators{$_}, @norm_source);
		}

		# Increase total count
		$total += $src_file_counts{$file};

		close(FD);
	}
}

# Find longest source filename (used for output indention)
foreach my $filename (keys %src_file_counts) {
	my $current_length = length($filename);
	if ($current_length > $max_length) {
		$max_length = $current_length;
	}
}

# Print partial number of occurences (per file)
foreach my $file (sort keys %src_file_counts) {
	print $file;
	print_spaces(length($file), length($src_file_counts{$file}));
	print $src_file_counts{$file} . "\n";
}

# Print overall number of occurences (in all files)
print $total_text;
print_spaces(length($total_text), length($total));
print $total . "\n";

# End of file: stat.pl
