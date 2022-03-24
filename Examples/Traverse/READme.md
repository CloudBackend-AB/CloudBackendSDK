# Traverse example code.

Copyright © CloudBackend AB 2021-2022.

Release notes
=============

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
It uses: AccountEventProtocol, ItemEventProtocol.
Function processContainer uses C++ lambda expressions.


Second program will also traverse any shares given by other users.
Users 1 and 3 have shared one container each with user 2.
The repeating mutex lock code is implemented as template Sync.
It uses: AccountEventProtocol, ItemEventProtocol, ShareEventProtocol.
Function processContainer uses C++ lambda expressions.


Third program will also build a map of objects with their id.
It uses: AccountEventProtocol, ItemEventProtocol, ShareEventProtocol.
Functions processContainer and buildPath use C++ lambda expressions.


Fourth program
It uses: AccountEventProtocol, ItemEventProtocol, ShareEventProtocol.
Functions processContainer, buildPath and processItem use C++ lambda expressions.


Fifth program
It uses: AccountEventProtocol, ItemEventProtocol, ShareEventProtocol.
Functions processContainer, buildPath and processItem use C++ lambda expressions.
Include more descriptive information on some errors.


Acknowledgement:
Many thanks to Forware AB for valuable contribution to the example code here.
