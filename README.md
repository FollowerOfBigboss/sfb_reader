Another simple sfb reader for PS3's PS3_DISC.SFB file

# Compiling

```
gcc sfb.c -o sfb.o
```

Windows user can use cl.exe for compiling. Don't forget to set environment variables before execute!

```
cl.exe sfb.c /Fe:sfb.exe
```

# Usage

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