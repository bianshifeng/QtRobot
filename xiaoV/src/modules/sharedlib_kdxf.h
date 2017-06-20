#ifndef SHAREDLIB_KDXF_H
#define SHAREDLIB_KDXF_H
/*
 * 科大讯飞语音模块（离线版+部分在线） V1.1
 * log: 在原来的离线模块基础上，添加在线功能
 */
#include "sharedlib_kdxf_global.h"


#include "QString"
#include <QTextCodec>

#define MAX_UPDATA_LEN 12800 // max upload size once

typedef int SR_DWORD;
typedef short int SR_WORD ;

//音频头部格式
struct wave_pcm_hdr
{
    char            riff[4];                        // = "RIFF"
    SR_DWORD        size_8;                         // = FileSize - 8
    char            wave[4];                        // = "WAVE"
    char            fmt[4];                         // = "fmt "
    SR_DWORD        dwFmtSize;                      // = 下一个结构体的大小 : 16

    SR_WORD         format_tag;              // = PCM : 1
    SR_WORD         channels;                       // = 通道数 : 1
    SR_DWORD        samples_per_sec;        // = 采样率 : 8000 | 6000 | 11025 | 16000
    SR_DWORD        avg_bytes_per_sec;      // = 每秒字节数 : dwSamplesPerSec * wBitsPerSample / 8
    SR_WORD         block_align;            // = 每采样点字节数 : wBitsPerSample / 8
    SR_WORD         bits_per_sample;         // = 量化比特数: 8 | 16

    char            data[4];                        // = "data";
    SR_DWORD        data_size;                // = 纯数据长度 : FileSize - 44
} ;
// callback
int build_grm_cb(int errorCode, const char* info, void* userData);

class SHAREDLIB_KDXFSHARED_EXPORT Sharedlib_kdxf
{
public:
    enum Special_Type{
        Specail_Null = 0,
        Special_HuYuanHuJin,
        Special_HuiSheng,
        Special_Robot,
        Special_HeChang,
        Special_ShuiXia,
        Special_HunXiang,
        Special_YinYangGuaiQi,
        Special_Counts // 种类个数
    };

private:
    QString m_strSpeaker;
    int m_nSpeed;
    int m_nPitch;
    int m_nSpecial;
    int m_nNumberSay, m_nSpeedUp;

    QTextCodec* m_textCodec;

    // isr
    QString pr_strISR_params;
    const char* m_sessionID;


public:
    Sharedlib_kdxf();
    bool login(QString strAppID);
    void logout();
    void setDefaultParams(QString strSpeaker="xiaofeng", int nSpeed=51, int nPitch=51, int nSpecial=Specail_Null);
    int getTTSfile(QString strText, QString strFilePath, bool bCloud=true);
    int getTTSfile(QString strText, QString strFilePath, QString strSpeaker, int nSpeed=51, int nPitch=51, int nSpecial=Specail_Null, bool bCloud = true);
    // -------------
    int isr_start(bool bIsAsr=false, bool bCloud=true);
    void isr_stop();
    void isr_updateAudio(char* pData, int nLen);
    void isr_setGramID(QString strIDName);
    QString isr_getResult();

    // contact
    bool isr_build_bnf(QString strBnfFileName);
    bool isr_update_bnf(QString key, QString val);

    // callback
    QString m_strGamName;
};

#endif // SHAREDLIB_KDXF_H
