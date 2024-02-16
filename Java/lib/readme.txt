Copyright © CloudBackend AB 2021-2024.

This folder contains the libs relevant for the cloudbackend SDK. 
Run the install script to get all libs installed at a system path that JVM can find, in this case [usr/lib].
If you know how and want to change this location look in to the script or unpack and place the files where you want to.
You may need to run something like
export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH
Modify the statment for your environment.

Do not change the dependency libs, meaning all poco and openssl libs.
Cloudbackend cannot ensure that any other versions of poco or openssl will work together with the Cloudbackend SDK.

