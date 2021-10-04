# Copyright © CloudBackend AB 2021.
# The libcbe.jar contains the .so:s/ libraries for the java sdk. you can set -Djava.library.path=path/to/libraries if you want to have them somewhere else than the standard path. 

javac -cp '.:com.cbe.jar' *.java
java -cp '.:com.cbe.jar' runme 
