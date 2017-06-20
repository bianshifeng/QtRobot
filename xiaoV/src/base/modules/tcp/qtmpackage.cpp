#include "qtmpackage.h"
#include <QDebug>

QTmPackage::QTmPackage(QObject *parent) : QObject(parent)
{
    m_data.clear();
    m_strHead = "Tami";
    m_strUUID = "tami_test_uuid";
}

QTmPackage::QTmPackage(QTmPackage::PackageType type, QByteArray &dataArray, QObject *parent)
{   
    Q_UNUSED(parent);
    m_type = type;
    m_data = dataArray;
}

QTmPackage::QTmPackage(QTmPackage::PackageType type, const char *data, int nLen, QObject *parent)
{
    Q_UNUSED(parent);
    m_type = type;
    m_data = QByteArray(data, nLen);
}

void QTmPackage::setType(QTmPackage::PackageType type)
{
    m_type = type;
}

QTmPackage::PackageType QTmPackage::getType()
{
    return m_type;
}

void QTmPackage::setData(QByteArray &dataArry)
{
    m_data = dataArry;
}

QByteArray QTmPackage::getData()
{
    return m_data;
}

QString QTmPackage::getHead()
{
    return m_strHead;
}

QString QTmPackage::getUUID()
{
    return m_strUUID;
}

QByteArray QTmPackage::get_full_package()
{
    QByteArray arrRet;
    arrRet.append(m_strHead);
    int nDataSize = m_data.size()+m_strHead.length()+4+1;
    arrRet.append((char*)&nDataSize, 4);
    arrRet.append((char)m_type);
    arrRet.append(m_data);
    //qDebug()<<m_type<<nDataSize;
    return arrRet;
}

void QTmPackage::setUUID(QString uuid)
{
    m_strUUID = uuid;
}

int QTmPackage::parse_Head(QByteArray &bArry, PackageType* type)
{
    if( bArry.size()<9 ){
        return -1;
    }
    if( !bArry.startsWith(m_strHead.toUtf8()) ){
        int nPos = findHead(bArry);

        if( nPos < 0 ){
            return -1;
        }else{
            bArry.remove(0, nPos);
        }
    }
    QByteArray lenArry = bArry.mid(m_strHead.length(), 4);
    int nLen = *((int*)lenArry.data());
    if( type && nLen>0 ){
        *type = (PackageType)bArry.at(4+m_strHead.length());
    }
    return nLen;
}

QByteArray QTmPackage::make_full_package(QTmPackage::PackageType type, QByteArray &bArry)
{
    setType(type);
    setData(bArry);
    return get_full_package();
}

QByteArray QTmPackage::parse_full_package(QTmPackage::PackageType type, QByteArray &bArry)
{
    if( !bArry.startsWith(m_strHead.toUtf8()) ){
        return QByteArray();
    }
    QByteArray lenArry = bArry.mid(m_strHead.length(), 4);
    int nLen = *((int*)lenArry.data());
    if( nLen > bArry.length() ){
        return QByteArray();
    }
    char nType = bArry.at(m_strHead.length()+4);
    if( nType!=(char)type ){
        return QByteArray();
    }
    //QByteArray aaret = bArry.mid(m_strHead.length()+4+1, nLen-m_strHead.length()-4-1);
    return bArry.mid(m_strHead.length()+4+1, nLen-m_strHead.length()-4-1);
}

QByteArray QTmPackage::make_full_package_video(QByteArray &bArry)
{
    return make_full_package(Type_Video, bArry);
}

QByteArray QTmPackage::make_full_package_video(const char *data, int nLen)
{
    QByteArray buf(data,nLen);
    return make_full_package_video(buf);
}

QByteArray QTmPackage::parse_full_package_video(QByteArray &bArry)
{
    return parse_full_package(Type_Video, bArry);
}

QByteArray QTmPackage::make_full_package_audio(QByteArray &bArry)
{
    return make_full_package(Type_Audio, bArry);
}

QByteArray QTmPackage::make_full_package_audio(const char *data, int nLen)
{
    QByteArray buf(data,nLen);
    return make_full_package_audio(buf);
}

QByteArray QTmPackage::parse_full_package_audio(QByteArray &bArry)
{
    QByteArray bArray1 = parse_full_package(Type_Audio, bArry);
    int nArray1Len = bArray1.length();
    int nLeaveSize = bArry.length() - 9 - nArray1Len;
    while( nLeaveSize >=(nArray1Len+9) ){
        bArray1 += parse_full_package(Type_Audio, bArry);
        nLeaveSize = nLeaveSize - 9 - nArray1Len;
    }
    //qDebug()<<"Audio size:"<<nArray1Len<<bArry.length()<<bArray1.size();
    return bArray1;
}

QByteArray QTmPackage::make_full_package_control(QByteArray jsCmd)
{
    return make_full_package(Type_Control, jsCmd);
}

QByteArray QTmPackage::parse_full_package_control(QByteArray &bArry)
{
    return parse_full_package(Type_Control, bArry);
}

