#!/usr/bin/sh

# Copyright © CloudBackend AB 2022.
# Compile and Run Java CBE program.
# Dependent on the lib/libcbe.jar being installed,
# which contains the .so libraries for the java sdk. 
# You can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# compiling
javac -cp '.:../../../../../lib/com.cbe.jar' cbe/tutorial/*.java
result=$?

if [ $result -gt 0 ]
then
    echo "Halting due to error returned:" $result
    exit $result
fi

# running
java -Djava.library.path=../../../../../lib/ -cp '.:../../../../../lib/com.cbe.jar' cbe/tutorial/Main
