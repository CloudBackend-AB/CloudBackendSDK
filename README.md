# CloudBackendSDK

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

SDK Android Version:

built on c++ version 1.3.5 of the SDK.

SDK Java Version:

built on c++ version 1.3.5 of the SDK.

Release notes JAVA/Android:

The java and android wrappers are built on the c++ sdk version 1.3.5 this includes share actions but not group actions.

current version SDK c++:
1.4.5

Release notes:
Main update for this release is join functionality on queries. ex: query(filter, delegate)->join(ContainerToQuery, key1, key2, filter, ContainerForResults)
For more details check the header file for QueryChain class.
Extended group functionality, added member functionality such as kick ban unban and listing of banned members on the group object.

SDK c++ Version:
1.4.1

Release notes:
Extended group functionality, added create, join, listMembers, leave, remove, rename on the group object together with relevent data members like bool joined().
On GroupManager added listing functionality searchGroups and a vector of currently joined groups on the GroupManager.
Added classes for group searches includes GroupFilter and GroupQuery classes, these handle like QueryResult, the result of a group search where both joined and non-joined groups may appear.

SDK c++ Version:
1.4.0

Release notes:
Bug fixes, listGroups on groupManager added. CreateAccount added.
New CLI with new extended Readme. 
