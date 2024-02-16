#!/bin/sh
# CloudBackend AB 2023.
# compile.sh #
# version 2023-02-03
# This compiler script has sections corresponding to
# the different exercises in the Tutorial # on the website. 
# Submit the number of the Exercise to compile.
# Example: sh compile.sh 1

if [ $# -gt 0 ]
then
    exe=$1
else
    exe="0"
fi
ARCH=`uname -m`
echo "identified computer architechture:" ${ARCH}
case "$ARCH" in
    "x86_64")
    COMPILER_COMMAND="g++ -std=c++17 -pthread "
    WARNINGS="-Wpedantic -Wall -Wextra -Weffc++ -Wsuggest-override -Wno-unused-parameter"
    # your path to the copy of the SDK binary lib
    # libCBE="../../lib/Linux_x86/libcb_sdk.so"
    # export LD_LIBRARY_PATH="../../lib/Linux_x86/"
    libCBE="../../lib/Linux_x86/libcb_sdk.a"
    ;;

    *)
    echo ${ARCH} "platform is not supported in this release"
    exit 1
    ;;
esac

echo "about to compile exercise:" ${exe}
case "${exe}" in
    1|2|3)
    # Tutorial exercise 1, 2, 3:
    ${COMPILER_COMMAND} ${WARNINGS} -o tut${exe} Main.cpp Logic${exe}.cpp ${libCBE} -I "../../include" -ldl
    command_result=$?
    ;;
    
    *)
    # Default.
    echo "syntax : "$0" <1-3> "
    echo "example: "$0" 1"
    exit 2
    ;;
esac

if [ ${command_result} -eq 0 ]
then
    echo "to run use: sh run.sh" ${exe}
else
    echo "compilation had errors"
fi
