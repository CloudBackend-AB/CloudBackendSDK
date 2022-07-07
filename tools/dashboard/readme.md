## Dashboard example applications.
Add the apps to a container Applications.
A tenant administrator with write access to tenant://Applications can put apps there and they will then become visible in the Applications section.
Alternatively the apps can be uploaded to an indivitual users home://Applications and will then be available there.

app1 - app4 are started by looking up the applications and doing a double click on the respective icon.

app2 - app3 will the read the content of a single file and map the points:
home://POI/stations.xml

app3 & app4 will the read the content of a single file and map the shortest path between points:
home://POI/track.xml

app5 & app6 will read the key/value metadata of multiple objects and should be started from the dashboard cli:

load Applications/app5Lmt-title-Lat-Long.xml -d POI/drive/

load Applications/app6Pmt-title-Lat-Long.xml -d POI/drive/
