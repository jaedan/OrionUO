

#pragma once

#include <windows.h>

#if _MSC_VER < 1300
typedef unsigned __int64 DWORD64, *PDWORD64;
#if defined(_WIN64)
typedef unsigned __int64 SIZE_T, *PSIZE_T;
#else
typedef unsigned long SIZE_T, *PSIZE_T;
#endif
#endif

class StackWalkerInternal;
class StackWalker
{
public:
    typedef enum StackWalkOptions
    {

        RetrieveNone = 0,

        RetrieveSymbol = 1,

        RetrieveLine = 2,

        RetrieveModuleInfo = 4,

        RetrieveFileVersion = 8,

        RetrieveVerbose = 0xF,

        SymBuildPath = 0x10,

        SymUseSymSrv = 0x20,

        SymAll = 0x30,

        OptionsAll = 0x3F
    } StackWalkOptions;

    StackWalker(
        int options = OptionsAll,
        LPCSTR szSymPath = NULL,
        DWORD dwProcessId = GetCurrentProcessId(),
        HANDLE hProcess = GetCurrentProcess());
    StackWalker(DWORD dwProcessId, HANDLE hProcess);
    virtual ~StackWalker();

    typedef BOOL(__stdcall *PReadProcessMemoryRoutine)(
        HANDLE hProcess,
        DWORD64 qwBaseAddress,
        PVOID lpBuffer,
        DWORD nSize,
        LPDWORD lpNumberOfBytesRead,
        LPVOID pUserData);

    BOOL LoadModules();

    BOOL ShowCallstack(
        HANDLE hThread = GetCurrentThread(),
        const CONTEXT *context = NULL,
        PReadProcessMemoryRoutine readMemoryFunction = NULL,
        LPVOID pUserData = NULL);

#if _MSC_VER >= 1300

protected:
#endif
    enum
    {
        STACKWALK_MAX_NAMELEN = 1024
    };

protected:
    typedef struct CallstackEntry
    {
        DWORD64 offset;
        CHAR name[STACKWALK_MAX_NAMELEN];
        CHAR undName[STACKWALK_MAX_NAMELEN];
        CHAR undFullName[STACKWALK_MAX_NAMELEN];
        DWORD64 offsetFromSmybol;
        DWORD offsetFromLine;
        DWORD lineNumber;
        CHAR lineFileName[STACKWALK_MAX_NAMELEN];
        DWORD symType;
        LPCSTR symTypeString;
        CHAR moduleName[STACKWALK_MAX_NAMELEN];
        DWORD64 baseOfImage;
        CHAR loadedImageName[STACKWALK_MAX_NAMELEN];
    } CallstackEntry;

    enum CallstackEntryType
    {
        firstEntry,
        nextEntry,
        lastEntry
    };

    virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
    virtual void OnLoadModule(
        LPCSTR img,
        LPCSTR mod,
        DWORD64 baseAddr,
        DWORD size,
        DWORD result,
        LPCSTR symType,
        LPCSTR pdbName,
        ULONGLONG fileVersion);
    virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
    virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
    virtual void OnOutput(LPCSTR szText);

    StackWalkerInternal *m_sw;
    HANDLE m_hProcess;
    DWORD m_dwProcessId;
    BOOL m_modulesLoaded;
    LPSTR m_szSymPath;

    int m_options;

    static BOOL __stdcall myReadProcMem(
        HANDLE hProcess,
        DWORD64 qwBaseAddress,
        PVOID lpBuffer,
        DWORD nSize,
        LPDWORD lpNumberOfBytesRead);

    friend StackWalkerInternal;
};

#if defined(_M_IX86)
#ifdef CURRENT_THREAD_VIA_EXCEPTION

#define GET_CURRENT_CONTEXT(c, contextFlags)                                                       \
    do                                                                                             \
    {                                                                                              \
        memset(&c, 0, sizeof(CONTEXT));                                                            \
        EXCEPTION_POINTERS *pExp = NULL;                                                           \
        __try                                                                                      \
        {                                                                                          \
            throw 0;                                                                               \
        }
__except (
    ((pExp = GetExceptionInformation()) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER))
{
}
if (pExp != NULL)
    memcpy(&c, pExp->ContextRecord, sizeof(CONTEXT));
c.ContextFlags = contextFlags;
}
while (0)
    ;
#else

#define GET_CURRENT_CONTEXT(c, contextFlags)                                                       \
    do                                                                                             \
    {                                                                                              \
        memset(&c, 0, sizeof(CONTEXT));                                                            \
        c.ContextFlags = contextFlags;                                                             \
        __asm call x __asm x : pop eax __asm mov c.Eip, eax __asm mov c.Ebp, ebp __asm mov c.Esp,  \
            esp                                                                                    \
    } while (0);
#endif

#else

#define GET_CURRENT_CONTEXT(c, contextFlags)                                                       \
    do                                                                                             \
    {                                                                                              \
        memset(&c, 0, sizeof(CONTEXT));                                                            \
        c.ContextFlags = contextFlags;                                                             \
        RtlCaptureContext(&c);                                                                     \
    } while (0);
#endif
