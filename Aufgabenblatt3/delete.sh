#!/bin/bash

delete() {

  DIRNAME=$(date +%y%m%d%H%M%S)_$$.dat

	if [ ! -d $HOME/.trashBin ]; then
		mkdir $HOME/.trashBin
	fi

  if [ ! -d $HOME/.trashBin/.dir ]; then
    touch $HOME/.trashBin/.dir
  fi

	if [ -e $1 ]; then
    echo "$DIRNAME $1" >> $HOME/.trashBin/.dir
		mv $1 $HOME/.trashBin/$DIRNAME
	fi
}

delete $1
