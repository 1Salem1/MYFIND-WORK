#!/bin/bash

# Check if two find commands return the same output for a given path
# Usage: check path/to/directory
function check() {
  local path="$1"
  if [ -z "${path:+x}" ]; then
    echo "Error: please provide a path to the function."
    return 1
  fi

  local diff=$(diff <(find "${path}" 2>/dev/null) <(./myfind "${path}" 2>/dev/null))
  if [ -n "${diff}" ]; then
    echo -e "\e[31m[ FAIL ]\e[39m ${path}"
    echo "${diff}"
    return 1
  else
    echo -e "\e[32m[ PASS ]\e[39m ${path}"
    return 0
  fi
}


check src
check makefile src
check .
check src -name 'myfind.*'
check src -name 'myfind.?'
check src -type d
check src/*.o -o src/include/myfind.h
check find
check src/main.o -newer src/main.c

