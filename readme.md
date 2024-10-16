# Nero-2.0
The Cars Dashboard Written in C++ With Qt Framework

### Onboarding

Download Qt Installer For Open Source (https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)

Open Qt Installer. 

Create a Qt account if you do not already have one. 

Accept the open source licensing agreement

Select the Default options for installing components
- Select Qt version 6.5.3

Select Next Until You start downloading Qt (Should be around 30 GB)

### Install protobuf for your machine

#### Mac Os

`brew install protobuf@3`

`brew link --overwrite protobuf@3`

#### Linux

`sudo apt update`

`sudo apt install protobuf-compiler`

`protoc`

### Compiling QtMqtt for your Qt version
1 load QtCreator -> Load Project c:\Qt\6.5.3\Src\QtMqtt\CMakeList.txt

2 Build Release and Debug

3 Exit QtCreator

4 Go to C:\Qt\6.5.3\Src\qtmqtt\build\build-qtmqtt-Desktop_Qt_6_5_3_MinGW_64_bit-Release

5 cmake --install . (see the point at the end)

Done

#### Opening the Project
Clone This repository to a directory of your choosing

Select the cmakelists.txt file inside the NERO Development to open in qt creator

It should generate the project

Select the kit for your respective desktop and press build and run

### Installing Prettier

We use clang-format for formatting our files

Ensure that the Beautifier plugin is installed in your Qt Creator. You can check and install plugins by going to “Help” > “About Plugins” and then enabling the “Beautifier” plugin.

Note: Enable the Beautifier plugin to use it. Since Qt Creator 10.0.0, the ClangFormat plugin is enabled by default. Select Preferences > C++ > Formatting mode > Disable to turn off ClangFormat if you enable Beautifier because combining them can lead to unexpected results.


#### Mac

brew install clang-format

#### Linux

sudo apt-get install clang-format

#### Windows

Visit the LLVM Download page: https://releases.llvm.org/download.html
Choose the appropriate version for Windows. Look for the "Pre-Built Binaries" section.
Download the Windows installer (MSVC) for the desired LLVM version.

Follow the installation instructions. You can usually go with the default settings, but make sure to select "clang-format" as one of the components to install.

Right-click on "This PC" (or "Computer") on your desktop or in the File Explorer.
Choose "Properties" from the context menu.
Click on "Advanced system settings" on the left sidebar.
In the System Properties window, click the "Environment Variables" button.
Under "System variables," find and select the "Path" variable, then click the "Edit" button.
Click the "New" button and add the path to the bin directory of your LLVM installation (e.g., C:\Program Files\LLVM\bin).
Click OK to close all the windows.

Type clang-format --version and press Enter. You should see the version information for clang-format if the installation was successful.

### Linking Clang-Format to Qt Creator

With Qt Creator open, select preferences from under the Qt Creator tab in the top menu bar

Select the beautifer tab (Should have a diamond icon)

Under the general tab check the box for automatic formatting on save

Select Clang-format as the tool

<img width="1098" alt="Screen Shot 2023-08-03 at 8 33 27 AM" src="https://github.com/Northeastern-Electric-Racing/Nero-2.0/assets/113635669/dc179628-2a80-47cd-8dcd-47f5a6815189">

Now under the Clang Format tab Set the path to where you installed your clang-format command
You can figure out the file directory by running ```which clang-format```

Now you can edit a file and save it to make sure it works
