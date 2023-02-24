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



check .
check src/include -print
check tests -type d
