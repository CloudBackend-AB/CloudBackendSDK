### CloudBackend SDK Java API.

**lib** contains the jar files needed for installing and running the SDK.
```
cd lib
bash install.sh
```

**Example** provides a number of code that can be run to learn how to use the API.

**resources** contains information used by the Example code, e.g. user credentials 

This has been tested on Ubuntu LTS 22.04 with OpenJDK 17.
<br>To install:
```
sudo apt install openjdk-17-jre-headless
jar --version
java --version
```

---------------------------------------------------------------------------------------------------------------------------------------------------
## Release notes
### Current version SDK Java: **2.0.1**

- Updated Example code.
- Installation script.

### Older versions

### Current version SDK Java: **2.0.0**

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

#### Copyright © CloudBackend AB 2021-2022.
