Copyright © CloudBackend AB 2021.

This folder contains the libs relevant for the cloudbackend SDK.
Do not change the dependency libs meaning all poco and openssl libs, 
Cloudbackend can not ensure that any other versions of poco or openssl 
will work together with the Cloudbackend SDK.

Run the install script to get all libs installed at a system path that JVM can find;
in this case [usr/lib].

As an alternative to the lib directory /usr/lib
the lib files can be kept in any directory.
If so, from that lib directory and prior to calling the java program
execute the below export command

      export LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH


cbelib.jar is intended for installation of the sdk.  
com.cbe.jar is required for running programs once the sdk is installed.