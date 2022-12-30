#!/usr/bin/bash
# installSDK.sh
# CloudBackend 2022.
echo "Installing CloudBackend SDK."
cd                                          # the home directory or equivalent
mkdir -p cbe                                # if not already created
cd cbe                                      # replace with your project directory
unzip ~/Downloads/CloudBackendSDK-main.zip  # replace with the zip file name previously downloaded
mv CloudBackendSDK-main 2.0.1               # release number or another name of your choice
rm -i current                               # remove old symbolic link
ln -s 2.0.1 current                         # create symbolic link to the current version of the SDK

cd ~/cbe/current                            # the release directory
cd C++                                      # SDK C++
bash installC++SDK.sh                       # install script

cd ~/cbe/current                            # the release directory
cd Java                                     # SDK Java
bash installJavaSDK.sh                      # install script

echo "Installing CloudBackend CLI."
cd ~/cbe/current                            # the release directory
cd tools                                    # cli
unzip new_cli.zip                           # unpack the new_cli
./new_cli -uc gh1  << EOF                   # start new_cli with user credentials of githubtester1
 help
 set
 logout
EOF
