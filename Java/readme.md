### CloudBackend SDK Java API.

**lib** contains the lib files needed for installing and running the SDK.

**Example** provides a number of code that can be run to learn how to use the API.

**resources** contains information used by the Example code,
e.g. user credentials and the API java files.

This has been tested on Ubuntu 20.04 and 22.04 LTS with OpenJDK 17.
<br>To install:
```
sudo apt install openjdk-17-jre-headless
jar --version
java --version
```
or run the installation script which will do
```
cd ~/cbe/current                            # the release directory
cd Java                                     # SDK Java
pwd
bash installJavaSDK.sh                      # install script
```

------------------------------------------------------------------------
## Release notes
### Current version
#### SDK Java version: **2.1.3**

- Minimum operating system: Linux Ubuntu 20.04 LTS on x86_64.
- Minimum Java: openjdk-17-jre
- Updated lib with improved password encryption.

#### SDK Java version: **2.1.1**

- Minimum operating system: Linux Ubuntu 20.04 LTS on x86_64.
- Updated lib.
  getAcl() and setAcl() modified accordingly.
- Updates to the javadoc documentation.
- Account::tenantContainerId() added.
- Container::createObject() change in the order of the parameters.
- Object::updateKeyValues() change in the order of the parameters.
- AclMap has been extended to handle ACL for both user and group.
- Updates in the examples accordingly.


### Older versions
#### SDK Java version: **2.0.1**

- Updated Example code.
- Installation script.

#### SDK Java version: **2.0.0**

- The Protocols shared pointers have been replaced with object classes and more detailed Delegate classes.

#### SDK Java version: 1.4.6.7
- Due to a wrapper missmatch in callbacks, TransferEventProtocol does not exists anymore. 
- TransferEventProtocol was a combined class of the protocols TransferUploadEvent and TransferDownloadEvent. 
- Now the two protocols mentioned are instead used seperatly. 
- Example Simple added download of files.

#### SDK Java version: 1.4.6.4
- Support for < and > in filter.
- Example/Airplanes updated
- Example/MyGroups added

#### SDK Java version: 1.4.6.3
- Minor bug fixes.

#### SDK Java version: 1.4.6
- Built on C++ version 1.4.6 of the SDK.

#### SDK Java version: 1.3.5
- The Java wrappers are built on the C++ sdk version 1.3.5 this includes share actions but not group actions.

#### Copyright © CloudBackend AB 2021-2024.
