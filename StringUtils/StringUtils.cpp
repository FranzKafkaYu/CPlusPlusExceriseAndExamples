#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#ifdef LOG_MODULE_TAG
#undef LOG_MODULE_TAG
#endif
#define LOG_MODULE_TAG "StringUtil"

#ifdef __cplusplus
extern "C"
{
#endif
    /**
 * @brief 将传入的一个字节数据转成16进制字符串，内容为
 *
 * @param [in]  input_Bytes    字符数组
 * @param [in]  BytesLen  字符数组长度
 * @param [out] output_HexStr  转换后的字符串存放BUF
 * @param [in]  out_len   存放转换后字符串BUF的大小
 * 
 * @return 
 *   On success, 转换后的字符串长度 is returned.  On error, 0 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-12-11    18:26:28
 */
    int32_t StrUtils_DigitToStr(uint64_t vInputDigit, char *pvStrOutput)
    {
        char pvTempBuf[128] = {0};

        if (pvStrOutput == NULL)
        {
            ALOGE("Error,pvStrOutput is Null");
            return -1;
        }

        snprintf(pvTempBuf, 32, "%lu", vInputDigit);

        strncpy(pvStrOutput, pvTempBuf, strlen(pvTempBuf));

        ALOGD("vInputDigit:%lu pvTempBuf:%s pvStrOutput:%s len:%zu\n", vInputDigit, pvTempBuf, pvStrOutput, strlen(pvTempBuf));

        return 0;
    }
    /**
 * @brief 将Bytes转成16进制字符串，转换核心：一个字节的数据需要两个字节存放，
 * 如字节数据0x28，转换为string时，需要用两个字节的数据存放['2']['8'],具体内容应该为对应的Ascii码
 *
 * @param [in]  input_Bytes    字符数组
 * @param [in]  BytesLen  字符数组长度
 * @param [out] output_HexStr  转换后的字符串存放BUF
 * @param [in]  out_len   存放转换后字符串BUF的大小
 * 
 * @return 
 *   On success, 转换后的字符串长度 is returned.  On error, 0 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-12-11    18:26:28
 */
    int StrUtils_BytesToHexStr(uint8_t *pInBytes, uint32_t BytesLen, char *output_HexStr, uint32_t out_len)
    {
        uint32_t i;
        if ((output_HexStr == NULL) || (input_Bytes == NULL))
        {
            ALOGE("Error:input_Bytes or output_HexStr is Null");
            return -1;
        }

        char *pTempBuf = NULL;

        if (BytesLen > 0)
        {
            pTempBuf = (char *)calloc(1, BytesLen * 2 + 1);
            if (pTempBuf == NULL)
            {
                ALOGE("pTempBuf is null,calloc failed");
                return -1;
            };

            for (i = 0; i < BytesLen; i++)
            {
                sprintf(&pTempBuf[i * 2], "%02X", input_Bytes[i]);
            }

            pTempBuf[BytesLen * 2] = '\0'; 

            strncpy(output_HexStr, pTempBuf, out_len);

            free(pTempBuf);
            pTempBuf = NULL;

            return strlen(output_HexStr);
        }
        else
        {
            return 0;
        }
    }

    /**
 * @brief 将Bytes转成带分隔符的16进制字符串
 *
 * @param [in]  pInBytes    字符数组
 * @param [in]  inBytesLen  字符数组长度
 * @param [out] pOutHexStr  转换后的字符串存放BUF
 * @param [in]  outStrLen   存放转换后字符串BUF的大小
 * @param [in]  delimiter   分隔符
 * 
 * @return 
 *   On success, 转换后的字符串长度 is returned.  On error, 0 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-12-11    18:26:28
 */
    int32_t StrUtils_BytesToHexStrWithDelimiter(uint8_t *pInBytes, uint32_t inBytesLen, char *pOutHexStr, uint32_t outStrLen, const char delimiter)
    {
        uint32_t i;
        if ((pInBytes == NULL) || (pOutHexStr == NULL))
        {
            ALOGE("Error:pInBytes or pOutHexStr is Null");
            return -1;
        }
        char *pTempBuf = NULL;

        if (inBytesLen > 0)
        {
            pTempBuf = (char *)calloc(1, inBytesLen * 3 + 1);
            if (pTempBuf == NULL)
            {
                ALOGE("pTempBuf is null,calloc failed");
                return -1;
            }

            for (i = 0; i < inBytesLen; i++)
            {
                sprintf(&pTempBuf[i * 3], "%02X%c", pInBytes[i], delimiter);
            }
			//字符串结尾均为'\0'
            pTempBuf[inBytesLen * 3] = '\0';

            strncpy(pOutHexStr, pTempBuf, outStrLen);

            free(pTempBuf);
            pTempBuf = NULL;

            return strlen(pOutHexStr);
        }
        else
        {
            return 0;
        }
    }
    /**
 * @brief 将16进制字符串转为Byte数组01->0x00 0x01
 *
 * @param [in]  input_HexStr 字符串
 * @param [out] output_Bytes   转换后的数组存放BUF
 * 
 * @return 
 *   On success, 0 is returned.  On error, 0 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-12-11    18:26:28
 */
    int StrUtils_HexStrToBytes(char *input_HexStr, uint8_t output_Bytes[])
    {
        uint32_t i = 0;
        uint32_t tmp = 0;

        char *pTempHexStr = NULL;

        if ((input_HexStr == NULL) || (output_Bytes == NULL))
        {
            ALOGE("Error:input_HexStr or output_Bytes is Null");
            return -1;
        }

        pTempHexStr = (char *)calloc(1, strlen(input_HexStr) + 2);
        if (pTempHexStr == NULL)
        {
            ALOGE("pTempHexStr is null,calloc failed");
            return -1;
        }

        if (strlen(input_HexStr) % 2 != 0)
        {
            pTempHexStr[0] = '0';
            strncat(pTempHexStr, input_HexStr, strlen(input_HexStr));
        }
        else
        {
            strncpy(pTempHexStr, input_HexStr, strlen(input_HexStr));
        }

        for (i = 0; i < strlen(pTempHexStr); i += 2)
        {
            sscanf(&pTempHexStr[i], "%02X", &tmp);

            *output_Bytes = tmp;

            output_Bytes++;
        }

        free(pTempHexStr);
        pTempHexStr = NULL;

        return 0;
    }
    /**
 * @brief 将16进制字符串转为数组
 *
 * @param [in]  input_HexStr 字符串
 * @param [out] pOutHexStr   转换后的字符串存放BUF
 * @param [in]  outStrLen    存放转换后字符串BUF的大小
 * @param [in]  delimiter    分隔符
 * 
 * @return 
 *   On success, 转换后的字符串长度 is returned.  On error, 0 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-12-11    18:26:28
 */
    int StrUtils_BytesToHexStrPrint(uint8_t input_Bytes[], uint32_t BytesLen)
    {
        uint32_t i;
        char *pTempBuf = NULL;

        if (input_Bytes == NULL)
        {
            ALOGE("Error:input_Bytes is Null");
            return -1;
        }

        if (BytesLen > 0)
        {
            pTempBuf = (char *)calloc(1, (BytesLen << 2) + 1);
            if (pTempBuf == NULL)
            {
                ALOGE("pTempBuf is null,calloc failed");
                return -1;
            }

            for (i = 0; i < BytesLen; i++)
            {
                snprintf(&pTempBuf[i * 2], 2 + 1, "%02X", input_Bytes[i]);
            }
            LOGI("input_Bytes:%s BytesLen:%zu\r\n", pTempBuf, strlen(pTempBuf));

            free(pTempBuf);
            pTempBuf = NULL;
        }

        return 0;
    }

    int32_t StrUtils_StrToAsciiBytes(const char *vInputStr, struct BytesData *pvBytesOutput)
    {
        uint32_t i;

        if ((vInputStr == NULL) || (pvBytesOutput == NULL))
        {
            ALOGE("Error:vInputStr or pvBytesOutput is Null");
            return -1;
        }

        for (i = 0; i < strlen(vInputStr); i++)
        {
            pvBytesOutput->Bytes[i] = vInputStr[i];
        }
        pvBytesOutput->BytesLen = strlen(vInputStr);

        return 0;
    }

    /**
 * @brief change str to Upper
 *
 * @param  str [in] string which to be changed to upper
 * 
 * @return 
 *   Return the changed string. 
 * 
 * @Owner		: FranzKafka
 * @Date&Time	: 2018-08-03	08:53:58 AM
 */
    char *StrUtils_StrToUpper(char *str)
    {
        char *orign = str;

        for (; *str != '\0'; str++)
        {
            *str = toupper(*str);
        }

        return orign;
    }

    int StrUtils_HexToStr(uint8_t input_Bytes[], uint32_t BytesLen, char *output_HexStr)
    {
        uint32_t i;

        char *pTempBuf = NULL;

        if (BytesLen > 0)
        {
            pTempBuf = (char *)calloc(1, BytesLen << 2);
            if (pTempBuf == NULL)
            {
                ALOGE("pTempBuf is null,calloc failed");
                return -1;
            }
            for (i = 0; i < BytesLen; i++)
            {
                snprintf(&pTempBuf[i * 3], 3 + 1, "%x ", input_Bytes[i]);
            }

            strncpy(output_HexStr, pTempBuf, strlen(pTempBuf));

            free(pTempBuf);
            pTempBuf = NULL;

            return strlen(output_HexStr);
        }
        else
        {
            return 0;
        }
    }

    /**
 * @brief 获取两个字符中间的字符串
 *
 * @param [in]inputStr 源字符串
 * @param [in]forwardChar 第一个字符
 * @param [in]nextChar 第二个字符
 * @param [out]outputStr 两个字符中间的字符串

 * 
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-07-23    10:07:26
 */
    int32_t StrUtils_GetMiddleStr(char const *inputStr, char const forwardChar, char const nextChar, char *outputStr)
    {
        if (inputStr == NULL || outputStr == NULL)
        {
            ALOGE("this pointer is NULL");
            return -1;
        }

        char *p_buf = NULL;
        char *forward_seat = NULL;
        char *next_seat = NULL;

        p_buf = (char *)calloc(1, strlen(inputStr) + 1);
        if (p_buf == NULL)
        {
            ALOGE("p_buf is null,calloc failed");
            return -1;
        }

        strncpy(p_buf, inputStr, strlen(inputStr));
        forward_seat = strchr(p_buf, forwardChar);
        if (forward_seat != NULL)
        {
            next_seat = strchr(forward_seat + 1, nextChar);
            if (next_seat != NULL)
            {
                *next_seat = '\0';
                strncpy(outputStr, forward_seat + 1, next_seat - forward_seat - 1);
            }
        }

        free(p_buf);
        p_buf = NULL;
        return 0;
    }

    /**
 * @brief 去除字符串中的空格和换行
 *
 * @param [in]inputChar 输入字符串
 * @param [out]输出字符串
 * 
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned. 
 * 
 * @Owner     : FranzKafka
 * @Date&Time : 2019-07-24    13:06:37
 */
    int32_t StrUtils_DelSpaceChar(char *inputChar, char *outputChar)
    {
        uint16_t i = 0;

        if (inputChar == NULL || outputChar == NULL)
        {
            ALOGE("this pointer is NULL");
            return -1;
        }
        for (i = 0; i < strlen(inputChar); i++)
        {
            if (inputChar[i] != 0x20 && inputChar[i] != 0x0A)
            {
                *(outputChar++) = inputChar[i];
            }
        }
        return 0;
    }

#ifdef __cplusplus
}
#endif
