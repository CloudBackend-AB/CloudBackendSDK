#!/bin/sh
# CloudBackend AB 2022.
# compile.sh <excercise>
# version 2022-04-08

PARENTSCRIPT_PATH="$(dirname "$0")"
echo "$PARENTSCRIPT_PATH" 
cd "$PARENTSCRIPT_PATH"
echo $PWD

if [ $# -gt 0 ]
then
    exercise=$1
else
    exercise="1"
fi
ARCH=`uname -m`
echo "identified computer architechture:" ${ARCH}
case "$ARCH" in
    "x86_64")
    COMPILER_COMMAND="g++ -std=c++17 -pthread "
    # libCBE="../../lib/Linux_x86/libcb_sdk.so"    # export LD_LIBRARY_PATH="../../lib/Linux_x86/"
    libCBE="../../lib/Linux_x86/libcb_sdk.a"
    WARNINGS="-Wpedantic -Wall -Wextra -Weffc++ -Wsuggest-override -Wno-unused-parameter"
    CODE_PATH="./"
    echo $CODE_PATH
    ;;

    *)
    echo ${ARCH} "platform not supported in this release"
    exit 1
    ;;
esac

case "$exercise" in
  "1")
  # Section 1:
    echo "compile example code."
     ${COMPILER_COMMAND} ${WARNINGS} -o "cb_traverse1" "${CODE_PATH}cb_traverse1.cpp" ${libCBE} -I "../../include/" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh 1"
    fi
    ;;
  "2")
  # Section 2:
    echo "compile example code."
     ${COMPILER_COMMAND} ${WARNINGS} -o "cb_traverse2" "${CODE_PATH}cb_traverse2.cpp" ${libCBE} -I "../../include/" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh 2"
    fi
    ;;
  "3")
  # Section 3:
    echo "compile example code."
     ${COMPILER_COMMAND} ${WARNINGS} -o "cb_traverse3" "${CODE_PATH}cb_traverse3.cpp" ${libCBE} -I "../../include/" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh 3"
    fi
    ;;
  "4")
  # Section 4:
    echo "compile example code."
     ${COMPILER_COMMAND} ${WARNINGS} -o "cb_traverse4" "${CODE_PATH}cb_traverse4.cpp" ${libCBE} -I "../../include/" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh 4"
    fi
    ;;

  "5")
  # Section 5:
    echo "compile example code."
     ${COMPILER_COMMAND} ${WARNINGS} -o "cb_traverse5" "${CODE_PATH}cb_traverse5.cpp" ${libCBE} -I "../../include/" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh 5"
    fi
    ;;

  *)
    # Default.
    echo "syntax: "$0" <1-5> "
    ;;
esac
