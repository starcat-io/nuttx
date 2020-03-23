#!/bin/bash

APPROVED_AUTHORS=$1
FILE_AUTHORS=$2
REVERSE=$3

if [ "$FILE1" = "-h" ]; then
    echo "Usage: $0 [APPROVED-AUTHORS-PATHNAME] [AUTHORS-FILE-PATHNAME] <-n>"
    echo "  Determines if the only email addresses in AUTHORS-FILE-PATHNAME are from the file APPROVED-AUTHORS-PATHNAME"
    echo "  -n reverses result; prints file only if the AUTHORS-FILE-PATHNAME has lines that are NOT in APPROVED-AUTHORS-PATHNAME"
    echo "  FILE1 and FILE2 must only contain email addresses, one per line, all lower case."
    echo "  Useful for determining if authors of a file are on a list of people with signed software grants."
fi

result=$(comm -13 <(cat "$APPROVED_AUTHORS" | tr A-Z a-z | cut -f2 | tail -n+2 |sort -u) \
                  <(cat "$FILE_AUTHORS" | grep -v / | tr A-Z a-z | cut -f2 |sort -u) )

# empty result means $FILE1 only contains lines from $FILE2
if [ -z "$result" ]; then
  # output filename
  if [ -z "$REVERSE" ]; then
    echo $(cat "$FILE_AUTHORS" | head -1)
  fi
else
  # output filename
  if [ -n "$REVERSE" ]; then
    echo $(cat "$FILE_AUTHORS" | head -1)
  fi
fi
