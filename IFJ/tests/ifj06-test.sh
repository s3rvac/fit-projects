#!/bin/sh
#
# Testing script for IFJ06 project
#

# Result variables
num_passed=0
num_failed=0

# Possible tests
test_all=0
test_sort=0
test_scanner=0
test_parser=0

# Verbose
verbose=1

# Prints correct result and result
print_results () {
	if [ "$verbose" -eq 1 ]; then
		echo "Correct result: "
		echo "	" $1
		echo "Result: "
		echo "	" $2
	fi
}

# Prints correct result and result (from files)
print_results_files () {
	if [ "$verbose" -eq 1 ]; then
		echo "Correct result: "
		cat $1
		echo "Result: "
		cat $2
	fi
}

echo "IFJ06 - testing script"

# Do tests according to program parameters
until [ -z "$1" ]
do
	case $1 in
	"--sort")
		shift
		test_sort=1
		;;
	"--scanner")
		shift
		test_scanner=1
		;;
	"--parser")
		shift
		test_parser=1
		;;
	"--all")
		shift
		test_all=1
		;;
	"--verbose")
		shift
		verbose=1
		;;
	esac
done

# Scanner
if [ "$test_all" -eq 1 ] || [ "$test_scanner" -eq 1 ]; then
	echo ""
	echo "------------------------------------------------"
	echo "Scanner"
	echo "------------------------------------------------"
	echo ""

	# la_01 - all possible tokens
	valid_result='./tests/la/la_01.result'
	output_file='./tests/la_01.output'
	`./tests/happyscanner ./tests/la/la_01 > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "All possible tokens (la/la_01):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "All possible tokens (la/la_01):		Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# la_02 - empty file
	valid_result='./tests/la/la_02.result'
	output_file='./tests/la_02.output'
	`./tests/happyscanner ./tests/la/la_02 > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Empty file (la/la_02):			OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Empty file (la/la_02):			Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# la_03 - empty file
	valid_result='./tests/la/la_03.result'
	output_file='./tests/la_03.output'
	`./tests/happyscanner ./tests/la/la_03 > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "EOF right after lexem (la/la_03):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "EOF right after lexem (la/la_03):	Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# la_04 - long identifiers/numbers
	valid_result='./tests/la/la_04.result'
	output_file='./tests/la_04.output'
	`./tests/happyscanner ./tests/la/la_04 > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Long identifiers/numbers (la/la_04):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Long identifiers/numbers (la/la_04):	Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_01.ifj
	valid_result='./tests/prog_01.result'
	output_file='./tests/prog_01.output'
	`./tests/happyscanner ./tests/prog_01.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_01.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_01.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_02.ifj
	valid_result='./tests/prog_02.result'
	output_file='./tests/prog_02.output'
	`./tests/happyscanner ./tests/prog_02.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_02.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_02.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_03.ifj
	valid_result='./tests/prog_03.result'
	output_file='./tests/prog_03.output'
	`./tests/happyscanner ./tests/prog_03.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_03.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_03.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Comments (extension)
	valid_result='./tests/la/comments.result'
	output_file='./tests/comments.output'
	`./tests/happyscanner ./tests/la/comments.input > $output_file 2>&1`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Comments (extension):			OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Comments (extension):			Failed"
		num_failed=$(($num_failed + 1))
		print_results_files $valid_result $output_file
	fi
	rm $output_file
fi

# Sorting
if [ "$test_all" -eq 1 ] || [ "$test_sort" -eq 1 ]; then
	echo ""
	echo "------------------------------------------------"
	echo "Sorting"
	echo "------------------------------------------------"
	echo ""

	# Stand-alone sort test - correct input
	valid_result='alois$benda$cenek$zemek'
	result=`./tests/sort-alone 'zemek$cenek$alois$benda'`
	if [ "$result" = "$valid_result" ]; then
		echo "Stand-alone (correct input):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Stand-alone (correct input):		Failed"
		num_failed=$(($num_failed + 1))
		print_results $valid_result $result
	fi

	# Stand-alone sort test - empty string
	valid_result=''
	result=`./tests/sort-alone`
	if [ "$result" = "$valid_result" ]; then
		echo "Stand-alone (empty input):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Stand-alone (empty input):		Failed"
		num_failed=$(($num_failed + 1))
		print_results $valid_result $result
	fi

	# Stand-alone sort test - only delimiters
	valid_result='$$$$'
	result=`./tests/sort-alone '$$$$'`
	if [ "$result" = "$valid_result" ]; then
		echo "Stand-alone (only delimiters):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Stand-alone (only delimiters):		Failed"
		num_failed=$(($num_failed + 1))
		print_results $valid_result $result
	fi
fi

# Parser (SA)
if [ "$test_all" -eq 1 ] || [ "$test_parser" -eq 1 ]; then
	echo ""
	echo "------------------------------------------------"
	echo "Parser"
	echo "------------------------------------------------"
	echo ""

	# Input reading
	result=`./tests/parser ./tests/vzorove/cteni_vstupu.src 2>&1`
	if [ "$result" = "OK" ]; then
		echo "Input reading (cteni_vstupu.src):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Input reading (cteni_vstupu.src):	Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi

	# Input reading II
	result=`./tests/parser ./tests/vzorove/cteni_vstupu_2.src 2>&1`
	if [ "$result" = "OK" ]; then
		echo "Input reading 2 (cteni_vstupu_2.src):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Input reading 2 (cteni_vstupu_2.src):	Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi

	# Ray
	result=`./tests/parser ./tests/vzorove/ray.src 2>&1`
	if [ "$result" = "OK" ]; then
		echo "Ray (ray.src):				OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Ray (ray.src):				Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi

	# Sqrt
	result=`./tests/parser ./tests/vzorove/sqrt.src 2>&1`
	if [ "$result" = "OK" ]; then
		echo "Sqrt (sqrt.src):			OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Sqrt (sqrt.src):			Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi

	# Valid program - prog_01.ifj
	valid_result='./tests/prog_01.result_sa'
	output_file='./tests/prog_01.output_sa'
	cat ./tests/prog_01.ifj | ./tests/parser > $output_file 2>&1
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_01.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_01.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_02.ifj
	valid_result='./tests/prog_01.result_sa'
	output_file='./tests/prog_01.output_sa'
	cat ./tests/prog_01.ifj | ./tests/parser > $output_file 2>&1
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_02.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_02.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_03.ifj
	valid_result='./tests/prog_01.result_sa'
	output_file='./tests/prog_01.output_sa'
	cat ./tests/prog_01.ifj | ./tests/parser > $output_file 2>&1
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_03.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_03.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file
fi

# "Complete program (interpret)
if [ "$test_all" -eq 1 ] || [ "$test_sort" -eq 1 ]; then
	echo ""
	echo "------------------------------------------------"
	echo "Complete program (interpret)"
	echo "------------------------------------------------"
	echo ""

	# Valid program - prog_01.ifj
	valid_result='./tests/prog_01.out'
	output_file='./tests/prog_01.out_'
	`./ifj06 ./tests/prog_01.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_01.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_01.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_02.ifj
	valid_result='./tests/prog_02.out'
	output_file='./tests/prog_02.out_'
	`./ifj06 ./tests/prog_02.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_02.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_02.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Valid program - prog_03.ifj
	valid_result='./tests/prog_03.out'
	output_file='./tests/prog_03.out_'
	`./ifj06 ./tests/prog_03.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Valid program (prog_03.ifj):		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Valid program (prog_03.ifj):		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Input reading
	valid_result='./tests/vzorove/cteni_vstupu.out'
	output_file='./tests/vzorove/cteni_vstupu.out_'
	`./ifj06 ./tests/vzorove/cteni_vstupu.src < ./tests/vzorove/cteni_vstupu.in > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Input reading (cteni_vstupu.src):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Input reading (cteni_vstupu.src):	Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Input reading II
	valid_result='./tests/vzorove/cteni_vstupu_2.out'
	output_file='./tests/vzorove/cteni_vstupu_2.out_'
	`./ifj06 ./tests/vzorove/cteni_vstupu_2.src < ./tests/vzorove/cteni_vstupu_2.in > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Input reading 2 (cteni_vstupu_2.src):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Input reading 2 (cteni_vstupu_2.src):	Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Ray
	valid_result='./tests/vzorove/ray.out'
	output_file='./tests/vzorove/ray.out_'
	`./ifj06 ./tests/vzorove/ray.src > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Ray (ray.src):				OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Ray (ray.src):				Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Sqrt
	valid_result='./tests/vzorove/sqrt.out'
	output_file='./tests/vzorove/sqrt.out_'
	`./ifj06 ./tests/vzorove/sqrt.src > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Sqrt (sqrt.src):			OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Sqrt (sqrt.src):			Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Mandel (z fitusky)
	valid_result='./tests/vzorove/mandel.out'
	output_file='./tests/vzorove/mandel.out_'
	`./ifj06 ./tests/vzorove/mandel.src > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Mandel - z fitusky  (mandel.src):	OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Mandel - z fitusky (mandel.src):	Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# All possible instructions
	valid_result='./tests/all_instructions.out'
	output_file='./tests/all_instructions.out_'
	`./ifj06 ./tests/all_instructions.ifj > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "All possible instructions:		OK"
		num_passed=$(($num_passed + 1))
	else
		echo "All possible instructions:		Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file

	# Built-in functions
	valid_result='./tests/std_funcs.out'
	output_file='./tests/std_funcs.out_'
	input_data='./tests/std_funcs.in'
	`./ifj06 ./tests/std_funcs.ifj < $input_data > $output_file`
	result=`diff $valid_result $output_file`
	if [ "$result" = "" ]; then
		echo "Built-in functions:			OK"
		num_passed=$(($num_passed + 1))
	else
		echo "Built-in functions:			Failed"
		num_failed=$(($num_failed + 1))
		#print_results_files $valid_result $output_file
	fi
	rm $output_file
fi

# Summary
echo ""
echo "------------------------------------------------"
echo "Summary"
echo "------------------------------------------------"
echo ""
echo "Passed: " $num_passed
echo "Failed: " $num_failed
echo ""

echo "================================================"
echo "================================================"
./tests/sa2/semantic-test2.sh
./tests/sa2/semantic-test3.sh

exit 0
