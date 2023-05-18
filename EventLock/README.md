# 使用说明  
- Header-only,使用时直接导入该头文件  
- 使用方法：
```  
    EventLock mEventLock=new EventLock();
    //wait continusly
    mEventLock.waitForEvent();
    //wait for a specific time,unit:ms
    mEventLock.waitForEvent(40)  
    //post event,here you can choose whether notify all
    mEventLock.postEvent();  
```  