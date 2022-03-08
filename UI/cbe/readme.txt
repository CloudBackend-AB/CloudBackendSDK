readme.txt for the CloudBackend extension to the XIOS/3 package.

See the tutorial pdf-document on preparing your system.

Unzip in the directory where XIOS/3 is installed: 
e.g. in C:\xios on your local Windows workstation, 
then start the local webserver in the Windows tool: cmd
cd C:\xios
web-server-localhost.bat


Start your Chrome web browser and go to:
http://localhost:8000/login.html

Use your CloudBackend credentials e.g.: githubtester3

After login in the web CLI shell:
system
dir /
dir /POI
load xios/apps/system/document.xml
type /POI/list.xml
load cbe/listPOI.xml
load cbe/plotPOI.xml
load cbe/listMy.xml -d /POI/list.xml
load cbe/plotMy.xml -d /POI/list.xml
