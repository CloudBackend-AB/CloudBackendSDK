#!/usr/bin/bash
# unpack the compressed lib

unzip libcbeJava.zip

exit

# optional:
# Run the install script below to get all libs installed at 
# a system path that JVM can find, in this case [usr/lib].

jar xf libcbe.jar 

sudo cp lib* /usr/lib 
