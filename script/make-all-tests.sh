#! /bin/bash

set -eo pipefail

echo "#include <stdio.h>"
echo "int main() {"

files=$(find . -path "./test_*.c" -printf "%P\n")
for file in $files; do
  funcs=$(ctags -x $file | cut -f 1 -d ' ')
  for func in $funcs; do
    echo "void $func();"
    # TODO: Character buffering
    echo "printf(\"Running $func...\n\");"
    echo "$func();"
    echo "printf(\"Done!\n\");"
  done
done

echo "return 0;"
echo "}"
