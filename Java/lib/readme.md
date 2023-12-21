### CloudBackend SDK Java lib

This folder contains the lib and jar relevant for the CloudBackend SDK. 

Run the install script to get the SDK lib unziped.

If you know how and want to change this location look at the optional in
the install script or unpack and place the files where you want to.
<br>If so, you may need to prior to running the Java program, do something like
```
export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
```
Modify the statement for your environment.

See the Examples directories for some alternatives.

The SDK is built with dependency libs, i.e. poco and openssl.
<br>Cloudbackend cannot ensure that any other versions of poco or openssl will work together with the Cloudbackend SDK.

#### Copyright © CloudBackend AB 2021-2023.
