# CloudBackend AB 2021-2022.
# The libcbe.jar contains the .so library for the java sdk.
# Option -Djava.library.path=path/to/libraries can point to the SDK lib
# if you want to have them somewhere else than the given path. 

mkdir -p classes
javac -cp '.:../../lib/com.cbe.jar' -d classes/ *.java

result=$?

if [ $result -gt 0 ]
then
    echo "Halting due to error returned:" $result
    exit $result
fi
