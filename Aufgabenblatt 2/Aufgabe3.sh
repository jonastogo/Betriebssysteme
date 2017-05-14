#!/bin/bash

exist() {
  for argument in $*
    do
      dir=$argument
      if [ -e $dir ];
      then
        echo "$dir existiert"
      else
        echo "$dir existiert nicht"
      fi
    done
}

regular() {
  for argument in $*
    do
      dir=$argument
      if [ -f $dir ];
      then
        echo "$dir ist regulär"
      else
        echo "$dir ist nicht regulär"
      fi
    done
}

symbolic() {
  for argument in $*
    do
      dir=$argument
      if [ -h $dir ];
      then
        echo "$dir ist symbolisch"
      else
        echo "$dir ist nicht symbolisch"
      fi
    done
}

owner() {
  for argument in $*
    do
      dir=$argument
      if [ -O $dir ];
      then
        echo "$dir gehört dem Aufrufer"
      else
        echo "$dir gehört nicht dem Aufrufer*-"
      fi
    done
}

user(){
  for argument in $*
    do
      dir=$argument
      stat -c '%U' $dir
    done
}

show() {
for argument in $*
  do
    dir=$argument
    if [ -e $dir ];
    then
      dircut=$(echo "$dir" | cut -d. -f2)
      echo "-$dircut-"
      if [ "$dircut" = "txt" ];
      then
        echo "soll %dir ausgegeben werden? (y/n)"
        read choice
        if [ "$choice" = "y" ];
        then
          cat $dir
        fi
      fi
    else
      echo "$dir existiert nicht"
    fi
  done
}

exist $*
regular $*
symbolic $*
owner $*
user $*
show $*
