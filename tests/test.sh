#!/bin/bash

if [[ $# != 2 ]]; then
  echo "Sposób uzytkowania: <ścieżka do computer.h> <liczba linii debugu> " >&2
  exit 1
fi

tests=$(pwd)
computer="$1"
fedback_size="$2"

threshold=${3:-1}

total=0
correct=0
leaked=0

RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'
  
function traverse_folder() {
  folder="$1"
  variant="$2"
  shopt -s nullglob
  cp $computer $folder #kopia computer.h
  for f in "$folder"/*.cc; do
    randfloat=$(printf '0%s\n' $(echo "scale=8; $RANDOM/32768" | bc ))
    if (( $(echo "$randfloat < $threshold" |bc -l) )); then
      run_test "$f" "$variant"
    fi
  done
  rm $folder/computer.h #usuniecie computer.h

  shopt -s nullglob
  for d in "$folder"/*/; do
    echo "$d"
    traverse_folder "$(realpath "$d")" "$variant"
  done
}

function run_test() {
  input_file="$1"
  variant="$2"
  
  ((total++))
  echo -e "\e[1mTest $f \e[0m"
  
  OUTPUT=$(clang -Wall -Wextra -std=c++17 -O2 -lstdc++ $input_file 2>&1)
  
  result=$?
  
    if [[ "$variant" -eq 1 ]]; then
        if [[ "$result" -eq 1 ]]; then
            echo -ne "${GREEN}OK${NOCOLOR}\n"
            let "correct++"
        else
            echo "$OUTPUT" | head -n "$fedback_size"
            echo -ne "${RED}BŁĄD KOMPILACJA ZAKOŃCZONA SUKCESEM${NOCOLOR}\n"
            let "leaked++"
            rm "a.out"
        fi
    elif [[ "$variant" -eq 2 ]]; then
        if [[ "$result" -eq 0 ]]; then
            echo -ne "${GREEN}OK${NOCOLOR}\n"
            let "correct++"
            rm "a.out"
        else
            echo "$OUTPUT" | head -n "$fedback_size"
            echo -ne "${RED}BŁĄD KOMPILACJI${NOCOLOR}\n"
            let "leaked++"
        fi
    else
        if [[ "$result" -eq 0 ]]; then
            echo "running compiled program.."
            timeout 5 ./a.out
            result=$?
            rm "a.out"
            if [[ "$result" -eq 0 ]]; then 
                echo -ne "${GREEN}OK BRAK ZAPĘTLENIA${NOCOLOR}\n"
                let "correct++"
            else
                echo -ne "${RED}BŁĄD ZAPĘTLENIE${NOCOLOR}\n"
                let "leaked++"
            fi
        else
            echo "$OUTPUT" | head -n "$fedback_size"
            echo -ne "${RED}BŁĄD KOMPILACJI${NOCOLOR}\n"
            let "leaked++"
        fi
    fi
  }

temp_out=$(mktemp)
trap 'rm -f "$temp_out"' INT TERM HUP EXIT

traverse_folder "$tests/compilation_fail" 1
traverse_folder "$tests/compilation_success" 2
traverse_folder "$tests/infinite_loop" 3
echo -ne "${GREEN}CORRECT: $correct ${NOCOLOR}\n"
echo -ne "${RED}FAILED: $leaked ${NOCOLOR}\n"
echo -ne "TOTAL: $total\n"