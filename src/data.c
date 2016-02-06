#include "data.h"

int *dData;
int dSize;
int dPos;

void DInit(int size)
{
    dSize = size;
    dPos = 0;

    dData = (int*) calloc(size, sizeof(int));
}

void DAddValue(int value)
{
    if (dPos > dSize) {
        dPos = 0;
    }
    else {
        dPos++;
    }

    // fprintf(stdout, "INFO: dPos = %d\n", dPos);
    dData[dPos] = value;
    dData[dPos +1] = -1;
    fprintf(stdout, "INFO: dData[%d] = %d\n", dPos, dData[dPos]);
}

int DGetItem(int i)
{
    return dData[i];
}

// int DNextItem(int i)
// {
//     if (i > dSize) {
//         return -1;
//     }
//     else {
//         int out = dData[i +1];
//         fprintf(stdout, "INFO: out = %d w/ i = %d\n", out, i);
// 
//         return out;
//     }
// }
