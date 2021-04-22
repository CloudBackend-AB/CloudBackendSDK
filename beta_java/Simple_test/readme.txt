Copyright © CloudBackend AB 2021.

This has been tested on Ubuntu LTS 20.04 with OpenJDK 11.

To install the CloadBackend java package; first time only:

sudo apt update
sudo apt install openjdk-11-jdk
cd lib
cat  install.sh  # see alternative directory choise instructions in that file
bash install.sh
cd ..

To test the installation, run the provided test program:

cd Simple_test
bash Compile*


The last line will test the code as below; 
make sure the com.cbe.jar is in the same folder as this test.

Compile: javac -cp .:com.cbe.jar *.java
Run:     java  -cp .:com.cbe.jar runme 
