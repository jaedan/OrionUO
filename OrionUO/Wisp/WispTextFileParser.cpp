#include "stdafx.h"

namespace WISP_FILE
{
CTextFileParser::CTextFileParser(
    const string &path, const char *delimiters, const char *comentaries, const char *quotes)
{
    WISPFUN_DEBUG("c11_f1");
    m_File.Load(path);

    StartupInitalize(delimiters, comentaries, quotes);
}

CTextFileParser::CTextFileParser(
    const wstring &path, const char *delimiters, const char *comentaries, const char *quotes)
{
    WISPFUN_DEBUG("c11_f2");
    m_File.Load(path);

    StartupInitalize(delimiters, comentaries, quotes);
}

CTextFileParser::~CTextFileParser()
{
    WISPFUN_DEBUG("c11_f3");

    m_File.Unload();
}

void CTextFileParser::StartupInitalize(
    const char *delimiters, const char *comentaries, const char *quotes)
{
    WISPFUN_DEBUG("c11_f4");

    memset(&m_Delimiters[0], 0, sizeof(m_Delimiters));
    m_DelimitersSize = (int)strlen(delimiters);

    if (m_DelimitersSize)
        memcpy(&m_Delimiters[0], &delimiters[0], m_DelimitersSize);

    memset(&m_Comentaries[0], 0, sizeof(m_Comentaries));
    m_ComentariesSize = (int)strlen(comentaries);

    if (m_ComentariesSize)
        memcpy(&m_Comentaries[0], &comentaries[0], m_ComentariesSize);

    memset(&m_Quotes[0], 0, sizeof(m_Quotes));
    m_QuotesSize = (int)strlen(quotes);

    if (m_QuotesSize)
        memcpy(&m_Quotes[0], &quotes[0], m_QuotesSize);

    m_End = m_File.End;
}

void CTextFileParser::Restart()
{
    WISPFUN_DEBUG("c11_f5");
    m_File.ResetPtr();
}

bool CTextFileParser::IsEOF()
{
    WISPFUN_DEBUG("c11_f6");
    return (m_File.Ptr >= m_End);
}

void CTextFileParser::GetEOL()
{
    WISPFUN_DEBUG("c11_f7");

    m_EOL = m_File.Ptr;

    if (!IsEOF())
    {
        while (m_EOL < m_End && *m_EOL)
        {
            if (*m_EOL == '\n')
                break;

            m_EOL++;
        }
    }
}

bool CTextFileParser::IsDelimiter()
{
    WISPFUN_DEBUG("c11_f8");
    bool result = false;

    for (int i = 0; i < m_DelimitersSize && !result; i++)
        result = (*m_Ptr == m_Delimiters[i]);

    return result;
}

void CTextFileParser::SkipToData()
{
    WISPFUN_DEBUG("c11_f9");

    while (m_Ptr < m_EOL && *m_Ptr && IsDelimiter())
        m_Ptr++;
}

bool CTextFileParser::IsComment()
{
    WISPFUN_DEBUG("c11_f10");
    bool result = (*m_Ptr == '\n');

    for (int i = 0; i < m_ComentariesSize && !result; i++)
    {
        result = (*m_Ptr == m_Comentaries[i]);

        if (result && i + 1 < m_ComentariesSize && m_Comentaries[i] == m_Comentaries[i + 1] &&
            m_Ptr + 1 < m_EOL)
        {
            result = (m_Ptr[0] == m_Ptr[1]);
            i++;
        }
    }

    return result;
}

bool CTextFileParser::IsQuote()
{
    bool result = (*m_Ptr == '\n');

    for (int i = 0; i < m_QuotesSize; i += 2)
    {
        if (*m_Ptr == m_Quotes[i] || *m_Ptr == m_Quotes[i + 1])
        {
            result = true;
            break;
        }
    }

    return result;
}

bool CTextFileParser::IsSecondQuote()
{
    bool result = (*m_Ptr == '\n');

    for (int i = 0; i < m_QuotesSize; i += 2)
    {
        if (*m_Ptr == m_Quotes[i + 1])
        {
            result = true;
            break;
        }
    }

    return result;
}

string CTextFileParser::ObtainData()
{
    WISPFUN_DEBUG("c11_f11");
    string result = "";

    while (m_Ptr < m_End && *m_Ptr && *m_Ptr != '\n')
    {
        if (IsDelimiter())
            break;

        else if (IsComment())
        {
            m_Ptr = m_EOL;
            break;
        }

        if (*m_Ptr != '\r' && (!m_Trim || (*m_Ptr != ' ' && *m_Ptr != '\t')))
            result.push_back(*m_Ptr);

        m_Ptr++;
    }

    return result;
}

string CTextFileParser::ObtainQuotedData()
{
    WISPFUN_DEBUG("c11_f12");
    bool exit = false;
    string result = "";

    for (int i = 0; i < m_QuotesSize; i += 2)
    {
        if (*m_Ptr == m_Quotes[i])
        {
            char endQuote = m_Quotes[i + 1];
            exit = true;

            m_Ptr++;
            puchar ptr = m_Ptr;

            while (ptr < m_EOL && *ptr && *ptr != '\n' && *ptr != endQuote)
                ptr++;

            size_t size = ptr - m_Ptr;

            if (size > 0)
            {
                result.resize(size + 1);

                memcpy(&result[0], &m_Ptr[0], size);
                result[size] = 0;

                for (int j = (int)size - 1; j >= 0 && result[j] == '\r'; j--)
                    result[j] = 0;

                m_Ptr = ptr;

                if (m_Ptr < m_EOL && *m_Ptr == endQuote)
                    m_Ptr++;
            }

            break;
        }
    }

    if (!exit)
        result = ObtainData();

    return result;
}

void CTextFileParser::SaveRawLine()
{
    WISPFUN_DEBUG("c11_f13");
    size_t size = m_EOL - m_Ptr;

    if (size > 0)
    {
        RawLine.resize(size, 0);
        memcpy(&RawLine[0], &m_Ptr[0], size);

        while (RawLine.length() && (RawLine[size - 1] == '\r' || RawLine[size - 1] == '\n'))
            RawLine.resize(RawLine.length() - 1);
    }
    else
        RawLine = "";
}

STRING_LIST CTextFileParser::ReadTokens(bool trim)
{
    WISPFUN_DEBUG("c11_f14");
    m_Trim = trim;
    STRING_LIST result;

    if (!IsEOF())
    {
        m_Ptr = m_File.Ptr;
        GetEOL();

        SaveRawLine();

        while (m_Ptr < m_EOL)
        {
            SkipToData();

            if (IsComment())
                break;

            string buf = ObtainQuotedData();

            if (buf.length())
                result.push_back(buf);
            else if (IsSecondQuote())
                m_Ptr++;
        }

        m_File.Ptr = m_EOL + 1;
    }

    return result;
}

STRING_LIST CTextFileParser::GetTokens(const char *str, bool trim)
{
    WISPFUN_DEBUG("c11_f15");
    m_Trim = trim;
    STRING_LIST result;

    puchar oldEnd = m_End;

    m_Ptr = (puchar)str;
    m_End = (puchar)str + strlen(str);
    m_EOL = m_End;

    SaveRawLine();

    while (m_Ptr < m_EOL)
    {
        SkipToData();

        if (IsComment())
            break;

        string buf = ObtainQuotedData();

        if (buf.length())
            result.push_back(buf);
    }

    m_End = oldEnd;

    return result;
}

CTextFileWritter::CTextFileWritter(const string &path)
{
    fopen_s(&m_File, path.c_str(), "w");
}

CTextFileWritter::CTextFileWritter(const wstring &path)
{
    _wfopen_s(&m_File, path.c_str(), L"w");
}

CTextFileWritter::~CTextFileWritter()
{
    Close();
}

void CTextFileWritter::Close()
{
    if (m_File != NULL)
    {
        fclose(m_File);
        m_File = NULL;
    }
}

void CTextFileWritter::WriteString(const string &key, const string &value)
{
    if (m_File != NULL)
        fputs(string(key + "=" + value + "\n").c_str(), m_File);
}

void CTextFileWritter::WriteInt(const string &key, int value)
{
    if (m_File != NULL)
        fputs(string(key + "=" + std::to_string(value) + "\n").c_str(), m_File);
}

void CTextFileWritter::WriteBool(const string &key, bool value)
{
    if (m_File != NULL)
        fputs(string(key + "=" + (value ? "yes" : "no") + "\n").c_str(), m_File);
}

}; // namespace WISP_FILE
