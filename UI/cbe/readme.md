### CloudBackend UI additions.

#### User Interface - web browser tool

This is the SDK used in the dashboard, see
https://db.cloudbackend.com/

----

After login select:
1. Database home and check for file in home POI.
<br>(If not present, upload)
  - list.xml

1. Applications and check for
<br>(If not present, upload)
  - 1L-home-POI-list.xml
  - 2P_home-POI-list.xml

**Double click on one App to start it.**

----

#### CLI
Alternatively, in the dashboard CLI shell:
```
cd /POI
ls
```
if file is not already there:
```
upload data/list.xml
type /POI/list.xml
```

Applications:
```
cd tenant://
cd Applications
ls
```
if files are not already there:
```
upload cbe/1L-home-POI-list.xml
upload cbe/2P_home-POI-list.xml
```

----
#### Copyright © CloudBackend AB 2022.
----

#### XIOS/3
is a partner that provides a web browser UI that includes integration with 
the CloudBackend service.

#### local server

To run the UI on your own web server, please contact us for more information.

----
