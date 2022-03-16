/**
 * @file EvsCrypto.h
 * @brief 
 * @author FranzKafkaYu (franzkafkayu@gmail.com)
 * @version 1.0
 * @date 2022-03-10
 * 
 * @copyright 
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description
 * <tr><td>2022-03-10 <td>1.0 <td>wangh <td>内容
 * </table>
 */

#ifndef __EVS_CRYPTO_H__
#define __EVS_CRYPTO_H__

//Avoid header files occupied repeatly
//#pragma once
#include <stdint.h>

/* typedef a 32 bit type */
//typedef unsigned long int UINT4;
typedef uint32_t UINT4;

#define EVS_CRYPTO_SINGLE_CRYPTO_LENGTH 8192

#ifdef __cplusplus
extern "C"
{
#endif

    /* Data structure for MD5 (Message Digest) computation */
    typedef struct
    {
        UINT4 i[2];               /* number of _bits_ handled mod 2^64 */
        UINT4 buf[4];             /* scratch buffer */
        unsigned char in[64];     /* input buffer */
        unsigned char digest[16]; /* actual digest after MD5Final call */
    } MD5_CTX;
    /**
     * @brief This  function will be used init md5 
     * 
     * @param [in] mdContext 
     * 
     * @details 
     */

    void MD5Init(MD5_CTX *mdContext);
    /**
     * @brief 
     * 
     * @param [in] mdContext 
     * @param [in] inBuf 
     * @param [in] inLen 
     * 
     * @details 
     */
    void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
    /**
     * @brief 
     * 
     * 
     * @param [in] mdContext 
     * 
     * @details 
     */
    void MD5Final(MD5_CTX *mdContext);
    /**
     * @brief 
     * 
     * @param [in] buf 
     * @param [in] in 
     * 
     * @details 
     */
    void Transform(UINT4 *buf, UINT4 *in);
    /**
     * @brief 
     * 
     * @param [in] path 
     * @param [in] md5_len 
     * 
     * @details 
     * @return char* 
     */

    char *MD5_file(char *path, int md5_len);

#ifdef __cplusplus
}
#endif

#endif