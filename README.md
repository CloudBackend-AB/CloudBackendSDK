![version](https://badgen.net/badge/version/2.0.1/green)
![platform](https://badgen.net/badge/platform/ubuntu)
![languages](https://badgen.net/badge/language/C++,Java,Android/purple?list=|)
# CloudBackendSDK 2.0.1
This repository contains the CloudBackend SDK, a build for Linux x86_64 Ubuntu 22 LTS.
The CloudBackend SDK is a tool for application developers that provides a simple way to store data in the cloud.

To learn more about the SDK, see developer documentation links via:
https://cloudbackend.com/

### SDK Content
languages: Java and C++, each containing lib and example code.

tools: cli, dashboard example files and test data files.

UI: XIOS/3 partner browser User Interface bundle extension for login to CloudBackend.

### Dashboard
Login to the dashboard at
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

Tenant a.k.a. signup source

Change the user account for the sample program in user_credentials.cpp

Notes: This SDK has been built and tested on x86_64 Ubuntu 22 LTS

------------------------------------------------------------------------

# Installation

## x86_64 Ubuntu 22.04 LTS

```
cd
cd Downloads                                # or where you downloaded the package
unzip CloudBackendSDK-main.zip              # or the zip file name
cd CloudBackendSDK-main
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
#### Copyright © CloudBackend AB 2020-2023.
