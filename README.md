Another simple sfb reader for PS3's PS3_DISC.SFB file  
Also you can look pink1stools' s editor in [here](https://github.com/pink1stools/PS3_DISC.SFB-Editor)


# Features
It has gui editor which supports reading, writing and creating. Command line version supports reading and creating but it doesn't support writing.

# Compiling
This program has 2 versions, command line and gui. Gui version also brings command line version but command line version doesn't bring gui version so it is less size and it doesn't require any dependency.
If you don't want to use gui version you don't need to specify any extra parameters. Gui version uses Qt5. 

Steps to build command line version

```
mkdir build
cd build
cmake ..
```

after project file generation in windows open visual studio then build. In linux you need to run ```make```

Steps to build gui version

```
mkdir build
cd build
cmake .. -DUSEGUI=True
```

For linux you need to run ```make``` . In visual studio you need to install Qt5 extension.
After build in windows you need to run winqtdeploy because Qt5 linked dynamically so we need libraries.

```
winqtdeploy <Your build path>
```

if cmake couldn't find your Qt5 location provide your Qt5 directory like this

```
cmake .. -DUSEGUI=True -DCMAKE_PREFIX_PATH=D:\Qt\5.9.9\msvc2017_64 
```


# Usage
The usage is same in Linux and Windows but if you want to use gui version you shouldn't give any parameters.

```
./sfb.o [options] <file>
```
### Options for command line
* read
Reads file at given file parameter
* create  
Creates a file according to file parameter, if there is no file parameter it creates automatically file by default name

An expected output should look like this

```
$./sfb.o read PS3_DISC.SFB
Magic: .SFB
Version: 0x00010000
HYBRID FLAG: HYBRID_FLAG
Disc Content Data Offset: 0x00000200
Disc Content Data Lenght: 0x00000020
Disc Title Name: TITLE_ID
Disc Title Data Offset: 0x00000220
Disc Title Data Lenght: 0x00000010
Disc Content: gu
Disc Title: BLES-00932
```
