#!/bin/bash
TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX= make $1 $2 $3 $4
#TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX=posix make $1 $2 $3 $4
#TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX=win32 make $1 $2 $3 $4
#TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX= make $1 $2 $3 $4
#TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX=posix make $1 $2 $3 $4
#TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX=win32 make $1 $2 $3 $4
