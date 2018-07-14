
#ifndef WISPBINARYFILEWRITER_H
#define WISPBINARYFILEWRITER_H

namespace WISP_FILE
{
class CBinaryFileWriter : public WISP_DATASTREAM::CDataWriter
{
private:
    FILE *m_File{ NULL };

public:
    CBinaryFileWriter();

    virtual ~CBinaryFileWriter();

    bool Open(const string &path);
    bool Open(const wstring &path);

    void Close();

    void WriteBuffer();
};

}; // namespace WISP_FILE

#endif
