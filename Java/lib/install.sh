jar xf libcbe.jar 

sudo cp lib* /usr/lib 

exit

# the following is optional
shopt -s extglob
rm *.!(txt|jar|sh)
