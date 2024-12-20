//*********************************************************************************************
//* Programme : WebSocketServer.cpp                                      Date : 29/11/2024
//*--------------------------------------------------------------------------------------------
//* Derni�re mise � jour : 29/11/2024
//*
//* Programmeurs : Rosier Valentin                                       Classe : BTSCIEL2
//*                Fauquembergue Yann
//*--------------------------------------------------------------------------------------------
//* But : Interface de gestion de serveur WebSocket 
//*       
//* 
//*********************************************************************************************

#include "WebSocketServer.h"
#include <QDebug>

WebSocketServer::WebSocketServer(ControleCamera* controleCamera, QObject* parent)
    : QObject(parent), cameraControl(controleCamera)
{
    server = new QWebSocketServer("Camera Control Server", QWebSocketServer::NonSecureMode, this);
}

WebSocketServer::~WebSocketServer()
{
    if (server->isListening()) {
        server->close();
    }
    qDeleteAll(clients);
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de mettre en �coute le serveur WebSocket.
//* Param�tres :
//*  - quint16 port : le port du serveur � mettre en �coute
//*
//* Valeur de retour : bool, vrai si le serveur est en �coute, sinon faux.
//---------------------------------------------------------------------------------------------
bool WebSocketServer::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Echec du d�marrage du serveur WebSocket: " << server->errorString();
        return false;
    }
    connect(server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
    qDebug() << "Serveur WebSocket en �coute sur le port " << port;
    return true;
}

//---------------------------------------------------------------------------------------------
//* Fonction qui est appel�e lorsqu'une nouvelle connexion client est �tablie.
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void WebSocketServer::onNewConnection()
{
    auto client = server->nextPendingConnection();
    clients.append(client);
    connect(client, &QWebSocket::textMessageReceived, this, &WebSocketServer::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &WebSocketServer::onClientDisconnected);
    qDebug() << "Nouvelle connexion �tablie.";
}

//---------------------------------------------------------------------------------------------
//* Fonction qui est appel�e lorsque le serveur re�oit un message.
//* Param�tres :
//*  const QString& message : Le message re�u
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void WebSocketServer::onTextMessageReceived(const QString& message)
{
    QWebSocket* senderClient = qobject_cast<QWebSocket*>(sender());
    qDebug() << "Message re�u de client: " << message;

    // Message �tait-il une commande ?
    if (message == "POWER_ON") cameraControl->powerON();
    else if (message == "INITIALIZE") cameraControl->camInitialisation();
    else if (message == "AUTO_MODE") cameraControl->autoMode();
    else if (message == "MOVE_UP") cameraControl->MoveUp();
    else if (message == "MOVE_DOWN") cameraControl->MoveDown();
    else if (message == "MOVE_LEFT") cameraControl->MoveLeft();
    else if (message == "MOVE_RIGHT") cameraControl->MoveRight();
    else if (message.startsWith("ZOOM_")) {
        QString zoomValueStr = message.mid(5);
        bool ok;
        int zoomValue = zoomValueStr.toInt(&ok);
        if (ok) {
            cameraControl->adjustZoom(zoomValue);
        }
        else {
            qDebug() << "Valeur zoom invalide:" << zoomValueStr;
        }
    }
    else if (message.startsWith("OPEN_PORT_")) {
        QString portString = message.mid(10).replace("_", " ");
        QSerialPort* port = new QSerialPort(portString);
        if (cameraControl->openPort(port)) {
            qDebug() << "Port ouvert avec succ�s: " << portString;
            if (senderClient) {
                senderClient->sendTextMessage("OPEN_PORT_SUCCESS");
            }
        }
        else {
            qDebug() << "Echec ouverture du port: " << portString;
            if (senderClient) {
                senderClient->sendTextMessage("OPEN_PORT_FAILURE");
            }
        }
    }
}



//---------------------------------------------------------------------------------------------
//* Fonction qui est appel�e lorsqu'un client se d�connecte du serveur.
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void WebSocketServer::onClientDisconnected()
{
    auto client = qobject_cast<QWebSocket*>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
    }
    qDebug() << "Client d�connect�.";
}
