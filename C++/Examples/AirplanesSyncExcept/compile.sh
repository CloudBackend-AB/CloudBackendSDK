#!/usr/bin/sh
# compile.sh <excercise>
# release 2023-02-10

PARENTSCRIPT_PATH="$(dirname "$0")"
cd ${PARENTSCRIPT_PATH}
echo ${PWD}

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
    COMPILER_COMMAND="g++ -std=c++11 -pthread "
    # libCBE="../../lib/Linux_x86/libcb_sdk.so"
    libCBE="../../lib/Linux_x86/libcb_sdk.a"
    CODE_PATH="./"
    ;;

    # "armv6l")
    # COMPILER_COMMAND="arm-linux-gnueabihf-g++ -mcpu=arm1176jzf-s -mfpu=vfp -fPIC -ldl -pthread "
    # libCBE="libCloudBackend-rpi.a"
    # ;;

    # "armv7l")
    # COMPILER_COMMAND="arm-linux-gnueabihf-g++ -mcpu=cortex-a53 -mfpu=neon-vfpv4 -fPIC -ldl -pthread "
    # libCBE="libCloudBackend-rpi.a"
    # ;;

    *)
    uname -a
    echo "platform not supported in this release"
    exit 1
    ;;
esac

case "$exercise" in
  "1")
    echo "compile example code."
     ${COMPILER_COMMAND} -o "airplanes" "${CODE_PATH}main.cpp" "${CODE_PATH}Airplanes.cpp" ${libCBE} -I "../../include" -ldl
    if [ $? -eq 0 ]
    then
        echo "to run use: sh run.sh"
    else
        echo "Error encountered."
    fi
    ;;

    *)
    # Default.
    echo "syntax: "$0" <1-7> "
    ;;
esac
