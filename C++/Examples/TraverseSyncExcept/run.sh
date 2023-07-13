#!/bin/sh
# run.sh #
# This run script has sections corresponding to the different exercises in the Tutorial on the website.
# Submit the number of the exercise to compile.
# example: sh run.sh 1

echo "CloudBackend SDK is provided under a limited evaluation licence "
echo "that is not for production use."

if [ $# -gt 0 ]
then
  exe=$1
else
  exe="1"
fi
ARCH=`uname -m`
echo "computer architechture:" ${ARCH}
export LD_LIBRARY_PATH="../../lib/Linux_x86"
case "$exe" in
  1|2|3|4|5)
  echo "run exercise:" ${exe}
  ./cb_traverse${exe}
  ;;
  
  *)
  # Default.
  echo
  echo "syntax: "$0" <1-5> "
  echo "example: sh run.sh 1"
  exit 1
  ;;
esac
