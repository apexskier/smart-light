#!/bin/bash
#
# you need html-minifier installed
# `npm install html-minifier -g`

# minify html
html-minifier --config-file html-minifier.conf.js index.html > index.min.html

# rewrite to c++ and split where live color is added
cp index.min.html index_html.ino.example
sed -i 's|"|\\"|g' index_html.ino.example
sed -i 's|^\(.*\)$|const String index_html_head = "\1";|' index_html.ino.example
sed -i 's|INITIAL_COLOR|";\nconst String index_html_tail = "|g' index_html.ino.example

# remove old stuff and add new stuff into server.ino
tail -n +3 server.ino > server.ino.~
cat index_html.ino.example > server.ino
echo >> server.ino
cat server.ino.~ >> server.ino

