CloudBackend AB 2021-2022.

To test the code make sure the com.cbe.jar is in the same folder as this test.

Compile: javac -cp '.:com.cbe.jar' *.java
run: java -cp '.:com.cbe.jar' RunMe 

The com.cbe.jar contains a class that automatically loads the libcb_sdk_shared_wrapper.so
with all its dependencies so there is no need to write a load function.

As soon as the com.cbe.jar is loaded in the correct way
and java.library.path can find the .so:s,
the com.cbe classes can be used.

The .so is built with Ubuntu 22.04 and might have some dependencies regarding core libraries
linked with the Ubuntu version, so please do use the latest LTS Ubuntu version and have it updated before you test.
This issue might be solved in the future, by being statically linked into the sdk:s .so
(however not the poco and openSSL libraries).

The CloudBackend Java SDK has been tested with openjdk 17.
