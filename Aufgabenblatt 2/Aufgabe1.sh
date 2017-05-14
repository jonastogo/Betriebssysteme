#!/bin/bash

dir=$1

exist() {
  dir=$1
  if [ -e $dir ];
  then
    echo "$dir existiert"
  else
    echo "$dir existiert nicht"
  fi
}

regular() {
  dir=$1
  if [ -f $dir ];
  then
    echo "$dir ist regulär"
  else
    echo "$dir ist nicht regulär"
  fi
}

symbolic() {
  dir=$1
  if [ -h $dir ];
  then
    echo "$dir ist symbolisch"
  else
    echo "$dir ist nicht symbolisch"
  fi
}

owner() {
  dir=$1
  if [ -O $dir ];
  then
    echo "$dir gehört dem Aufrufer"
  else
    echo "$dir gehört nicht dem Aufrufer"
  fi
}

user(){
  dir=$1
  stat -c '%U' $dir
}

exist $dir
regular $dir
symbolic $dir
owner $dir
user $dir
