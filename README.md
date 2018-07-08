This is a fork of OrionUO specifically for Ultima Online Outlands (https://uooutlands.com).

[![Build status](https://ci.appveyor.com/api/projects/status/wa98hrfsr6rag1rl/branch/master?svg=true)](https://ci.appveyor.com/project/jaedan/classicuo/branch/master)

# Introduction

This is an open source implementation of the Ultima Online Classic Client. This client is intended to emulate client version 7.0.15.1.

The client is currently reasonably functional and is suitable for most activites in game. Bug reports and fixes are always welcome.

The code itself has been cobbled together from the following projects:

* [OrionUO](https://github.com/hotride/orionuo) - The bulk of the "base" client has been pulled from this project, which is written in C++.
* [Razor](https://github.com/msturgill/razor) - The razor assistant has been integrated into the client directly, which introduces a C++/CLI DLL and a C# application.
* [UltimaXNA](https://github.com/ZaneDubya/UltimaXNA) - This is primarily used as a reference, for now.
* [ServUO](https://github.com/servuo/servuo) - This is also primarily used as a reference, but we expect more code to be shared here over time.

# Building

Currently, only Windows is supported. Cross-platform support is a long term goal.

You'll need [Visual Studio 2017](https://www.visualstudio.com/downloads/). The free community edition should be fine. Once that
is installed:

- Open OrionUO.sln in the root of the repository.
- Select your configuration (Release, Win32 probably) at the top
- Hit F5 to build. The output will be in the "Release/Win32" directory.

# Running

First, create a file in the same directory as OrionUO.exe named 'client.cfg' that contains the following:

~~~
LoginServer=login.uooutlands.com,2593
CustomPath=C:\Path\to\your\UO client
ClientVersion=7.0.15.1
Plugin=Crypt.dll
~~~

Then, double click OrionUO.exe and the game will launch!

# Legal

This work is released under the MIT license. This project does not distribute any copyrighted game assets. In order to run this client you'll need to legally obtain a copy of version 7.0.15.1 of the Ultima Online Classic Client.

Ultima Online(R) © 2018 Electronic Arts Inc. All Rights Reserved.