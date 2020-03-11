#!/bin/bash
FILE=$1
REPORT="$FILE.authors"

if [ "$FILE" = "-h" ]; then
    echo "Usage: $0 [FILE-PATHNAME]"
    echo "  Creates FILE-PATHNAME.authors file with sorted list of unique author names and email addresses from git log."
fi

echo $FILE > $REPORT  
git log --follow "$FILE" | grep Author | sort | uniq | sed 's/Author://g' >> $REPORT
