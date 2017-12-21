How to build EAQUAL?

MS Visual C++ 6:
Simply load EAQUAL/build/MSVC/EAQUAL.dsw and rebuild all.

Makefile based
(The current solution is very unsophisticated and only tested
on one single linux system. Feel free to contribute configure
scripts, makefiles, suggestions for compiler switches etc.)
First build the libSndFile library for File-Input. Enter 
EAQUAL/src/libSndFile and type "./configure" and, after that, 
"make". Then enter /EAQUAL/build/unix/ and type "make" to build
the EAQUAL library and its frontend.