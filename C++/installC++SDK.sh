#!/usr/bin/bash
# installC++SDK.sh
# CloudBackend 2022-12-29.
echo "Installing CloudBackend C++ SDK."
echo -n "Checking g++ version: "
g++ --version                               # check that the compiler is installed
read -p "Do you want to install g++ (y/N) ? " Answer
if [ "${Answer}" = "y" ];
then
  sudo apt-get install g++                  # if not already installed
fi
echo
echo "Compile and run Java C++ installation test"
cd Examples/Simple                          # Simple code example directory
sh compile.sh                               # compile the test program
sh run.sh  << EOF                           # run the installation test program and then enter name
Adam2
y
EOF
