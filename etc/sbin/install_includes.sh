#!/bin/bash
SRC=$1
DST=$2

# Sanity checks
if [ -z $SRC ] || [ -z $DST ]
then
    exit -1
fi

# Copy all contents from SRC to DST in case SRC/* is not empty
if [ ! -z "$(ls -A $SRC)" ]
then
	sudo mkdir -p $DST
	sudo cp -r $SRC/* $DST
fi
