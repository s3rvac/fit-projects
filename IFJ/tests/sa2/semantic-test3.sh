#!/bin/sh
#
# Testing script for IFJ06 - parser only, tests III.
#
file=./tests/sa2/illegalexpressions.input
valid_result=./tests/sa2/illegalexpressions.result
output_file=tmp
parser=./tests/parser
test_num=1
num_passed=0
num_failed=0


  echo ""
  echo "------------------------------------------------"
  echo "Parser - tests III - nelegalni vyrazy - enjoy ;)"
  echo "------------------------------------------------"
  echo ""

for expression in `cat $file`;
do
  echo "void main(); void main() var int inttmp, double doubletmp, string stringtmp; {$expression}" | $parser > $output_file 2>&1

  result=`diff $valid_result $output_file`
  if [ "$result" = "" ]; then
    num_passed=$(($num_passed + 1))
  else
    echo "Failed $test_num"
    num_failed=$(($num_failed + 1))
  fi

  rm $output_file
  test_num=$(($num_failed + 1))
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

