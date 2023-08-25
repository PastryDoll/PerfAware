#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

// (CAIO) Load binary from memory
static u32 LoadMemoryFromFile(const char *FileName, u32 *buffer, u32 bufferSize)
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

// (CAIO) This function takes a buffer and an index and extracts the byte on that index
static u32 GetByte(u32 buffer, u32 byteIndex)
{
    u32 byte = (buffer >> (byteIndex * 8)) & 0xFF;

    return byte;

}

// (CAIO) This function prints a buffer in binary.. It first inverts to the same order as the commands
//  it also takes the number of bytes to print
static void PrintBinary(u32 buffer, u32 numberBytes)
{   
    for (u32 i = 0; i < numberBytes; ++i)
    {
        u32 byte = GetByte(buffer, i);
        for (int j = 7; j>=0; --j)
        {  
            if (byte & (1<<j))
            {
                printf("%u", 1);
            }
            else 
            {
                printf("%u", 0 );
            }
            if (j == 0)
            {
                printf(" ");
            }
        }
    }
}

static void CheckFirstByte(u32 Command)
{
    switch (Command)
    {
    case (0b100010):
    {
        printf("mov");
    }
        break;
    
    default:
        break;
    }
}

int main(int ArgCount, char **Args)
{
    const char *FileName = "listing_0037_single_register_mov.txt";
    char Commands[1][4] = {"mov"};
    u32 Buffer[100] = {};
    u32 Elements = 2;
    u32 BytesRead = LoadMemoryFromFile(FileName, Buffer, Elements);

    printf("Bytes read: %u\n", BytesRead);
    printf("File content:\n");
    u32 NumberOfLines = (u32)ceil((float)(Elements*8)/32);
    for (u32 i = 0; i<NumberOfLines; ++i)
    {
        for (u32 byteIndex = 0; byteIndex < 2; ++byteIndex)
        {   
            u32 byte = GetByte(Buffer[i], byteIndex);

            if (byteIndex == 0)
            {
                u32 OPCODE = ((byte>>2)&0b00111111);
                u32 D = (byte>>1)&0b00000001;
                u32 W = (byte&0b00000001);

                CheckFirstByte(OPCODE);
            }

            if (byteIndex == 1)
            {
                u32 MOD = (byte>>6)&0b00000011;
                u32 REG = (byte>>3)&0b00000111;
                u32 RM = (byte)&0b00000111;
            }

        }
        printf("\n");
        PrintBinary(Buffer[i],2);
    }

    printf("\n");
    return 0;
}

/*
REG W=0 W=1
000 AL  AX
001 CL CX
010 DL DX
011 BL BX
100 AH SP
101 CH BP
110 DH SI
111 BH DI
*/