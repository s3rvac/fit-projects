#!/bin/bash
#
# Encoding: UTF-8
# Script: transform.sh
#

# Default I/O files
d_csv_file="seznam.csv"
d_ans_file="odpovedi.txt"
eval_file="eval.txt"

# Get parameters
until [ -z "$1" ]
do
    case $1 in
        # Print statistical data
        "-s")
            print_stat=1 
            ;;       
        # Another csv file
        "-csv")    
            shift        
            d_csv_file=$1 
            ;;      
        # Another answer file
        "-a")
            shift 
            d_ans_file=$1
            ;;
        # Another output file  
        "-o")
            shift
            eval_file=$1
            ;;        
    esac
    shift
done

# Create blank output file
echo -n "" > $eval_file

# Remove white spaces and save input files into variables for faster access
# + do proper text modification (for speeding up the script) 
csv_file=`cat "$d_csv_file" | sed 's/[[:space:]][[:space:]]*/ /g' |\
          sed 's/^[[:space:]]//g' |\
          tr -d "\"" | tr -d ";" | tr A-Z a-z`
ans_file=`cat "$d_ans_file" | sed 's/[[:space:]][[:space:]]*/ /g' |\
          sed 's/^[[:space:]]//g' |\
          tr -d "," | tr A-Z a-z`

oldIFS=$IFS
IFS='
'
for line in `echo "$csv_file"`
do    
    # Read login, variants and answers  
    login=`echo "$line" | awk '{ print $1 }'`
    variant=`echo "$line" | awk '{ print $2 }'`
    answers=`echo "$line" | cut -d' ' -f3-`
        
    # Find line with answers incident to loaded variant
    answers_ans=`echo "$ans_file" | grep ^$variant | cut -d' ' -f2-`
             
    # Count of right answers
    count=0
    # Count of answers
    ans_count=`echo "$answers" | wc -w`       
    # Counter
    i=1
            
    # Checking count of right answers
    while [ $i -le $ans_count ]
    do
        # Save student's and right answers to strings
        ans1=`echo "$answers" | cut -d' ' -f"$i"`
        ans2=`echo "$answers_ans" | cut -d' ' -f"$i"`
                
        # Check answer
        if [ "$ans1" = "$ans2" ]; then            
            count=$(($count + 1))
        fi 
        
        # Counter incrementation                    
        i=$(($i + 1))                       
    done 
    
    # Variant should be upper-case
    variant=`echo $variant | tr a-z A-Z`
    # Write data to output file
    echo "$login: $variant: $count" >> $eval_file
done

# If parameter -s was given, statistical data will be written to standard output
if [ $print_stat ]; then
    # List of all possible variants   
    variant_list="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
    
    IFS=$oldIFS
    for variant in $variant_list
    do
        data=`grep "$variant" "$eval_file" | tr -d " " | cut -d: -f3`        
        
        # At least 1 item was found
        if [ "$data" = "" ]; then
            continue
        fi              
        # Compute overall number of students
        count=`echo "$data" | wc -l`
                
        # Compute final sum
        sum=0        
        for value in $data 
        do           
            sum=$(($sum + $value))
        done                
        # Write data to standard output
        echo "$variant: $count: $sum"        
    done 
fi

