# CloudBackendSDK 2.0
This repository contains the CloudBackend SDK, a build for Linux x86_64 Ubuntu 22 LTS.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see documentation links via:
https://cloudbackend.com/docs.html

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
## Test installation
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

## Release notes
### Current version SDK C++: **2.0.0**

- The Protocols shared pointers have been replaced with object classes and more detailed Delegate classes.

### Older versions

#### SDK C++ version: 1.4.6.4

- Support for < and > in filter.

#### SDK C++ version: 1.4.6.3

- Minor bug fixes.

#### SDK C++ version: 1.4.6.2

- Repackaged directory structure

#### SDK C++ version: 1.4.6

- Fix for queryWithPath. API reachable both on CloudBackend class object and Container class objects.
- Additional API from version 1.4.5 is the  
    - `QueryChainPtr queryWithPath(std::string relativePath, ItemDelegatePtr delegate, uint64_t queryRoot) {}`
    - `QueryChain queryWithPath(std::string relativePath, ItemDelegatePtr delegate) {}` 
  
  Returns the container with items on the specified cloud path relative to either rootContainer or a specified container (that will act as root for the path).

#### SDK C++ version: 1.4.5

- Main update for this release is join functionality on queries. e.g.: 
    -   `query(filter, delegate)->join(ContainerToQuery, key1, key2, filter, ContainerForResults)`

 For more details check the header file for QueryChain class or online on cloudbackend.com.

- Search has been added as a function of object search on whole account, e.g. <br> `search(string tag, containerId, delegate)` will search the container structure (full container tree) for objects with the key and/or key:value. e.g. : `search("artist|year:2006", rootContainerId, delegate)` will return all objects that has the key artist and the key year with value 2006.

- There is also `search(CBE::Filter filter, ItemDelegatePtr delegate)` on container, where you define your own filter for the search. bare in mind that a queryString would need to be set to filter out objects on the account, unless one only wants to see all objects in a ascending or decending order for instance.

- Extended group functionality, added member functionality such as kick ban unban and listing of banned members on the group object.

#### SDK C++ version: 1.4.1

- Extended group functionality, added create, join, listMembers, leave, remove, rename on the group object together with relevent data members like bool joined().
- On GroupManager added listing functionality searchGroups and a vector of currently joined groups on the GroupManager.
- Added classes for group searches includes GroupFilter and GroupQuery classes, these handle like QueryResult, the result of a group search where both joined and non-joined groups may appear.

#### SDK C++ version: 1.4.0

- Bug fixes, listGroups on groupManager added. CreateAccount added.
- New CLI with new extended Readme. 

----
#### Copyright © CloudBackend AB 2020-2022.
