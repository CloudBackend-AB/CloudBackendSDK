# Traverse example code.

CloudBackend AB 2021-2023.

Release notes
=============

Release 2023-07-10
------------------
* The source code has been updated to the current version of the SDK C++.

Release 2022-03-24
------------------
* The source code has been updated to the current version of the SDK C++.

Release 2021-09-02
------------------
* Improved output.

Content
=======

Traverse is a group of programs demonstrating 
advanced examples of C++ coding techniques
for the CloudBackend SDK.

The programs will recusively traverse the root container hierarchy.
Each program will introduce new coding practices, 
building on the earlier programs levels.


First program uses mutex locks that will wait for the callback.
It uses: LogInDelegate, QueryDelegate.
Function processContainer uses C++ lambda expressions.
User 1.


Second program will also traverse any shares given by other users.
Users 1 and 3 have shared one container each with user 2.
The repeating mutex lock code is implemented as template Sync.
It uses: ListSharesDelegate.
Function processContainer uses C++ lambda expressions.
User 2.


Third program will also build a map of objects with their id.
It uses: LogInDelegate, QueryDelegate, ListSharesDelegate.
Functions processContainer and buildPath use C++ lambda expressions.
User 3.


Fourth program
It uses: LogInDelegate, QueryDelegate, ListSharesDelegate.
Functions processContainer, buildPath and processItem use C++ lambda expressions.
User 1.


Fifth program
It uses: LogInDelegate, QueryDelegate, ListSharesDelegate.
Functions processContainer, buildPath and processItem use C++ lambda expressions.
Includes more descriptive information on some errors.
User 2.


Acknowledgement:
Many thanks to Forware AB for valuable contribution to the example code here.
