#ifndef QTMPACKAGE_H
#define QTMPACKAGE_H

#include <QObject>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

/*
 * Package the binary data to ByteArray
 *
 * Why use it? add some check and length info into the data
 *
 * Version: 0.2
 */


#define TM_CMD_Video "video"
#define TM_CMD_Audio "audio"
#define TM_CMD_Say "say"
#define TM_CMD_TTS "tts"
#define TM_CMD_Led1 "led1"
#define TM_CMD_Led2 "led2"
#define TM_CMD_Face "face"
#define TM_CMD_Wheel "wheel"
#define TM_CMD_Head_LR "head_lr"
#define TM_CMD_Head_UD "head_up"
#define TM_CMD_Hand_L_Swing "hand_l_swing"
#define TM_CMD_Hand_L_Expand "hand_l_expand"
#define TM_CMD_Hand_R_Swing "hand_r_swing"
#define TM_CMD_Hand_R_Expand "hand_r_expand"

#define TM_CMD_Mode_Set "mode_set"
#define TM_CMD_Mode_Get "mode_get"

#define TM_CMD_UpdateInfo "update_info"

#define TM_CMD_Action "action"
#define TM_CMD_Head "head"
#define TM_CMD_Script "script"
#define TM_CMD_Navi "navi"



class QTmPackage : public QObject
{
    Q_OBJECT
public:
    explicit QTmPackage(QObject *parent = 0);

    enum PackageType{
        Type_Unknown = 0,
        Type_Audio, // 音频
        Type_Video, // 视频
        Type_Control, // 命令类型
        Type_ACK  // 回复类型
    };

    QTmPackage(PackageType type, QByteArray& dataArray, QObject* parent = 0);
    QTmPackage(PackageType type, const char* data, int nLen, QObject* parent = 0);

    void setType(PackageType type);
    PackageType getType();

    void setData(QByteArray& dataArry);
    QByteArray getData();

    // Msg head
    QString getHead();
    /*
     * uuid: check permission
     */
    void setUUID(QString uuid);
    QString getUUID();
    /*
     * 获取打包完成的数据
     */
    QByteArray get_full_package();
    /*
     * 解析包头，至少9个字节, 解析失败返回false
     */
    int parse_Head(QByteArray& bArry, PackageType* type=NULL);

public: //static control and ack parse / make
    QByteArray make_full_package(PackageType type, QByteArray& bArry);
    QByteArray parse_full_package(PackageType type, QByteArray& bArry);

    QByteArray make_full_package_video(QByteArray& bArry);
    QByteArray make_full_package_video(const char* data, int nLen);
    QByteArray parse_full_package_video(QByteArray& bArry);

    QByteArray make_full_package_audio(QByteArray& bArry);
    QByteArray make_full_package_audio(const char* data, int nLen);
    QByteArray parse_full_package_audio(QByteArray& bArry);

    QByteArray make_full_package_control(QByteArray jsCmd);
    QByteArray parse_full_package_control(QByteArray& bArry);

    QByteArray make_full_package_ack(QByteArray jsCmd);
    QByteArray parse_full_package_ack(QByteArray& bArry);


    QString make_json_cmd_bool(QString strCmd, bool bOpen);
    QString make_json_cmd_string(QString strCmd, QString strString);
    QString make_json_cmd_params(QString strCmd, QString strParams);
    QString make_json_ack(bool bOk, QString strCmd, QString strMsg="");
    QString make_json_ack(bool bOk, QString strCmd, int nNum);
    // -1 not found, 0 find, >1 error
    int parse_json_cmd_object(QByteArray jsCmd, QString strCmd, QJsonValue& jsVal, QString* pUUID=0); // default one
    int parse_json_cmd_bool(QByteArray jsCmd, QString strCmd, bool& bOk, QString* pUUID=0);
    int parse_json_cmd_int(QByteArray jsCmd, QString strCmd, int& nValue, QString* pUUID=0);
    int parse_json_cmd_array(QByteArray jsCmd, QString strCmd, QJsonArray& arrObj, QString* pUUID=0);

    //
    int parse_json_ack(QByteArray jsCmd, bool& bOk, QString& strCmd, QJsonValue* valMsg = 0);

signals:

public slots:

private:
    PackageType m_type;
    QString m_strHead;
    QByteArray m_data;
    QString m_strUUID;

    int findHead(QByteArray& bArry);
};


#endif // QTMPACKAGE_H
