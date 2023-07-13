#!/usr/bin/sh
# run.sh
# release 2023-07-11

echo "CloudBackend SDK is provided under a limited evaluation licence."
echo "Not for production use."

export LD_LIBRARY_PATH="../../lib/Linux_x86"
././partition_large_csv_file $*

