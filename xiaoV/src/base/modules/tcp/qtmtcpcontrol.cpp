#include "qtmtcpcontrol.h"
#include "tmglobaltool.h"

QTmTcpControl::QTmTcpControl(QObject *parent) : QObject(parent)
{
    m_cmdDataSize = 0;
    m_curSocket = NULL;
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(new_tcp_connect()));
}

void QTmTcpControl::startServer(int nPort)
{
    if( m_server.isListening() ){
        return;
    }
    m_server.listen(QHostAddress::Any, nPort);
}

QString QTmTcpControl::getRemoteAddr()
{
    return m_addrRemoteIP.toString();
}

void QTmTcpControl::new_tcp_connect()
{
    if( m_curSocket ){
        if( m_curSocket->state() == QAbstractSocket::ConnectedState ) {
            m_curSocket->close();
            m_curSocket = NULL;
        }
        m_addrRemoteIP = QHostAddress();
        //disconnect(m_curSocket, SIGNAL(readyRead()), this, SLOT(new_tcp_data()));
    }
    if( m_server.hasPendingConnections() ){
        m_curSocket = m_server.nextPendingConnection();
        m_addrRemoteIP = m_curSocket->peerAddress();
        g_log("Remote Connect IP:"+m_addrRemoteIP.toString());
        connect(m_curSocket, SIGNAL(readyRead()), this, SLOT(new_tcp_data()));
        connect(m_curSocket, SIGNAL(disconnected()), this, SLOT(sock_disconnect()));

    }
}

void QTmTcpControl::new_tcp_data()
{
    if( !m_curSocket ) {
        return;
    }

    while(m_curSocket->bytesAvailable()) {

        m_cmdData += m_curSocket->readAll();
        if( m_curSocket->state() == QAbstractSocket::UnconnectedState
                || m_curSocket->state() == QAbstractSocket::ClosingState ){
            m_curSocket->close();
            m_curSocket = NULL;
            break;
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    }
    // process
    QTmPackage::PackageType type;
    int nSize = m_package.parse_Head(m_cmdData, &type);
    qDebug()<<"Tcp Control:"<<nSize<<type<<m_cmdData;
    if( nSize>0 ){
        m_cmdDataSize = nSize;
    }
    if( m_cmdDataSize <= m_cmdData.size() && m_cmdDataSize>10){
        QByteArray strJson = m_package.parse_full_package_control(m_cmdData);
        QJsonValue jsVal;
        g_log("Json Control cmd:"+strJson);
        //逐层进行解包，通过包类型，来emit 通讯过里的命令，进行server相应
        if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Video, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Video,"test"));
            emit signal_msg_cmd(TM_CMD_Video, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Audio, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Audio,"test"));
            emit signal_msg_cmd(TM_CMD_Audio, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Say, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Say,"test"));
            emit signal_msg_cmd(TM_CMD_Say, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Face, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Face,"test"));
            emit signal_msg_cmd(TM_CMD_Face, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Mode_Set, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Mode_Set,"test"));
            emit signal_msg_cmd(TM_CMD_Mode_Set, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Mode_Get, jsVal )){
            //write_ack(m_package.make_json_ack(true,TM_CMD_Mode_Get,"test")); // later return
            emit signal_msg_cmd(TM_CMD_Mode_Get, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Action, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Action,"test"));
            emit signal_msg_cmd(TM_CMD_Action, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_Head, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_Head,"test"));
            emit signal_msg_cmd(TM_CMD_Head, jsVal);
        }else if(0 == m_package.parse_json_cmd_object(strJson, TM_CMD_TTS, jsVal )){
            write_ack(m_package.make_json_ack(true,TM_CMD_TTS,"test"));
            emit signal_msg_cmd(TM_CMD_TTS, jsVal);
        }

        m_cmdData.remove(0, m_cmdDataSize);
        m_cmdDataSize = 0;
    }
}

void QTmTcpControl::sock_disconnect()
{
    m_curSocket->close();
    g_log(getRemoteAddr()+" disconnect!");
    m_addrRemoteIP = QHostAddress();
    disconnect(m_curSocket, SIGNAL(readyRead()), this, SLOT(new_tcp_data()));
    m_curSocket = NULL;

    emit signal_cur_disconnect();
}

void QTmTcpControl::write_ack(QString strJson)
{
    if( m_curSocket ){
        QByteArray bData = m_package.make_full_package_ack(strJson.toUtf8());
        m_curSocket->write(bData);
    }
}


