#!/bin/bash
FILE=$(realpath --no-symlinks $1)
DO_NOT_DELETE=("/usr/include" "/usr/bin" "/usr/sbin" "/usr/lib")

# Check is mandatory parameters are set.
if [ -z $FILE ]
then
    echo "Script must be called with a single parameter. Abort."
    exit -1
fi

# Check if DST must not be deleted.
for DIR in "${DO_NOT_DELETE[@]}"
do
    if [[ $FILE == $DIR ]]
    then
        echo "Not allowed to delete $DIR. Abort."
        exit -1
    fi
done

# All seems to be okay.
sudo rm -rf $FILE
