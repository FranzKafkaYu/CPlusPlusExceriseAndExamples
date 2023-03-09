/**
 * @file OwnProperty.h
 * @brief
 * @author FranzKafkaYu 
 * @version 1.0
 * @date 2022-12-28
 *
 * @copyright Copyright (c) 2022  
 *
 */

#ifndef ANDROID_PROPERTY_UTIL_H
#define ANDROID_PROPERTY_UTIL_H

#include "stdio.h"

#include <map>
#include <string>
#include <utils/Vector.h>
#include <utils/RefBase.h>
#include <cutils/properties.h>


using std::map;
using android::sp;
using std::string;
using android::Vector;

//核心在于系统API：property_set与property_get

inline bool setOwnProperty(const char *const propertyName, bool value);
inline bool getOwnProperty(const char *const propertyName, bool *const pValue);
inline bool getOwnProperty(const char *const propertyName, bool defaultValue);
inline bool setOwnProperty(const char *const propertyName, int value);
inline bool getOwnProperty(const char *const propertyName, int *const pValue);
inline int getOwnProperty(const char *const propertyName, int defaultValue);
inline bool setOwnProperty(const char *const propertyName, const string &value);
inline string getOwnProperty(const char *const propertyName, const char *const defaultValue);
inline string getOwnProperty(const char *const propertyName, const string &defaultValue);

//属性值为bool
/**
 * @brief Set the Own Property object
 * 
 * @param  propertyName     
 * @param  value            true or false，will be transform to 1 or 0
 * @return true 
 * @return false 
 */
inline bool setOwnProperty(const char *const propertyName, bool value)
{
    char valueStr[5] = {};
    snprintf(valueStr, sizeof(valueStr), "%s", (value ? "1" : "0"));
    int ret = property_set(propertyName, valueStr);
    return ret == 0;
}

inline bool getOwnProperty(const char *const propertyName, bool *const pValue)
{
    char valueStr[PROPERTY_VALUE_MAX];
    property_get(propertyName, valueStr, "-1");
    if (strcmp(valueStr, "1") == 0)
    {
        *pValue = true;
        return true;
    }
    else if (strcmp(valueStr, "0") == 0)
    {
        *pValue = false;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Get the Own Property object，
 * 
 * @param  propertyName     
 * @param  defaultValue     bool类型
 * @return true 
 * @return false 
 */
inline bool getOwnProperty(const char *const propertyName, bool defaultValue)
{
    bool value = false;
    bool ret = getOwnProperty(propertyName, &value);
    if (!ret)
    {
        value = defaultValue;
        setOwnProperty(propertyName, defaultValue);
    }
    return value;
}

/**
 * @brief Set the Own Property object
 * 
 * @param  propertyName     
 * @param  value            int类型
 * @return true 
 * @return false 
 */
inline bool setOwnProperty(const char *const propertyName, int value)
{
    char valueStr[PROPERTY_VALUE_MAX];
    int len = snprintf(valueStr, sizeof(valueStr), "%d", value);
    if (len > PROPERTY_VALUE_MAX)
    {
        return false;
    }
    int ret = property_set(propertyName, valueStr);
    return ret == 0;
}

//属性值为int
inline bool getOwnProperty(const char *const propertyName, int *const pValue)
{
    char valueStr[PROPERTY_VALUE_MAX];
    property_get(propertyName, valueStr, "null");
    if (strcmp(valueStr, "null") == 0)
    {
        return false;
    }
    *pValue = atoi(valueStr);
    return true;
}

/**
 * @brief Get the Own Property object，带默认值的属性，若获取不到属性，则会下设默认值，并将默认值返回
 * 
 * @param  propertyName     My Param doc
 * @param  defaultValue     My Param doc
 * @return int 
 */
inline int getOwnProperty(const char *const propertyName, int defaultValue)
{
    int value = -1;
    bool ret = getOwnProperty(propertyName, &value);
    if (!ret)
    {
        value = defaultValue;
        // 将默认值重新设置
        setOwnProperty(propertyName, defaultValue);
    }
    return value;
}

//属性值为字符串
inline bool setOwnProperty(const char *const propertyName, const string &value)
{
    if (value.length() >= PROPERTY_VALUE_MAX)
    {
        return false;
    }
    int ret = property_set(propertyName, value.c_str());
    return ret == 0;
}

inline string getOwnProperty(const char *const propertyName, const char *const defaultValue)
{
    char valueStr[PROPERTY_VALUE_MAX];
    property_get(propertyName, valueStr, defaultValue);
    return string(valueStr);
}

inline string getOwnProperty(const char *const propertyName, const string &defaultValue)
{
    return getOwnProperty(propertyName, defaultValue.c_str());
}

#endif