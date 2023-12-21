# CloudBackend AB 2022-2023.

## This is a simple Example of a test program.

In the SimpleEx.cpp you will find all the code for the test program
built with CloudBackend AB:s Software Development Kit.

You can follow the code in the main function in file **SimpleEx.cpp**
together with comments to support how to use the library.

In **user_credentials.cpp** you need to fill in, copy paste user info
from within that file, the user credentials that you are going to use. 
There are three to choose from and you are free to choose any of them
but remember multiple people might use the same account
at the same time. 

Due to multiple people accessing the same user credentials, we ask you 
to input through the terminal a name for the container in which an 
upload will occur.
Make sure you have a naming convention in place at your company as 
container names has to be unique. If you try to create a container with 
a name that already exists the program will give you an error and then 
terminate.

However CloudBackend AB does not take responsibility for neither 
multiple people creating the same container at the same time and 
clashing with server requests causing errors you did not expect,
they should be handled correctly though.

Also any sensitive information that you decide to put up yourself on a 
test account is strongly advised against.
CloudBackend AB can not be held responsible for any loss of data in 
these test accounts.

### Run test

Compile with `sh compile.sh` and then run with `sh run.sh`

**If you want to remove a container, use the CLI.**
 
CLI command `rmdir yourContainerName`

Release 2023-07-10
------------------
* The source code has been updated to the current version of the SDK C++.

