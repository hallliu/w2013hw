#!/bin/bash
for dir in `ls $1` 
do
    if [ -f "$1/$dir" ] #tests to see if the file in question is a regular file
    then
        echo "File $dir has `cat $1/$dir | wc -l` lines" #the awkward construction is to account for the weird behaviour of wc in which it appends the file name to the end of the line count
    elif [ -d "$1/$dir" ] #tests to see if the file in question is a directory
    then
        echo "Directory $dir has `ls $1/$dir/ | wc -l` files" #luckily, ls puts newlines after each file, so counting the number of lines works
    fi
done
exit 0
