#!/bin/bash
#
# Encoding: UTF-8
# Script: update.sh
#

# Default "parameters"
csv_file="hodnoceni.csv"

# Get parameters
# Another csv file
if [ "$1" = "-csv" ]
then
    shift        
    csv_file=$1 
    shift    
fi
  
# List of tests
list_tests=$1
shift

# Files with evaluation
test_files="$*"

# Counter
counter=1

# Create temp file
tmp_file="/tmp/tmp_file__"

# Evaluation update
for pos in $list_tests
do
    # Modify csv file - remove redundant white spaces
    csv_modified=`cat "$csv_file" | sed 's/[[:space:]][[:space:]]*/ /g' |\
                                    sed 's/^[[:space:]]//g'`       
    # Evaluation file
    file=`echo "$test_files" | cut -d' ' -f"$counter"`
    
    # Counter incrementation
    counter=$(($counter + 1))
     
    # Modify ans file - remove redundant white spaces
    file=`cat "$file" | sed 's/[[:space:]][[:space:]]*/ /g' |\
                        sed 's/^[[:space:]]//g'`     
    # Erase temp file
    echo -n "" > $tmp_file 
        
    IFS='
'
    for data in `echo "$csv_modified"`
    do      
        # Login
        login=`echo $data | cut -d' ' -f3 | tr -d '"' | tr -d ";"`
                    
        # Write students that are not in eval file to tmp file
        student=`echo "$file" | grep -i $login`               
           
        if [ -z "$student" ]; then
            echo "$data" >> $tmp_file            
            continue       
        fi 
            
        # Get score from eval file
        score=`echo "$student" | cut -d' ' -f3`            
                   
        # Load old score
        old_scores=`echo $data | cut -d' ' -f4- | tr -d ";"`
        
        for i in 1 2 3 4 5
        do
            if [ $i -eq $pos ]; then
                p[i]=\"$score\"
            else
                p[i]=`echo "$old_scores" | cut -d' ' -f$i`
            fi                
        done
            
        # Update        
        tmp_line=`echo \`echo "$data" | cut -d' ' -f1-3\` "${p[1]}"\; "${p[2]}"\; "${p[3]}"\; "${p[4]}"\; "${p[5]}"\;`
          
        # Compute sum
        sum=`echo $tmp_line |  tr -d ";" | tr -d '"' | awk '{ print int($4 + $5 + $6 + $7 + $8) }'`
         
        # Add sum to end of tmp_line and write tmp_line to tmp file        
        echo `echo "$tmp_line"` \"$sum\" >> $tmp_file            
    done
      
    # Move tmp file to $csv_file
    mv $tmp_file ./$csv_file  
done

