#!/bin/bash

USER_HOME="$HOME"

TARGET_DIR="$USER_HOME/.local/state/ftag"

if [ -d "$TARGET_DIR" ]; then
    echo "Directory $TARGET_DIR exists."

    read -p "Are you sure you want to delete $TARGET_DIR? This action cannot be undone. (Y/n): " CONFIRM
    # Convert input to uppercase to handle lowercase 'y' as well
    CONFIRM=$(echo "$CONFIRM" | tr '[:lower:]' '[:upper:]')

    if [[ "$CONFIRM" == "Y" || "$CONFIRM" == "" ]]; then
        rm -rf "$TARGET_DIR"
        echo "Directory $TARGET_DIR has been deleted."
    else
        echo "Deletion canceled."
    fi
else
    echo "Directory $TARGET_DIR does not exist. No action needed."
fi
rm -rf build 
mkdir build 
cd build
cmake ..
make 
