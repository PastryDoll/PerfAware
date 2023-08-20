#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

static u32 LoadMemoryFromFile(char *FileName, u8 *buffer, u32 bufferSize)
{
    u32 Result = 0;
    
    FILE *File = fopen(FileName, "rb");
    if(File)
    {
        Result = fread(buffer, 1, bufferSize, File);
        fclose(File);
    }
    else
    {
        fprintf(stderr, "ERROR: Unable to open %s.\n", FileName);
    }
    
    return Result;
};

int main(int ArgCount, char **Args)
{
    char *FileName = "listing_0037_single_register_mov.txt";
    u8 buffer[2];
    u32 BytesRead = LoadMemoryFromFile(FileName, buffer, 2);

    printf("Bytes read: %u\n", BytesRead);
    printf("File content:\n");

    for (u32 i = 0; i < BytesRead; ++i)
    {   
        for (int j=7; j>=0; --j)
        {  
            if (buffer[i] & (u8)(1<<j))
            {
                printf("%u", 1);
            }
            else {
                printf("%u", 0 );
            }
            if (j == 0){
                printf(" ");
            }
        }
    }

    printf("\n");

    return 0;
}