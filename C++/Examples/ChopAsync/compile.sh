# compile.sh <excercise>
# release 2022-04-08

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
echo "computer architechture ${ARCH}"
case "$ARCH" in
    "x86_64")
    COMPILER_COMMAND="g++ -std=c++17 -pthread "
    #libCBE="../../lib/Linux_x86/libcb_sdk.so"
    libCBE="../../lib/Linux_x86/libcb_sdk.a"
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
     ${COMPILER_COMMAND} -o "partition_large_csv_file" "${CODE_PATH}partition_large_csv_file.cpp" ${libCBE} -I "../../include" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: ./partition_large_csv_file -?"
    fi
    ;;

    *)
    # Default.
    echo "syntax: "$0" <1-7> "
    ;;
esac
