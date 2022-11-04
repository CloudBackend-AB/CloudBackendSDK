# CloudBackend AB 2022.
# Compile and Run Java CBE program.
# Dependent on the libcbe.zip being installed,
# which contains the .so library for the java sdk. 
# You can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# run
java -Djava.library.path=../../lib/ -cp 'classes:../../lib/com.cbe.jar' LoadTable $*
