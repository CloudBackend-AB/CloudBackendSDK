#!/usr/bin/sh
# run.sh
# release 2024-02-12

echo "CloudBackend SDK is provided under a limited evaluation licence."
echo "Not for production use."

export LD_LIBRARY_PATH="../../lib/Linux_x86"
./mediumExample

if [ $? -eq 0 ]; then
    date
    echo "List files"
    ls -l /tmp/Medium*
fi
