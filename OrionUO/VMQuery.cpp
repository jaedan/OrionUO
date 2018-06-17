

#include "stdafx.h"

typedef struct
{
    SIZE_T RgnSize;
    DWORD dwRgnStorage;
    DWORD dwRgnBlocks;
    DWORD dwRgnGuardBlks;
    BOOL fRgnIsAStack;
} VMQUERY_HELP;

static DWORD gs_dwAllocGran = 0;

static BOOL VMQueryHelp(HANDLE hProcess, LPCVOID pvAddress, VMQUERY_HELP *pVMQHelp)
{
    DWORD dwProtectBlock[4] = { 0 };

    ZeroMemory(pVMQHelp, sizeof(*pVMQHelp));

    MEMORY_BASIC_INFORMATION mbi;
    BOOL fOk = (VirtualQueryEx(hProcess, pvAddress, &mbi, sizeof(mbi)) == sizeof(mbi));

    if (!fOk)
        return (fOk);

    PVOID pvRgnBaseAddress = mbi.AllocationBase;

    PVOID pvAddressBlk = pvRgnBaseAddress;

    pVMQHelp->dwRgnStorage = mbi.Type;

    for (;;)
    {
        fOk = (VirtualQueryEx(hProcess, pvAddressBlk, &mbi, sizeof(mbi)) == sizeof(mbi));
        if (!fOk)
            break;

        if (mbi.AllocationBase != pvRgnBaseAddress)
            break;

        if (pVMQHelp->dwRgnBlocks < 4)
        {
            dwProtectBlock[pVMQHelp->dwRgnBlocks] = (mbi.State == MEM_RESERVE) ? 0 : mbi.Protect;
        }
        else
        {
            MoveMemory(
                &dwProtectBlock[0], &dwProtectBlock[1], sizeof(dwProtectBlock) - sizeof(DWORD));

            dwProtectBlock[3] = (mbi.State == MEM_RESERVE) ? 0 : mbi.Protect;
        }

        pVMQHelp->dwRgnBlocks++;
        pVMQHelp->RgnSize += mbi.RegionSize;

        if ((mbi.Protect & PAGE_GUARD) == PAGE_GUARD)
            pVMQHelp->dwRgnGuardBlks++;

        if (pVMQHelp->dwRgnStorage == MEM_PRIVATE)
            pVMQHelp->dwRgnStorage = mbi.Type;

        pvAddressBlk = (PVOID)((PBYTE)pvAddressBlk + mbi.RegionSize);
    }

    pVMQHelp->fRgnIsAStack = (pVMQHelp->dwRgnGuardBlks > 0) ||
                             ((pVMQHelp->dwRgnBlocks >= 4) && (dwProtectBlock[0] == 0) &&
                              (dwProtectBlock[1] == PAGE_NOACCESS) &&
                              (dwProtectBlock[2] == PAGE_READWRITE) && (dwProtectBlock[3] == 0));

    return (TRUE);
}

BOOL VMQuery(HANDLE hProcess, LPCVOID pvAddress, PVMQUERY pVMQ)
{
    if (gs_dwAllocGran == 0)
    {
        SYSTEM_INFO sinf;
        GetSystemInfo(&sinf);
        gs_dwAllocGran = sinf.dwAllocationGranularity;
    }

    ZeroMemory(pVMQ, sizeof(*pVMQ));

    MEMORY_BASIC_INFORMATION mbi;
    BOOL fOk = (VirtualQueryEx(hProcess, pvAddress, &mbi, sizeof(mbi)) == sizeof(mbi));

    if (!fOk)
        return (fOk);

    switch (mbi.State)
    {
        case MEM_FREE:
            pVMQ->pvBlkBaseAddress = NULL;
            pVMQ->BlkSize = 0;
            pVMQ->dwBlkProtection = 0;
            pVMQ->dwBlkStorage = MEM_FREE;
            break;

        case MEM_RESERVE:
            pVMQ->pvBlkBaseAddress = mbi.BaseAddress;
            pVMQ->BlkSize = mbi.RegionSize;

            pVMQ->dwBlkProtection = mbi.AllocationProtect;
            pVMQ->dwBlkStorage = MEM_RESERVE;
            break;

        case MEM_COMMIT:
            pVMQ->pvBlkBaseAddress = mbi.BaseAddress;
            pVMQ->BlkSize = mbi.RegionSize;
            pVMQ->dwBlkProtection = mbi.Protect;
            pVMQ->dwBlkStorage = mbi.Type;
            break;

        default:
            DebugBreak();
            break;
    }

    VMQUERY_HELP VMQHelp;
    switch (mbi.State)
    {
        case MEM_FREE:
            pVMQ->pvRgnBaseAddress = mbi.BaseAddress;
            pVMQ->dwRgnProtection = mbi.AllocationProtect;
            pVMQ->RgnSize = mbi.RegionSize;
            pVMQ->dwRgnStorage = MEM_FREE;
            pVMQ->dwRgnBlocks = 0;
            pVMQ->dwRgnGuardBlks = 0;
            pVMQ->fRgnIsAStack = FALSE;
            break;

        case MEM_RESERVE:
            pVMQ->pvRgnBaseAddress = mbi.AllocationBase;
            pVMQ->dwRgnProtection = mbi.AllocationProtect;

            VMQueryHelp(hProcess, pvAddress, &VMQHelp);

            pVMQ->RgnSize = VMQHelp.RgnSize;
            pVMQ->dwRgnStorage = VMQHelp.dwRgnStorage;
            pVMQ->dwRgnBlocks = VMQHelp.dwRgnBlocks;
            pVMQ->dwRgnGuardBlks = VMQHelp.dwRgnGuardBlks;
            pVMQ->fRgnIsAStack = VMQHelp.fRgnIsAStack;
            break;

        case MEM_COMMIT:
            pVMQ->pvRgnBaseAddress = mbi.AllocationBase;
            pVMQ->dwRgnProtection = mbi.AllocationProtect;

            VMQueryHelp(hProcess, pvAddress, &VMQHelp);

            pVMQ->RgnSize = VMQHelp.RgnSize;
            pVMQ->dwRgnStorage = VMQHelp.dwRgnStorage;
            pVMQ->dwRgnBlocks = VMQHelp.dwRgnBlocks;
            pVMQ->dwRgnGuardBlks = VMQHelp.dwRgnGuardBlks;
            pVMQ->fRgnIsAStack = VMQHelp.fRgnIsAStack;
            break;

        default:
            DebugBreak();
            break;
    }

    return (fOk);
}