QByteArray QTmPackage::make_full_package_ack(QByteArray jsCmd)
{
    return make_full_package(Type_ACK, jsCmd);
}

QByteArray QTmPackage::parse_full_package_ack(QByteArray &bArry)
{
    return parse_full_package(Type_ACK, bArry);
}

QString QTmPackage::make_json_cmd_bool(QString strCmd, bool bOpen)
{
    /*  Only Camera and audio
        { "uuid":"tami_test_uuid", "cmd": {"video":true/false} }
    */
    QString strOpen = bOpen?"true":"false";
    QString strRet = QString("{\"uuid\":\"%1\",\"cmd\":{\"%2\":%3}}").arg(m_strUUID,strCmd,strOpen);
    return strRet;
}

QString QTmPackage::make_json_cmd_string(QString strCmd, QString strString)
{
    QString strRet = QString("{\"uuid\":\"%1\",\"cmd\":{\"%2\":\"%3\"}}").arg(m_strUUID,strCmd,strString);
    return strRet;
}

QString QTmPackage::make_json_cmd_params(QString strCmd, QString strParams)
{
    QString strRet = QString("{\"uuid\":\"%1\",\"cmd\":{\"%2\":%3}}").arg(m_strUUID,strCmd,strParams);
    return strRet;
}

int QTmPackage::parse_json_cmd_bool(QByteArray jsCmd, QString strCmd, bool &bOk, QString *pUUID)
{
    QJsonValue jsVal;
    int nRet = parse_json_cmd_object(jsCmd, strCmd, jsVal, pUUID);
    if( nRet==0 ){
        bOk = jsVal.toBool();
    }
    return nRet;
}

int QTmPackage::parse_json_cmd_int(QByteArray jsCmd, QString strCmd, int &nValue, QString *pUUID)
{
    QJsonValue jsVal;
    int nRet = parse_json_cmd_object(jsCmd, strCmd, jsVal, pUUID);
    if( nRet==0 ){
        nValue = jsVal.toInt();
    }
    return nRet;
}

int QTmPackage::parse_json_cmd_array(QByteArray jsCmd, QString strCmd, QJsonArray &arrObj, QString *pUUID)
{
    QJsonValue jsVal;
    int nRet = parse_json_cmd_object(jsCmd, strCmd, jsVal, pUUID);
    if( nRet==0 ){
        arrObj = jsVal.toArray();
    }
    return nRet;
}
///提供jsVal为一个数据容器，用于装载从包里获取到的数据
/// *puuid 是用来接收一个包的id
int QTmPackage::parse_json_cmd_object(QByteArray jsCmd, QString strCmd, QJsonValue &jsVal, QString *pUUID)
{
    QJsonDocument jsDoc;
    QJsonParseError jsError;
    //静态方法直接把bytearray数据，解析成jsondoc 1；读取错误
    jsDoc = QJsonDocument::fromJson(jsCmd, &jsError);
    if( jsError.error != QJsonParseError::NoError ){
        return 1;
    }

    //处理数据
    QJsonObject root = jsDoc.object();

    // { "uuid":"xxx", "cmd":{ "video":true  } }

    if( root.contains("uuid")){
        if( pUUID )
            *pUUID = root.value("uuid").toString();
    }else{
        return 2;
    }
    if( root.contains("cmd") ){
        QJsonObject jsCmd = root.value("cmd").toObject();
        if( jsCmd.contains(strCmd) ){
            jsVal = jsCmd.value(strCmd);
            return 0;
        }
    }
    return -1;
}

int QTmPackage::parse_json_ack(QByteArray jsCmd, bool &bOk, QString &strCmd, QJsonValue *valMsg)
{
    QJsonDocument jsDoc;
    QJsonParseError jsError;
    jsDoc = QJsonDocument::fromJson(jsCmd, &jsError);
    if( jsError.error != QJsonParseError::NoError ){
        return 1;
    }
    QJsonObject root = jsDoc.object();
    if( root.contains("ok") ){
        bOk = root.value("ok").toBool();
    }else{
        return 2;
    }
    if( root.contains("msg") && valMsg ){
        *valMsg = root.value("msg");
    }
    // cmd
    if( root.contains("cmd") ){
        strCmd = root.value("cmd").toString();
        return 0;
    }
    return -1;
}

QString QTmPackage::make_json_ack(bool bOk, QString strCmd, QString strMsg)
{
    // { "ok":true/false, "cmd":"xxx", "msg":"" }
    QString strOk = bOk?"true":"false";
    QString strRet = QString("{\"ok\":%1,\"cmd\":\"%2\",\"msg\":\"%3\"}").arg(strOk,strCmd,strMsg);
    return strRet;
}

QString QTmPackage::make_json_ack(bool bOk, QString strCmd, int nNum)
{
    QString strOk = bOk?"true":"false";
    QString strRet = QString("{\"ok\":%1,\"cmd\":\"%2\",\"msg\":%3}").arg(strOk,strCmd).arg(nNum);
    return strRet;
}

int QTmPackage::findHead(QByteArray &bArry)
{
    return bArry.indexOf("Tami");
}

