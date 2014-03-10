#!/bin/bash
#
# Encoding: UTF-8
# Script: print.sh
#

# Default "parameters"
csv_file="hodnoceni.csv"
reg_exp="x"
nums="1 2 3 4 5"
sort=0

# Get parameters
until [ -z "$1" ]
do
    case $1 in
        # Another csv file  
        "-csv")    
            shift        
            csv_file=$1 
            ;;         
        # Regular expression
        "-c")
            shift 
            reg_exp=$1
            ;;
        # List of tests
        "-t")
            shift
            nums=$1
            ;; 
        # Sort
        "-s")
            shift
            sort=$1  
            ;;         
    esac
    shift    
done

# Modification: list of tests to format x,x,x
test_nums="1,2"
for i in $nums 
do
    i=$(($i + 2))   
    test_nums="$test_nums,$i"
done
test_nums="$test_nums,8"

# Create temp file
tmp_file="/tmp/tmp_file__"
echo -n "" > $tmp_file

# Read all lines from csv file and modify them
IFS=""
for line in `grep -i $reg_exp $csv_file |\
             # Removing white spaces
             sed 's/[[:space:]][[:space:]]*/ /g' | sed 's/^[[:space:]]//g' |\
             # Lines modification
             sed 's/""/"0"/' | tr -d "\"" | tr -s ";" ":"`             
do    
    echo "$line" >> $tmp_file
done

# Print students - if sorting is enabled, then sort data before printing 
if [ $sort -eq 0 ]; then
    cat "$tmp_file" |\
    # Get only wanted collums
    cut -d: -f${test_nums}
else
    # If we're going to sort by the final result, then
    # we have to sort "numerically"
    if [ $sort -gt 2 ]; then
        numeric="n"
    fi
    
    # Sorting
    cat "$tmp_file" | sort -t: -k"$sort,$sort"$numeric |\
    # Get only wanted collums
    cut -d: -f${test_nums}
fi

# Remove temp file
rm -f $tmp_file

