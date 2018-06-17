

typedef struct
{
    PVOID pvRgnBaseAddress;
    DWORD dwRgnProtection;
    SIZE_T RgnSize;
    DWORD dwRgnStorage;
    DWORD dwRgnBlocks;
    DWORD dwRgnGuardBlks;
    BOOL fRgnIsAStack;

    PVOID pvBlkBaseAddress;
    DWORD dwBlkProtection;
    SIZE_T BlkSize;
    DWORD dwBlkStorage;
} VMQUERY, *PVMQUERY;

BOOL VMQuery(HANDLE hProcess, LPCVOID pvAddress, PVMQUERY pVMQ);
