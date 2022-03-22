#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__
#include <stdint.h>
#include <stdlib.h>

#define BYTE_DATA_LEX_MAX 2048
#define STR_ONCE_DEFLATE_MAX_LEN 4096 //16384

#define BOOL_TO_STRING(b) (b) ? "true" : "false" 

#ifdef __cplusplus
extern "C"
{
#endif
  struct BytesData
  {
    uint8_t Bytes[BYTE_DATA_LEX_MAX];
    uint32_t BytesLen;
  };

  int StrUtils_BytesToHexStr(uint8_t input_Bytes[], uint32_t BytesLen, char *output_HexStr, uint32_t out_len);


  int32_t StrUtils_BytesToHexStrWithDelimiter(uint8_t *pInBytes, uint32_t inBytesLen, char *pOutHexStr, uint32_t outStrLen, const char delimiter);

  int StrUtils_HexStrToBytes(char *input_HexStr, uint8_t output_Bytes[]);

  int StrUtils_BytesToHexStrPrint(uint8_t input_Bytes[], uint32_t BytesLen);

  int32_t StrUtils_DigitToStr(uint64_t vInputDigit, char *pvStrOutput);
  int32_t StrUtils_StrToAsciiBytes(const char *vInputStr, struct BytesData *pvBytesOutput);

  char *StrUtils_StrToUpper(char *str);

  int StrUtils_HexToStr(uint8_t input_Bytes[], uint32_t BytesLen, char *output_HexStr);

  int32_t StrUtils_GetMiddleStr(char const *inputStr, char const forwardChar, char const nextChar, char *outputStr);

  int32_t StrUtils_DelSpaceChar(char *inputChar, char *outputChar);

#ifdef __cplusplus
}
#endif

#endif
