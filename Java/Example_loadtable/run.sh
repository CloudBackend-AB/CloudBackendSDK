#!/usr/bin/sh

# Copyright © CloudBackend AB 2022.
# Compile and Run Java CBE prohram.
# Dependent on the libcbe.jar being installed,
# which contains the .so:s/ libraries for the java sdk. 
# You can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# compiling
# javac -cp '.:com.cbe.jar' *.java
# result=$?

# if [ $result -gt 0 ]
# then
#     echo "Halting due to error returned:" $result
#     exit $result
# fi

# running
java -cp '.:com.cbe.jar' loadtable $*
