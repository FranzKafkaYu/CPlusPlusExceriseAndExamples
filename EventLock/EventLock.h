#ifndef EVENT_LOCK_H
#define EVENT_LOCK_H

#include <mutex>
#include <condition_variable>

class EventLock
{
private:
    std::mutex mEventLock;
    std::condition_variable mEventCondition

public:
    EventLock() {}
    ~EventLock() {}
    bool waitForEvent(int timeoutMs = 0)
    {
        std::unique_lock<std::mutex> g(mEventLock);
        std::cv_status result = std::cv_status::no_timeout;
        if (timeoutMs == 0)
        {
            mEventCondition.wait(g);
            return true;
        }
        else
        {
            std::cv_status result = mEventCondition.wait_for(g, std::chrono::milliseconds(timeoutMs));
            return (result == std::cv_status::no_timeout);
        }
    }

    bool waitForEvent(std::function<bool()> predicate, int timeoutMs = 0)
    {
        std::unique_lock<std::mutex> l(mEventLock);
        std::cv_status result = std::cv_status::no_timeout;
        if (timeoutMs == 0)
        {
            mEventCondition.wait(l, predicate);
            return true;
        }
        else
        {
            return mEventCondition.wait_for(l, std::chrono::milliseconds(timeoutMs), predicate);
        }
    }

    void postEvent(bool notifyAll = false)
    {
        std::lock_guard<std::mutex> g(mEventLock);
        if (notifyAll)
        {
            mEventCondition.notify_all();
        }
        else
        {
            mEventCondition.notify_one();
        }
    }
};

#endif // __EVENT_LOCK_H__