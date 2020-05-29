#include "RBTCPClient.h"

RBTCPClient::RBTCPClient(QObject *parent)
    : QTcpSocket(parent)
{
    RBConnectionState = RBLAN_CS_DISCONNECTED;
    RBClientSocket = new QTcpSocket(this);

    connect(RBClientSocket, SIGNAL(connected()), this, SLOT(RBClientConnected()));
    connect(RBClientSocket, SIGNAL(disconnected()), this, SLOT(RBClientDisconnected()));
}
RBTCPClient::~RBTCPClient()
{
}

void RBTCPClient::RBClientConnected(){
    RBConnectionState = RBLAN_CS_CONNECTED;
    FILE_LOG(logINFO) << "Client is connected..!! (" << PortName.toStdString().data() << ")";
    connect(RBClientSocket, SIGNAL(readyRead()), this, SLOT(ReadData()));
    emit SIG_Connected();
}

void RBTCPClient::RBClientDisconnected(){
    RBConnectionState = RBLAN_CS_DISCONNECTED;
    FILE_LOG(logINFO) << "Client is disconnected..!! (" << PortName.toStdString().data() << ")";
    emit SIG_Disconnected();
}

void RBTCPClient::RBConnect(QString _ip, quint16 _port){
    RBIpAdress = _ip;
    RBPortNumber = _port;
    RBClientSocket->connectToHost(_ip, _port);
}

void RBTCPClient::RBDisconnect(void){    
    RBClientSocket->close();
    printf("Disconnect\n");
}

void RBTCPClient::RBSendData(QByteArray &data)
{
//    std::cout << "Send Data : " << data.toStdString() << std::endl;
    RBClientSocket->write(data);
}

void RBTCPClient::RBSendData(char *data, int size)
{
    RBClientSocket->write(data, size);
}
