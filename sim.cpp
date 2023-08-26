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

// (CAIO) This function prints a buffer in binary.. It first inverts to the same order as the comands
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

typedef struct
 {
    u32 OPCODE;
    u32 D;
    u32 W;

} FirstByte;

typedef struct
 {
    u32 MOD;
    u32 REG;
    u32 RM;

} SecondByte;

static FirstByte CheckFirstByte(u32 byte)
{   
    FirstByte Commands;
    Commands.OPCODE = ((byte>>2)&0b00111111); 
    Commands.D = (byte>>1)&0b00000001;
    Commands.W = (byte&0b00000001);

    return Commands;
}

static SecondByte CheckSecondByte(u32 byte)
{   
    SecondByte Commands;
    Commands.MOD = (byte>>6)&0b00000011;
    Commands.REG = (byte>>3)&0b00000111;
    Commands.RM = (byte)&0b00000111;

    return Commands;
}

int main(int ArgCount, char **Args)
{
    const char *FileName = "listing_0037_single_register_mov.txt";
    u32 Buffer[100] = {};
    u32 Elements = 2;
    u32 BytesRead = LoadMemoryFromFile(FileName, Buffer, Elements);

    printf("Bytes read: %u\n", BytesRead); 
    printf("File content:\n");
    u32 NumberOfLines = (u32)ceil((float)(Elements*8)/32);
    for (u32 i = 0; i<NumberOfLines; ++i)
    {
        u32 byte1 = GetByte(Buffer[i], 0);
        u32 byte2 = GetByte(Buffer[i], 1);
        u32 byte3 = GetByte(Buffer[i], 2);
        u32 byte4 = GetByte(Buffer[i], 3);

        FirstByte FirstByteCommands = CheckFirstByte(byte1);
        SecondByte SecondByteCommands = CheckSecondByte(byte2);

        if (FirstByteCommands.OPCODE == 0b00100010)
        {
            printf("mov ");

            if (FirstByteCommands.D == 0) // SOURCE = REG
                switch (SecondByteCommands.RM)
                {
                case (0b000):
                {
                    printf("ax, ");
                }
                break;
                case (0b001):
                {
                    printf("cx, ");
                }
                break;
                case (0b010):
                {
                    printf("dx, ");
                }
                break;
                case (0b011):
                {
                    printf("bx, ");
                }
                break;
                }
                switch (SecondByteCommands.REG)
                {
                case (0b000):
                {
                    printf("ax");
                }
                break;
                case (0b001):
                {
                    printf("cx");
                }
                break;
                case (0b010):
                {
                    printf("dx");
                }
                break;
                case (0b011):
                {
                    printf("bx");
                }
                break;
                }
        }


        printf("\n");
        PrintBinary(Buffer[i],2);
    }

    printf("\n");
    return 0;
}

// Function to check commands of first byte

//COMAND DW MOD REG RM
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
