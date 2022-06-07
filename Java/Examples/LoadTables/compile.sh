# Copyright © CloudBackend AB 2022.
# Compile and Run Java CBE program.
# Dependent on the libcbe.zip being installed,
# which contains the .so library for the java sdk. 
# You can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# compile
mkdir -p xclass
javac -cp '.:../../lib/com.cbe.jar' -d xclass/ *.java

result=$?

if [ $result -gt 0 ]
then
    echo "Halting due to error returned:" $result
    exit $result
fi

# run
# java -Djava.library.path=../../lib/ -cp 'xclass:../../lib/com.cbe.jar' LoadTable $*
