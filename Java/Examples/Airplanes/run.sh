# Copyright © CloudBackend AB 2021-2022.
# The libcbe.jar contains the .so library for the java sdk.
# Option -Djava.library.path=path/to/libraries can point to the SDK lib
# if you want to have them somewhere else than the given path. 

java -Djava.library.path=../../lib/ -cp 'classes:../../lib/com.cbe.jar' Airplanes