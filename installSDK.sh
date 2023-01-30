#!/usr/bin/bash
# installSDK.sh
# CloudBackend 2022-2023.
echo "Installing CloudBackend SDK."
mkdir -p ~/cbe                              # if not already created
mydir=${PWD}
echo "moving" ${mydir} "to ~/cbe"
mv ${mydir} ~/cbe/2.0.1
cd ~/cbe
ls -l current
rm -i current                               # remove old symbolic link
ln -s 2.0.1 current                         # create symbolic link to the current version of the SDK
echo "done."

echo ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
read -p "Next C++. Press ENTER " Answer
cd ~/cbe/current                            # the release directory
cd C++                                      # SDK C++
pwd
bash installC++SDK.sh                       # install script
echo "done."

echo ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
read -p "Next Java. Press ENTER " Answer
cd ~/cbe/current                            # the release directory
cd Java                                     # SDK Java
pwd
bash installJavaSDK.sh                      # install script
echo "done."

echo ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
read -p "Next CLI. Press ENTER " Answer
echo "Installing CloudBackend CLI."
cd ~/cbe/current                            # the release directory
cd tools                                    # cli
pwd
unzip new_cli.zip                           # unpack the new_cli
./new_cli -uc gh1  << EOF                   # start new_cli with user credentials of githubtester1
 help
 set
 logout
EOF
echo "done."
cd
ls -l cbe/current
cd cbe/current
pwd

