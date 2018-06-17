
#ifndef AUTORESETEVENT_H
#define AUTORESETEVENT_H

class AutoResetEvent
{
public:
    explicit AutoResetEvent(bool initial = false);

    void Set();
    void Reset();

    bool WaitOne();

private:
    AutoResetEvent(const AutoResetEvent &);
    AutoResetEvent &operator=(const AutoResetEvent &);
    bool flag_;
    std::mutex protect_;
    std::condition_variable signal_;
};

#endif
