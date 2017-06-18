undelete() {
  echo "hallo"
  if [ -e $HOME/.trashBin/$1 ]; then
    echo "Hallo"
    row=$(grep -F "$1" $HOME/.trashBin/.dir)
    oldFile=$(echo "$row" | cut -d' ' -f2)
    directory=$(dirname "$oldFile")
    echo $directory
    if [ -d $directory ]; then
      echo "Hallo"
      mv "$HOME/.trashBin/$1" "$oldFile"
      sed -i "/$1/d" "$HOME/.trashBin/.dir"
      echo "$1 restored to $oldFile"
    else
      echo "Old directory not found!"
    fi

  fi
}

undelete $1
