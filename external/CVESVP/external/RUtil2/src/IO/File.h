#ifndef RUTIL2_FILE_H
#define RUTIL2_FILE_H

#include <stdio.h>
#include <dirent.h>
#include "../Structure/String.h"
#include "../Core/OO.h"

#if defined(__MINGW32__)
    #define DIR_SPLIT "\\"
#else
    #define DIR_SPLIT "/"
#endif

typedef enum
{
    NOFLAG = 0,
    FILEONLY,
    SHOWHIDDEN
} DirFlags;

typedef enum
{
    READONLY = 0,
    WRITEONLY,
    READWRITE,
    APPEND,
    CREATE
} OpenMode;

RClass(File)
{
    RInherit(RObject);
    int IsOpen;
    OpenMode FileMode;
    FILE* BaseStream;
    int64_t FilePtr;
    int64_t Length;
};

RClass(Directory)
{
    RInherit(RObject);
    
    // Public
    String Path;
    String Wildcard;
    DirFlags Flags;
    
    // Private
    DIR* Dir;
    struct dirent* Curr;
};

int File_Open(File* This, String* Path, OpenMode FileMode);
int File_OpenChars(File* This, char* Path, OpenMode FileMode);
void File_Close(File* This);

int File_Flush(File* This);

void File_SetPosition(File* This, int64_t Position);
int64_t File_GetPosition(File* This);
int64_t File_GetLength(File* This);

void File_Read_String(File* This, String* Dest);
int File_Read_Chars(File* This, char* Dest);
void File_Read_Buffer(File* This, void* Dest, int64_t ByteCount);

void File_Write_String(File* This, String* Sorc);
void File_Write_Chars(File* This, char* Sorc);
void File_Write_Buffer(File* This, char* Sorc, int64_t Length);

void File_ReadWord(File* This, String* Dest);
void File_ReadLine(File* This, String* Dest);
void File_ReadAll(File* This, String* Dest);
void File_WriteLine(File* This, String* Sorc);

int File_IsFile(String* Path);

void DirFromFilePath(String* Dest, String* Sorc);
void BaseFromFilePath(String* Dest, String* Sorc);

int File_OpenDir(Directory* This, String* Path);
int File_CloseDir(Directory* This);
void File_SetDirFlags(Directory* This, DirFlags Flags);
void File_SetDirFilter(Directory* This, String* Wildcard);
int File_ReadDir(Directory* This, String* Dest);

//Template Reads & Writes
#if 0
#include "_File.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/IO/_File.h"
#else
#define _RTAddress "IO/_File.h"
#endif
#define _ClassName
#define _Attr 1

#include "../Core/Include_T1AllTypes.h"

#endif //RUTIL2_FILE_H

