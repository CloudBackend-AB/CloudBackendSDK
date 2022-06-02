# CloudBackendSDK 1.4.6.7

This repository contains the CloudBackend SDK.  The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.  The repository currently contains a build for linux x86_64.

To learn more about cloudbackend visit:
https://cloudbackend.com/

SDK documentation at:
https://cloudbackend.com/docs/APIov.html

include: To build with the CloudBackend SDK include the libCloudbackend.a file and headers from this repository.

Examples: contains a simple example program and build script.

UI: XIOS/3 partner browser User Interface bundle 


Accounts: 
The following are Test accounts that can be used to login to CloudBackend:

githubtester1 : gitHubTester1password || 
githubtester2 : gitHubTester2password || 
githubtester3 : gitHubTester3password 

Tenant / Signup source: cbe_githubtesters

Note: These test accounts can be used by anyone with access to this repository so it is advisable to create a container to work in and not upload anything private.

Change the user account for the sample program in user_credentials.cpp

Notes: This SDK has been built and tested on Ubuntu

---------------------------------------------------------------------------------------------------------------------------------------------------
# Android
current version Android SDK:
1.3.5

Release notes:
built on c++ version 1.3.5 of the SDK.

---------------------------------------------------------------------------------------------------------------------------------------------------
# Java
current version Java wrapper SDK:
1.4.6.7

Release Notes:
Due to a wrapper missmatch in callbacks, TransferEventProtocol does not exists anymore. TransferEventProtocol was a combined class of the protocols TransferUploadEvent and TransferDownloadEvent. Now the two protocols mentioned are instead used seperatly.

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
# C++
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

---------------------------------------------------------------------------------------------------------------------------------------------------
