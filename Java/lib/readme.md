Copyright © CloudBackend AB 2021-2022.

This folder contains the lib and jar relevant for the CloudBackend SDK. 
Run the install script to get the SDK lib unziped.

If you know how and want to change this location look in to the script or unpack and place the files where you want to.
You may need to run something like
export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
Modify the statment for your environment.
See the Examples directories for some alternatives.

The SDK is built with dependency libs, i.e. poco and openssl.
Cloudbackend cannot ensure that any other versions of poco or openssl will work together with the Cloudbackend SDK.
