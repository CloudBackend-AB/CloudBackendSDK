#!/usr/bin/bash
# installC++SDK.sh
# CloudBackend 2024-02-16.
echo "Installing CloudBackend C++ SDK."
echo -n "Checking g++ version: "
g++ --version                               # check that the compiler is installed
read -p "Do you want to install g++ (y/N) ? " Answer
if [ "${Answer}" = "y" ];
then
  sudo apt-get install g++                  # if not already installed
fi
cd lib/Linux_x86
pwd
if [ -f libcb_sdka.zip ];
then
    unzip -DD libcb_sdka.zip
    if [[ $? -eq 0 ]];
    then
        rm libcb_sdka.zip
    fi
fi
if [ -f libcb_sdkso.zip ];
then
    unzip -DD libcb_sdkso.zip
    if [[ $? -eq 0 ]];
    then
        rm libcb_sdkso.zip
    fi
fi
pwd
ls -lh lib*
echo
cd -
echo "Compile and run C++ installation test"
cd Examples/SimpleAsyncDelegates            # Simple code example directory
sh compile.sh                               # compile the test program
sh run.sh  << EOF                           # run the installation test program and then enter name
Adam2
y
EOF
