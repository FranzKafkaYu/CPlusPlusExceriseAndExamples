# 使用说明  
- 编译时需在Android.mk或者Android.bp内导入`libcutils`,`libutils`两个依赖库    
- 使用时直接导入该头文件即可使用  
- 核心依赖系统API:`property_set`,`property_get`  