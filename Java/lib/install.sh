#!/usr/bin/bash
# CloudBackend 2024-02-16.
# unpack the compressed lib

if [ -f libcb_sdk_shared_wrapper.zip ];
then
    echo -n "Unpacking "
    unzip -DD libcb_sdk_shared_wrapper.zip
    if [[ $? -eq 0 ]];
    then
        rm libcb_sdk_shared_wrapper.zip
    fi
else
    echo "No compressed lib package found."
fi
pwd
ls -lh libcb_*

exit

# optional:
# Run the install script below to get all libs installed at 
# a system path that JVM can find, in this case [usr/lib].

jar xf com.cbe.jar 

sudo cp lib* /usr/lib 
