# Command Line Interface to the CloudBackend core service.

For documentation see  https://cloudbackend.com/docs.html

Copyright © CloudBackend AB 2021-2022.
--------------------------------------

new_cli
=======

Release notes
=============

Release 2022-04-07
------------------
* Version 1.0
* New set of commands in line with the dashboard at https://db.cloudbackend.com/


-----

old cbe_CLI
===========

Release notes
=============

Release 2021-10-14
------------------
* Version 1.4.1
* Improved error message on network.

Release 2021-06-24
------------------
* Version 1.4
* Added loadGroups new feature for group data
* Added queryGroup to view group containers
* Command cd to set container
* Added queryCID to query on a container id
* Some minor errors have been corrected.


Release 2021-05-05
------------------
* Version 1.1
* Split the help command with subsections: container, object and query.
* Added command download object; to local filestore.
* Added command type object; type a text file. 
* Better capture of error conditions.
* Access to o/s shell commands with prefix !
* Recent command by !!
* Added color coding to the output types.
* Some minor errors have been corrected.
* Implemented parameters to the cbe_CLI command
*   -i for better readability when stdinput is redirected from a file
*   -c for reading user_credentials file in the directory .cbe
* example file .cbe/gh2
  username = "githubtester2";
  password = "gitHubTester2password";
  tenant   = "cbe_githubtesters";
  # keep your p/w safe. If omitted, you will be prompted for it.


Release 2021-03-25
------------------
* First relase, version 1.0
