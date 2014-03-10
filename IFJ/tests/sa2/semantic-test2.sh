#!/bin/sh
#
# Testing script for IFJ06 - parser only, tests II.
#
dir=./tests/sa2
parser=./tests/parser
num_tests=45
num_passed=0
num_failed=0

  echo ""
  echo "------------------------------------------------"
  echo "Parser - tests II."
  echo "------------------------------------------------"
  echo ""

for i in `seq 1 $num_tests`;
do
  valid_result=`echo $dir/sa_$i.result`
  input_file=`echo $dir/sa_$i.input`
  output_file=`echo $dir/sa_$i`

  cat $input_file | $parser > $output_file 2>&1

  result=`diff $valid_result $output_file`
  if [ "$result" = "" ]; then
    echo "$i OK"
    num_passed=$(($num_passed + 1))
  else
    echo "$i Failed"
    num_failed=$(($num_failed + 1))
  fi

  rm $output_file
done

# Summary
echo ""
echo "------------------------------------------------"
echo "Summary"
echo "------------------------------------------------"
echo ""
echo "Passed: " $num_passed
echo "Failed: " $num_failed
echo ""
