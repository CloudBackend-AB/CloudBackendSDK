# CloudBackendSDK 1.4.6.7
This repository contains the CloudBackend SDK  a build for Linux x86_64 Ubuntu 20 LTS.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see documentation at:
https://cloudbackend.com/docs.html

### SDK Content
languages: contains lib and example code.

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

Note: These test accounts can be used by anyone with access to this repository so it is advisable to create a container to work in and not upload anything private.

Tenant a.k.a. Signup source

Change the user account for the sample program in user_credentials.cpp

Notes: This SDK has been built and tested on x86_64 Ubuntu 20 LTS

---------------------------------------------------------------------------------------------------------------------------------------------------

# Installation

## x86_64 Ubuntu 20.04 LTS

```
sudo apt install g++                        # if not already installed
cd                                          # the home directory or equivalent
mkdir -p cbe                                # if not already created
cd cbe                                      # replace with your project directory
unzip ~/Downloads/CloudBackendSDK-main.zip  # replace with the zip file name previously downloaded
mv CloudBackendSDK-main 1.4.6.7             # release number or another name of your choice
ln -s 1.4.6.7 current                       # create symbolic link to the current version of the SDK
cd current                                  # the name previously given this release's directory
cd tools                                    # cli
unzip new_cli.zip                           # unpack the new_cli
./new_cli –uc gh1                           # start new_cli with user credentials of githubtester1
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
bash compile.sh                             # compile and run the installation test program
```

---
## C++
```
cd                                          # the home directory
cd cbe/current                              # the name previously given this release's directory
cd C++/lib/Linux_x86/                       # SDK C++ lib
unzip libcb_sdk.zip                         # unpack the libcb_sdk.a
cd ../../Examples                           # Example Code directory
cd Simple                                   # Simple code example directory
sh compile.sh                               # compile the test program
sh run.sh                                   # run the installation test program and then enter name e.g.

Name for a new Company Container: Adam
.
New container created: Adam
.
program complete
```

---------------------------------------------------------------------------------------------------------------------------------------------------
## Android
current version Android SDK:
1.4.8.0

Release notes:
Added features for using subscribe and publish container/objects when published users can subscribe to these Items and follow updates/notifications.

SDK Android version:
1.4.7.0

Release notes:
built on c++ with java wrappers version 1.4.7.0. New features are restRequest(...) and saopRequest(...) which bypasses the SDK to call any API on our service. the response comes back as a String which the user can parse to a XML/DOM object. 

SDK Android version:
1.3.5

Release notes:
built on c++ version 1.3.5 of the SDK.

---------------------------------------------------------------------------------------------------------------------------------------------------
## Java
current version Java wrapper SDK:
1.4.6.7

Release Notes:
Due to a wrapper missmatch in callbacks, TransferEventProtocol does not exists anymore. TransferEventProtocol was a combined class of the protocols TransferUploadEvent and TransferDownloadEvent. Now the two protocols mentioned are instead used seperatly. 
Example Simple added download of files.

SDK Java version:
1.4.6.4

Release Notes:
Support for < and > in filter.
Example/Airplanes updated
Example/MyGroups added

SDK Java version:
1.4.6.3

Release Notes:
Minor bug fixes.

SDK Java version:
1.4.6

Release Notes:
Built on c++ version 1.4.6 of the SDK.

SDK Java version:
1.3.5

Release notes JAVA/Android:
The java and android wrappers are built on the c++ sdk version 1.3.5 this includes share actions but not group actions.

-----------------------------------------------------------------------------------------------------------------------------------------------------
## C++
current version SDK c++:
1.4.6.4

Release notes:
Support for < and > in filter.

SDK c++ version:
1.4.6.3

Release notes:
Minor bug fixes.

SDK c++ version:
1.4.6.2

Release notes:
repackaged directory structure

SDK c++ version:
1.4.6

Release notes:
Fix for queryWithPath. API reachable both on CloudBackend class object and Container class objects.
Additional API from version 1.4.5 is the QueryChainPtr queryWithPath(std::string relativePath, ItemDelegatePtr delegate, uint64_t queryRoot) {}, QueryChain queryWithPath(std::string relativePath, ItemDelegatePtr delegate) {}. Returns the container with items on the specified cloud path relative to either rootContainer or a specified container (that will act as root for the path).

SDK c++ version:
1.4.5

Release notes:
Main update for this release is join functionality on queries. ex: query(filter, delegate)->join(ContainerToQuery, key1, key2, filter, ContainerForResults)
For more details check the header file for QueryChain class or online on cloudbackend.com.

Search has been added as a function of object search on whole account, ex search(string tag, containerId, delegate) will search the container structure (full container tree) for objects with the key and/or key:value. ex: search("artist|year:2006", rootContainerId, delegate) will return all objects that has the key artist and the key year with value 2006.

There is also search(CBE::Filter filter, ItemDelegatePtr delegate) on container, where you define your own filter for the search. bare in mind that a queryString would need to be set to filter out objects on the account, unless one only wants to see all objects in a ascending or decending order for instance.

Extended group functionality, added member functionality such as kick ban unban and listing of banned members on the group object.

SDK c++ version:
1.4.1

Release notes:
Extended group functionality, added create, join, listMembers, leave, remove, rename on the group object together with relevent data members like bool joined().
On GroupManager added listing functionality searchGroups and a vector of currently joined groups on the GroupManager.
Added classes for group searches includes GroupFilter and GroupQuery classes, these handle like QueryResult, the result of a group search where both joined and non-joined groups may appear.

SDK c++ version:
1.4.0

Release notes:
Bug fixes, listGroups on groupManager added. CreateAccount added.
New CLI with new extended Readme. 

