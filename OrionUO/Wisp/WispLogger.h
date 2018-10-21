
#ifndef WISPLOGGER_H
#define WISPLOGGER_H

namespace WISP_LOGGER
{
#ifdef _DEBUG
#define CWISPLOGGER 1
#else
#define CWISPLOGGER 0
#endif

#if CWISPLOGGER != 0
#define INITLOGGER(path) WISP_LOGGER::g_WispLogger.Init(path);
#define LOG WISP_LOGGER::g_WispLogger.Print
#if CWISPLOGGER == 2
#define LOG_DUMP(...)
#else
#define LOG_DUMP WISP_LOGGER::g_WispLogger.Dump
#endif
#else
#define INITLOGGER(path)
#define LOG(...)
#define LOG_DUMP(...)
#endif

#define INITCRASHLOGGER(path) WISP_LOGGER::g_WispCrashLogger.Init(path);
#define CRASHLOG WISP_LOGGER::g_WispCrashLogger.Print
#define CRASHLOG_DUMP WISP_LOGGER::g_WispCrashLogger.Dump

class CLogger
{
public:
    string FileName = "";

protected:
    FILE *m_File{ NULL };

public:
    CLogger();
    ~CLogger();

    void Close();

    bool Ready() const { return m_File != NULL; }

    void Init(const string &filePath);
    void Init(const wstring &filePath);

    void Print(const char *format, ...);
    void VPrint(const char *format, va_list ap);
    void Print(const wchar_t *format, ...);
    void VPrint(const wchar_t *format, va_list ap);
    void Dump(uchar *buf, int size);
};

extern CLogger g_WispLogger;
extern CLogger g_WispCrashLogger;
} // namespace WISP_LOGGER

#endif
