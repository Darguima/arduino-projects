#!/bin/bash

# Create the target folder
rm -r pages_h
mkdir -p pages_h

# Loop through all HTML files in the 'pages' folder
for file in pages/*.html; do
    # Extract the filename without extension
    filename=$(basename -- "$file")
    filename_noext="${filename%.*}"

    # Use xxd to generate a C array and save it in the 'pages_h' folder
    xxd -i "$file" > "pages_h/${filename_noext}.h"
done
