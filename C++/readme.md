# CloudBackendSDK 2.1.3
This repository contains the CloudBackend SDK, a build for Linux x86_64 Ubuntu 22 LTS.
It contains both the synchronous and the asynchronous API.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see developer documentation links via:
https://cloudbackend.com/

# Installation

## x86_64 Ubuntu 20.04 LTS & 22.04 LTS

The installation script which will do
```
cd ~/cbe/current                            # the release directory
cd C++                                      # SDK C++
pwd
bash installC++SDK.sh                       # install script
```

------------------------------------------------------------------------

## Release notes
2024-02-12
### Current version

#### SDK C++ version: 2.1.3

- Updated library with improved network security.
- Tidied up the demo example data paths.
- Bug fixes.

#### SDK C++ version: 2.1.2

- Updated lib with improved password encryption.
- CreateAccount() method now returns userId instead of CloudBackend object.
- Tidied up the test files.
- Bug fixes.

#### SDK C++ version: 2.1.1

- Corrections in some of the examples.
- Definition of AclMap has been extended to handle acl for both user and group.
  getAcl() and setAcl() modified accordingly.
- Added missing synchronous calls.
- Updates to the doxygen documentation.
- Updated lib.

#### SDK C++ version: 2.1.0

- Minimum operating system: Linux Ubuntu 20.04 LTS.
- Added synchronous api.
  Methods with same name but slight change in parameters compared to the async call.
- Account::tenantContainerId() added.
- Container::createObject() change in the order of the parameters.
- Object::updateKeyValues() change in the order of the parameters.
- Additional example programs.

### Older versions
#### SDK C++ version: 2.0.2

- Examples with improved error handling


#### SDK C++ version: 2.0.1

- New installation script

#### SDK C++ version: 2.0.0

- The Protocols shared pointers have been replaced with object classes and more detailed Delegate classes.

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
#### Copyright © CloudBackend AB 2020-2023.
