Copyright © CloudBackend AB 2020.

In the SimpleEx.h and .cpp you will find all the code for the test program built with CloudBackend AB:s Software Development kit.
There is a EntryPoint in the .cpp where you can read and follow the code chain together with comments to support how to use the library.

In user_credentials you need to fill in, copy paste user info from within that file, the user credentials that you are going to use.
You are free of choice which to use but remember multiple people might use the same account at the same time. 

Due to multiple people accessing the same user credentials we ask you to input through the terminal a name for the container in which an upload will occure,
make sure you have a nameing convention in place at your company but dont worry if the same name is used twice in a row the program will delete the already made 
container and create it again, as it do so the removed containers content, namly an uploaded file is also removed. However CloudBackend AB does not take responsibillity
for niether multiple people createing the same container at the same time and clashing with server requests causing errors you did not expect, they should be handled correctly although.
Also any information that you decide to put up yourself on a test account is strongly disadviced and the loss of those objects in the cloud will not held responsible by CloudBackend AB. 