# Copyright © CloudBackend AB 2021-2022.
# The libcbe.jar contains the .so library for the java sdk.
# It can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# compile
javac -cp '.:../../lib/com.cbe.jar' *.java

result=$?

if [ $result -gt 0 ]
then
    echo "Halting due to error returned:" $result
    exit $result
fi

# run 
java -Djava.library.path=../../lib/ -cp '.:../../lib/com.cbe.jar' RunMe
