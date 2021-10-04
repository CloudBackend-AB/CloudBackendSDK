# CloudBackendSDK

This repository contains the CloudBackend SDK.  The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.  The repository currently contains a build for linux x86_64.

To learn more about cloudbackend visit:
https://cloudbackend.com/

SDK documentation at:
https://cloudbackend.com/docs/APIov.html

To build with the cloudbackend SDK include the libCloudbackend.a file and headers from this repository.

Example code - contains a simple example program and build script.

Accounts: 
The following are Test accounts that can be used to login to CloudBackend:

githubtester1 gitHubTester1password
githubtester2 gitHubTester2password
githubtester3 gitHubTester3password

Signup source: cbe_githubtesters

Note: These test accounts can be used by anyone with access to this repository so it is advisable to create a container to work in and not upload anything private.

Change the user account for the sample program in user_credentials.cpp

Notes: This SDK has been built and tested on Ubuntu

SDK Android Version:

built on c++ version 1.3.5 of the SDK.

SDK Java Version:

built on c++ version 1.3.5 of the SDK.

Release notes JAVA/Android:

The java and android wrappers are built on the c++ sdk version 1.3.5 this includes share actions but not group actions.

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
