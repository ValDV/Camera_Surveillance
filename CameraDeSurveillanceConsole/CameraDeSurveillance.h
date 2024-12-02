#pragma once

#include <QObject>
#include <QSerialPort>
#include "ControleCamera.h"
#include "WebSocketServer.h"

class CameraDeSurveillance : public QObject
{
    Q_OBJECT

private:
    ControleCamera* controleCamera;
    QSerialPort* port;
    bool waitingForConfirmation = false;
    WebSocketServer* webSocketServer;

public:
    CameraDeSurveillance();
    ~CameraDeSurveillance();

private slots:
    void openPort();
    void camInitialisation();
    void powerOn();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void autoMode();

};
