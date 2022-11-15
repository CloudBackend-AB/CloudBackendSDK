![version](https://badgen.net/badge/version/2.0.0/green)
![platform](https://badgen.net/badge/platform/ubuntu)
![languages](https://badgen.net/badge/language/C++,Java,Android/purple?list=|)
# CloudBackendSDK 2.0
This repository contains the CloudBackend SDK, a build for Linux x86_64 Ubuntu 22 LTS.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see developer documentation links via:
https://cloudbackend.com/

### SDK Content
languages: Java and C++, each containing lib and example code.

tools: cli, dashboard example files and test data files.

UI: XIOS/3 partner browser User Interface bundle extension for login to CloudBackend.

### Dashboard
Login to the dashboard at
https://db.cloudbackend.com/

### Accounts
The following are Test accounts that can be used to login to CloudBackend:

| username | password | tenant |
| -------- | -------- | ------ |
| githubtester1 | gitHubTester1password | cbe_githubtesters | 
| githubtester2 | gitHubTester2password | cbe_githubtesters | 
| githubtester3 | gitHubTester3password | cbe_githubtesters | 

Note: These test accounts can be used by anyone with access to this repository,
so it is advisable to create a container to work in and not upload anything private.

Tenant a.k.a. signup source

Change the user account for the sample program in user_credentials.cpp

Notes: This SDK has been built and tested on x86_64 Ubuntu 22 LTS

---------------------------------------------------------------------------------------

# Installation

## x86_64 Ubuntu 22.04 LTS

```
sudo apt install g++                        # if not already installed
cd                                          # the home directory or equivalent
mkdir -p cbe                                # if not already created
cd cbe                                      # replace with your project directory
unzip ~/Downloads/CloudBackendSDK-main.zip  # replace with the zip file name previously downloaded
mv CloudBackendSDK-main 2.0                 # release number or another name of your choice
ln -s 2.0 current                           # create symbolic link to the current version of the SDK
cd current                                  # the name previously given this release's directory
cd tools                                    # cli
unzip new_cli.zip                           # unpack the new_cli
./new_cli -uc gh1                           # start new_cli with user credentials of githubtester1
help
set
logout
```

---
## Java
```
javac --version                             # check that javac is installed
sudo apt-get install openjdk-17-jdk         # if not already installed
cd ~/cbe/current                            # the directory of this release
cd Java/lib                                 # SDK Java
bash install.sh                             # install the SDK Java lib
cd ..
cd Examples/Simple
bash compile.sh                             # compile the installation test program
bash run.sh                                 # run the installation test program
```

---
## C++
```
g++ --version                               # check that the compiler is installed
cd                                          # the home directory
cd cbe/current                              # the name previously given this release's directory
cd C++                                      # SDK C++
cd Examples/Simple                          # Simple code example directory
sh compile.sh                               # compile the test program
sh run.sh                                   # run the installation test program and then enter name
...
Name for a new Container to be created: Adam2
...
/Adam2            created.
/Adam2/w2.xml     uploaded.        
Do you want to delete your newly created container called "Adam2"?
(y/n): y
Deleting container.
Container was deleted successfully!
End of program!
```

---------------------------------------------------------------------------------------------------------------------------------------------------
### Relese notes
- See each language folder
----
#### Copyright © CloudBackend AB 2020-2022.
