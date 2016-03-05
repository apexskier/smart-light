#!/bin/bash
#
# you need html-minifier installed
# `npm install html-minifier -g`

html-minifier --config-file html-minifier.conf.js index.html > index.min.html
cp index.min.html index_html.ino
sed -i 's|"|\\"|g' index_html.ino
sed -i 's|INITIAL_R|" + String(currentColor.r) + "|g' index_html.ino
sed -i 's|INITIAL_G|" + String(currentColor.g) + "|g' index_html.ino
sed -i 's|INITIAL_B|" + String(currentColor.b) + "|g' index_html.ino
sed -i 's|^\(.*\)$|const String index_html = "\1";|' index_html.ino
