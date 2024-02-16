# compile.sh <excercise>
# release 2023-07-05
parentscript_path="$(dirname "$0")"
cd "$parentscript_path"

if [ $# -gt 0 ]
then
    exercise=$1
else
    exercise="1"
fi
arch=`uname -m`
case "$arch" in
    "x86_64")
    compiler_command="g++ -std=c++11 -pthread "
    #libcbe="../../lib/linux_x86/libcb_sdk.so"
    libcbe="../../lib/Linux_x86/libcb_sdk.a"
    code_path="./"
    ;;

    *)
    exit 1
    ;;
esac

case "$exercise" in
  "1")
  # section 1:
    echo "compile example code."
     ${compiler_command} -o "mediumExample" "${code_path}MediumExample.cpp" ${libcbe} -I "../../include" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh"
    fi
    ;;

    *)
    # default.
    echo "syntax: "$0" <1-7> "
    ;;
esac

