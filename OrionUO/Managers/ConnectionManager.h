

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class CConnectionManager
{
protected:
    bool m_UseProxy = false;

public:
    bool GetUseProxy() { return m_UseProxy; };
    void SetUseProxy(bool val);

protected:
    string m_ProxyAddress = "";

public:
    string GetProxyAddress() { return m_ProxyAddress; };
    void SetProxyAddress(const string &val);

protected:
    int m_ProxyPort = 0;

public:
    int GetProxyPort() { return m_ProxyPort; };
    void SetProxyPort(int val);

protected:
    bool m_ProxySocks5 = false;

public:
    bool GetProxySocks5() { return m_ProxySocks5; };
    void SetProxySocks5(bool val);

protected:
    string m_ProxyAccount = "";

public:
    string GetProxyAccount() { return m_ProxyAccount; };
    void SetProxyAccount(const string &val);

protected:
    string m_ProxyPassword = "";

public:
    string GetProxyPassword() { return m_ProxyPassword; };
    void SetProxyPassword(const string &val);

private:
    CSocket m_LoginSocket{ CSocket(false) };

    CSocket m_GameSocket{ CSocket(true) };

    bool m_IsLoginSocket{ true };

    uchar m_Seed[4];

    void SendIP(CSocket &socket, puchar seed);

public:
    CConnectionManager();
    ~CConnectionManager();

    void Init();

    void Init(puchar GameSeed);

    bool Connected() { return (m_LoginSocket.Connected || m_GameSocket.Connected); }

    bool Connect(const string &address, int port, puchar gameSeed);

    void Disconnect();

    void Recv();

    int Send(puchar buf, int size);

    int Send(const UCHAR_LIST &data);

    puchar GetClientIP() const { return (puchar)m_Seed; }
};

extern CConnectionManager g_ConnectionManager;

#endif
