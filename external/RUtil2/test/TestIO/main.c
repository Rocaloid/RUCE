#include <stdio.h>
#include "Structure/String.h"
#include "Misc/Convert.h"
#include "Misc/Sort.h"
#include "IO/File.h"
#include "IO/WaveFile.h"

int main(void)
{
    /* Generic File IO
    String Path, tmp;
    RNew(String, & Path, & tmp);
    String_SetChars(& Path, "/tmp/a.txt");
    
    File TestFile, WriteFile;
    RNew(File, & TestFile, & WriteFile);
    int Status = File_Open(& TestFile, & Path, READONLY);
    printf("%d\n", Status);
    
    String_SetChars(& Path, "/tmp/b.txt");
    Status = File_Open(& WriteFile, & Path, WRITEONLY);
    printf("%d\n", Status);
    
    File_ReadLine(& TestFile, & tmp);
    printf("%s\n", String_GetChars(& tmp));
    
    int i;
    double a;
    for(i = 0; i < 100; i ++)
    {
        a = File_Read_Double(& TestFile);
        printf("%lf\n", a);
        File_Write_Double(& WriteFile, a / 2);
    }
    
    File_Write_Buffer(& WriteFile, Path.Data, 3);
    
    File_Close(& TestFile);
    File_Flush(& WriteFile);
    RDelete(& Path, & TestFile, & tmp, & WriteFile);
    */
    String Path;
    RNew(String, & Path);
    WaveFile Wave, OutputWave;
    RNew(WaveFile, & Wave, & OutputWave);
    
    String_SetChars(& Path, "/tmp/test.wav");
    WaveFile_Open(& Wave, & Path);
    WaveFile_PrintInfo(& Wave);
    
    float* temp = (float*)malloc(Wave.Header.DataNum * 4);
    WaveFile_FetchAllFloat(& Wave, temp);
    Sort_Dec_Float(temp, temp, Wave.Header.DataNum);
    String_SetChars(& Path, "/tmp/x.wav");
    
    OutputWave.Header = Wave.Header;
    WaveFile_Save(& OutputWave, & Path);
    WaveFile_WriteAllFloat(& OutputWave, temp, Wave.Header.DataNum);
    WaveFile_FinishWrite(& OutputWave);
    free(temp);
    
    WaveFile_Close(& Wave);
    WaveFile_Close(& OutputWave);
    RDelete(& Path, & Wave, & OutputWave);
    return 0;
}

