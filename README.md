Another simple sfb reader for PS3's PS3_DISC.SFB file  
Also you can look pink1stools' s editor in [here](https://github.com/pink1stools/PS3_DISC.SFB-Editor)


# Features

## Linux
- It can only read, write and create features will implemented in future.

## Windows
- It can read, write, and create(write/create usable in gui).
- It has an ugly gui. Drag and drop supported.

# Compiling

```
gcc sfb.c -o sfb.o
```

Windows user can use cl.exe for compiling. Don't forget to set environment variables before execute!

```
cl sfb.c /Fe:sfb.exe User32.lib Comdlg32.lib Gdi32.lib Shell32.lib
```

for those who want to use mingw

```
gcc sfb.c -luser32 -lshell32 -lgdi32  -lcomdlg32 -o sfb.exe
```


# Usage
The usage is same in Linux and Windows but in Windows if you want to use gui you shouldn't give any parameters(double clicking).

```
./sfb.o <YOUR_SFB_FILE>
```

An expected output should look like this

```
./sfb.o PS3_DISC.SFB
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
