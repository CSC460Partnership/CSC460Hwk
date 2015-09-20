# !bin/bash
# Author: Aaron Hinckley and Zach Metcafe

dump=~/RecycleBin   # NOTE: Variable assignments do not have spaces
                    # NOTE: either use for [ "" ] or [[]] for string operations 

#########  Number 1  ##########
if [ $# -eq 0 ]; then
	echo "-----------------------------------------------------------------------------------------------------------------------"
	echo "The command junk emulates the use of a recycle bin in the command line by keeping junk files in the location ~/RecycleBin"
	echo "This command has four uses: "
	echo "   1) junk ----------> Displays this help message"
	echo "   2) junk -l [fileName1 fileName2] ---------> lists all files and directories in ~/RecycleBin"
	echo "   3) junk -p [fileName1 fileName2] ---------> deletes all files and directories in ~/RecycleBin"
	echo "   4) junk fileName1 fileName2 ----------> moves the fileName arguments to the location ~/RecycleBin"
	echo "-----------------------------------------------------------------------------------------------------------------------"
    
#########  Number 2  ##########
elif [ "$1" = "-l" ]; then  # lists all files in the recycle directory
	if [ -a $dump ]; then
        cd $dump
        ls -R
	else 
		echo "RecycleBin does not exist"
	fi
    
#########  Number 3  ##########
elif [ "$1" = "-p" ]; then
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

#########  Number 4  ##########
else   # Assumes whatever is listed is a filename
	if [ -a $dump ]; then   # If RecycleBin exists 
        if [ -d $dump ]; then  # If RecycleBin is a directory
            for file in $*; do   # Case I
                #echo $file
                if [ -n "$(find ~ -name $file)" ]; then  # If file is found
                    mv $(find ~ -name $file) $dump  # mv command moves all nested files if $file is a directory
                else
                    echo "$file not found"
                fi
            done
        else    # RecycleBin exists but not a directory
            echo "RecycleBin is not a directory!!!"  # Case III
        fi
    else   # RecycleBin does not exist
        mkdir $dump   # Case II
        for file in $*; do   # Case I again
            if [ -n "$(find ~ -name $file)" ]; then  # If file is found
                mv $(find ~ -name $file) $dump  # mv command moves all nested files if $file is a directory
            else   
                echo "$file not found"
            fi
        done
    fi
fi



