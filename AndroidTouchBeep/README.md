# 使用说明  
- 采用单例实现，可以全局管控  
- 音源采用wa格式作为原始输入  
- 使用时需要在Android.mk或Android.bp内囊括以下依赖库：  
```  
liblog
libmedia
libaudiohal
libaudiohal@6.0
libaudioclient
libaudioclient_aidl_conversion
android.hardware.audio@6.0-util
android.hardware.audio.common@6.0-util
libhidlmemory
libmedia_helper
libmediautils
```  