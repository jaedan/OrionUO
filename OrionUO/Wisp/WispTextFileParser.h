
#ifndef WISPTEXTFILEPARSER_H
#define WISPTEXTFILEPARSER_H

namespace WISP_FILE
{
class CTextFileParser
{
public:
    string RawLine = "";

private:
    CMappedFile m_File;

    puchar m_Ptr{ 0 };

    puchar m_EOL{ 0 };

    puchar m_End{ 0 };

    char m_Delimiters[50];

    int m_DelimitersSize{ 0 };

    char m_Comentaries[50];

    int m_ComentariesSize{ 0 };

    char m_Quotes[50];

    int m_QuotesSize{ 0 };

    bool m_Trim{ false };

    void GetEOL();

    bool IsDelimiter();

    void SkipToData();

    bool IsComment();

    bool IsQuote();

    bool IsSecondQuote();

    string ObtainData();

    string ObtainQuotedData();

    void StartupInitalize(const char *delimiters, const char *comentaries, const char *quotes);

    void SaveRawLine();

public:
    CTextFileParser(
        const string &path,
        const char *delimiters = "",
        const char *comentaries = "",
        const char *quotes = "");
    CTextFileParser(
        const wstring &path,
        const char *delimiters = "",
        const char *comentaries = "",
        const char *quotes = "");
    ~CTextFileParser();

    void Restart();

    bool IsEOF();

    STRING_LIST ReadTokens(bool trim = true);

    STRING_LIST GetTokens(const char *str, bool trim = true);
};

class CTextFileWritter
{
private:
    FILE *m_File{ NULL };

public:
    CTextFileWritter(const string &path);
    CTextFileWritter(const wstring &path);
    ~CTextFileWritter();

    bool Opened() { return (m_File != NULL); };

    void Close();

    void WriteString(const string &key, const string &value);

    void WriteInt(const string &key, int value);

    void WriteBool(const string &key, bool value);
};

}; // namespace WISP_FILE

#endif
