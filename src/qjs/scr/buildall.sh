#!/bin/bash
TOOLCHAINDIR=/usr/local/djgpp/bin/ PREFIX=i586-pc-msdosdjgpp POSTFIX= make
TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX= make
TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX=posix make
TOOLCHAINDIR=/usr/bin/ PREFIX=i686-w64-mingw32 POSTFIX=win32 make
TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-linux-gnu POSTFIX= make
TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-linux-gnu POSTFIX=7 make
TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX= make
TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX=posix make
TOOLCHAINDIR=/usr/bin/ PREFIX=x86_64-w64-mingw32 POSTFIX=win32 make
