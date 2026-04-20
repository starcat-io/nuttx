#!/bin/bash
rm ./authors.db
for FILE in `grep -irl "Copyright"`; do ./authors.sh $FILE;done
