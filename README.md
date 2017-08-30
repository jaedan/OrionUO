This is a fork of the OrionUO project. The main repository is located at https://github.com/Hotride/OrionUO. Please support that project.

The purpose of this fork is to remove the need for ConfigurationEditor.exe, Orion.dll, and Client.cuo. These are
not open source and make it difficult to configure the Orion client. They are replaced by a simple
text-based configuration file instead.

# Building

You'll need [Visual Studio 2017](https://www.visualstudio.com/downloads/). The free community edition should be fine. Once that
is installed:

- Open OrionUO.sln in the root of the repository.
- Select your configuration (Release, Win32 probably) at the top
- Hit F5 to build. The output will be in the "Release" directory.

# Running

First, create a file in the same directory as OrionUO.exe named 'client.cfg' that contains the following:

~~~
LoginServer=login.yourfavoriteserver.com,2593
CustomPath=C:\Path\to\your\UO client
ClientVersion=7.0.15.1
~~~

Then, double click OrionUO.exe and the game will launch!
