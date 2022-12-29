# CloudBackend AB 2021-2022.
# The libcbe.jar contains the .so library for the java sdk.
# It can set -Djava.library.path=path/to/libraries
# if you want to have them somewhere else than the standard path. 

# run 
java -Djava.library.path=../../lib/ -cp 'classes:../../lib/com.cbe.jar' Pickout
