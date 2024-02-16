![version](https://badgen.net/badge/version/2.1.3/green)
![platform](https://badgen.net/badge/platform/ubuntu)
![languages](https://badgen.net/badge/language/C++,Java,Android/purple?list=|)

# CloudBackendSDK 2.1.3
This repository contains the CloudBackend SDK, a build for Linux x86_64 Ubuntu 22 LTS.
It contains both the synchronous and the asynchronous API.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see developer documentation links via:
https://cloudbackend.com/

### SDK Content
languages: Java and C++, each containing lib and example code.

### Database service in the cloud
Available to use to create, inspect and clean up test data and its structure.

Login to the database service at
https://db.cloudbackend.com/

### Accounts
The following are Test accounts that can be used to login to CloudBackend:

| username | password | tenant |
| -------- | -------- | ------ |
| githubtester1 | gitHubTester1password | cbe_githubtesters | 
| githubtester2 | gitHubTester2password | cbe_githubtesters | 
| githubtester3 | gitHubTester3password | cbe_githubtesters | 

Note: These test accounts can be used by anyone with access to this repository,
so it is advisable to create a container to work in and not upload anything private.

The test account have some limitations. To get your own tenant account see
https://cloudbackend.com/developer.html#how

Tenant is a.k.a. signup source

Change the user account for the sample program in user_credentials.cpp

Notes: Recommended platform for this SDK is Linux x86_64 Ubuntu 22 LTS,

------------------------------------------------------------------------

# Installation

### x86_64 Ubuntu 20.04 LTS & 22.04 LTS

Click on
Code : Download ZIP

then do:
```
cd
cd Downloads                                # or where you downloaded the package
unzip CloudBackendSDK-2.1.3.zip             # or the zip file name
cd CloudBackendSDK-2.1.3
bash installSDK.sh
```

---
## Java
Previous script calls
- Java/installJavaSDK.sh

---
## C++
Previous script calls
- C++/installC++SDK.sh

------------------------------------------------------------------------
### Relese notes
- See each language folder
----

#### Copyright © CloudBackend AB 2020-2024.

