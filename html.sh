#!/bin/bash
#
# you need html-minifier installed
# `npm install html-minifier -g`

html-minifier --config-file html-minifier.conf.js index.html > index.min.html
cp index.min.html index_html.ino.example
sed -i 's|"|\\"|g' index_html.ino.example
sed -i 's|^\(.*\)$|const String index_html_head = "\1";|' index_html.ino.example
sed -i 's|INITIAL_COLOR|";\nconst String index_html_tail = "|g' index_html.ino.example
