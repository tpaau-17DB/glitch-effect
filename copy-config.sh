#!/bin/bash

source_path="conf/*"
target_path="$HOME/.config/glitch-effect/"

copy_files()
{
  echo "Copying '${source_path}' to '${target_path}'"
  cp $source_path $target_path
}

mkdir -p "$target_path"

if [ -d "$target_path" ] && [ "$(ls -A "$target_path")" ]; then
    read -p "$target_path exists and is not empty.
Do you wish to overwrite it? [Y/n]: " choice
    choice="${choice:-Y}"
    case "$choice" in
        [Yy]* )
            echo "You chose to overwrite the file."
            copy_files
        ;;
        [Nn]* )
            echo "${target_path} was not overwritten."
            exit 0
        ;;
        * )
            echo "Invalid input. Please answer with 'y' or 'n'."
            exit 1
        ;;

    esac
else
    echo "OK: ${target_path} is empty"
    copy_files
fi
