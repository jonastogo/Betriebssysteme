undelete() {
  if [ -e $HOME/.trashBin/$1 ]; then
    
    row=$(grep -F "$1" $HOME/.trashBin/.dir)
    oldFile=$(echo "$row" | cut -d' ' -f2)
    directory=$(dirname "$oldFile")

    if [ -d $directory ]; then
      mv "$HOME/.trashBin/$1" "$oldFile"
      sed -i "/$1/d" "$HOME/.trashBin/.dir"
    else
      echo "Old directory not found!"
    fi

  fi
}

undelete $1
