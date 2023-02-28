#!/usr/bin/bash
# installJavaSDK.sh
# CloudBackend 2023-02-28.
echo "Installing CloudBackend Java SDK."
echo -n "Checking Java version: "
javac --version                             # check javac version
read -p "Do you want to install openjdk-17-jdk-headless (y/N) ? " Answer
if [ "${Answer}" = "y" ];
then
  sudo apt-get install openjdk-17-jdk-headless # if not already installed
fi
echo
echo -n "Unpacking lib "
cd lib                                      # SDK Java
bash install.sh                             # install the SDK Java lib
cd ..
echo
echo "Compile and run Java SDK installation test"
cd Examples/Simple
bash compile.sh                             # compile the installation test program
bash run.sh                                 # run the installation test program
