# !bin/bash
# Author: Aaron Hinckley and Zach Metcalf

dump=~/RecycleBin   # NOTE: Variable assignments do not have spaces
                    # NOTE: either use for [ "" ] or [[]] for string operations 

while getopts "lp" opt; do
  case $opt in
    l)
	if [ -a $dump ]; then
		cd $dump
		ls -R
	else
		echo "RecycleBin does not exist"
	fi
	;;
    p)
        if [ -a $dump ]; then
        	cd $dump
	        for file in *; do
        	    if [ -d $file ]; then
                	rm -drf $file  # d--> dir, r--> recursive, f--> force
	            else
        	        rm $file
	            fi
	        done
        else 
		echo "RecycleBin does not exist"
	fi
        ;;
   \?)
	echo "-----------------------------------------------------------------------------------------------------------------------"
	echo "The command junk emulates the use of a recycle bin in the command line by keeping junk files in the location ~/RecycleBin"
	echo "This command has four uses: "
	echo "   1) junk ----------> Displays this help message"
	echo "   2) junk -l [fileName1 fileName2] ---------> lists all files and directories in ~/RecycleBin"
	echo "   3) junk -p [fileName1 fileName2] ---------> deletes all files and directories in ~/RecycleBin"
	echo "   4) junk fileName1 fileName2 ----------> moves the fileName arguments to the location ~/RecycleBin"
	echo "-----------------------------------------------------------------------------------------------------------------------"
        ;;
  esac
done 
if [ $# -eq 0 ]; #checks to see if no opts were passed
then
  echo "-----------------------------------------------------------------------------------------------------------------------"
  echo "The command junk emulates the use of a recycle bin in the command line by keeping junk files in the location ~/RecycleBin"
  echo "This command has four uses: "
  echo "   1) junk ----------> Displays this help message"
  echo "   2) junk -l [fileName1 fileName2] ---------> lists all files and directories in ~/RecycleBin"
  echo "   3) junk -p [fileName1 fileName2] ---------> deletes all files and directories in ~/RecycleBin"
  echo "   4) junk fileName1 fileName2 ----------> moves the fileName arguments to the location ~/RecycleBin"
  echo "-----------------------------------------------------------------------------------------------------------------------"

else
  if [ -e $dump ]; then   # If RecycleBin exists 
    if [ -d $dump ]; then  # If RecycleBin is a directory
      for file in $*; do   # Case I
        #echo $file
	if [ -e $file ]; then  # If file is found
	  mv $file $dump  # mv command moves all nested files if $file is a directory
#///////else /////////////////////// This else statement gives problems with get opts
         # echo "$file not found"
        fi
      done
    else    # RecycleBin exists but not a directory
      echo "RecycleBin is not a directory!!!"  # Case III
    fi
  else   # RecycleBin does not exist
    mkdir $dump   # Case II
    for file in $*; do   # Case I again
      if [ -e $file ]; then  # If file is found
        mv $file $dump  # mv command moves all nested files if $file is a directory
      else   
        echo "$file not found"
      fi
    done
  fi
fi
