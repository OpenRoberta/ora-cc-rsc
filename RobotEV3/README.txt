The files, that are needed for the EV3-lejos crosscompiler are jars,
that are either

* retrieved from maven repositories (http://maven.iais.fraunhofer.de/artifactory/openroberta-release)
* generated during the build

When "./ora.sh --export" is called, this directory is filled with these resources.
It makes sense, they are NOT part of the "ora-cc-rsc" repository.