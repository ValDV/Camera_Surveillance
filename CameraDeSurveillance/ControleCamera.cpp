//*********************************************************************************************
//* Programme : ControleCamera.cpp                                              Date : 15/11/2024
//*--------------------------------------------------------------------------------------------
//* Derni�re mise � jour : 15/11/2024
//*
//* Programmeurs : Lemaire K�vin                                               Classe : BTSCIEL2
//*                Tellier N�o
//*--------------------------------------------------------------------------------------------
//* But : G�rer les commandes de la cam�ra de surveillance via un port s�rie, y compris 
//*       l'ouverture du port, l'envoi des commandes, et le traitement des confirmations.
//* Programmes associ�s : CameraDeSurveillance.cpp
//*********************************************************************************************

#include "ControleCamera.h"
#include <QThread>
#include <QDebug>

ControleCamera::ControleCamera() {}

ControleCamera::~ControleCamera()
{
    if (port)
    {
        port->close();
        delete port;
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'ouvrir le port s�rie pour communiquer avec la cam�ra
//* Param�tres :
//*  - QSerialPort* newPort : le port s�rie � ouvrir pour la communication avec la cam�ra
//*
//* Valeur de retour : bool, vrai si le port est ouvert avec succ�s, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::openPort(QSerialPort* newPort)
{
    if (newPort)
    {
        // Fermer et supprimer le port existant si n�cessaire
        if (port && port->isOpen())
        {
            port->close();
        }

        // Affecter le nouveau port
        port = newPort;

        // Configuration du port s�rie
        port->setBaudRate(QSerialPort::Baud9600);
        port->setDataBits(QSerialPort::Data8);
        port->setParity(QSerialPort::NoParity);
        port->setStopBits(QSerialPort::OneStop);
        port->setFlowControl(QSerialPort::NoFlowControl);

        // Essayer d'ouvrir le port
        if (port->open(QIODevice::ReadWrite))
        {
            isportOpen = true;
            return true;
        }
        else
        {
            isportOpen = false;
            return false;
        }
    }
    return false;
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'initialiser la cam�ra en envoyant une commande sp�cifique
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::camInitialisation()
{
    if (checkPort())
    {
        writeToPort("81 01 06 04 FF");
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'allumer la cam�ra en envoyant une commande sp�cifique pour l'alimentation
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::powerON()
{
    if (checkPort())
    {
        writeToPort("81 01 04 00 02 FF");
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de d�placer la cam�ra vers le haut
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveUp()
{
    writeToPort("81 01 06 01 18 14 03 01 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de d�placer la cam�ra vers le bas
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveDown()
{
    writeToPort("81 01 06 01 18 14 03 02 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de d�placer la cam�ra vers la gauche
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveLeft()
{
    writeToPort("81 01 06 01 18 14 01 03 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de d�placer la cam�ra vers la droite
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::MoveRight()
{
    writeToPort("81 01 06 01 18 14 02 03 FF");
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'activer un mode automatique qui balaye la salle trois fois de suite
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::autoMode()
{
    writeToPort("81 01 06 03 18 14 09 09 09 09 00 00 00 00 FF");
    writeToPort("81 01 06 03 18 14 05 05 05 05 00 00 00 00 FF");
}

//* Fonction permettant d'ajuster le zoom de la cam�ra en fonction de la valeur du curseur
//* Param�tres :
//*  - int zoomValue : la valeur du zoom de 0 (large) � 1023 (rapproch�)
//* 
//* Valeur de retour : aucun
void ControleCamera::adjustZoom(int zoomValue)
{
    if (checkPort())
    {
        // La valeur du zoom varie entre 0 (large) et 1023 (rapproch�)
        // Conversion de zoomValue en hexad�cimal pour envoyer la commande correspondante.
        QString zoomCommand = QString("81 01 04 47 0%1 0%2 0%3 0%4 FF")
            .arg(zoomValue & 0xFF, 2, 16, QLatin1Char('0'))  // Premier octet
            .arg((zoomValue >> 8) & 0xFF, 2, 16, QLatin1Char('0'))  // Deuxi�me octet
            .arg("00")  // Valeur fixe
            .arg("00");  // Valeur fixe
        writeToPort(zoomCommand);  // Envoi de la commande � la cam�ra
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant de v�rifier si le port s�rie est ouvert pour la communication
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : bool, vrai si le port est ouvert, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::checkPort()
{
    if (port && port->isOpen())
    {
        return true;
    }

    qDebug() << "Erreur: Port s�rie non ouvert.";
    return false;
}

//---------------------------------------------------------------------------------------------
//* Fonction permettant d'�crire une commande sur le port s�rie pour la cam�ra
//* Param�tres :
//*  - QString command : la commande � envoyer � la cam�ra sous forme de cha�ne hexad�cimale
//*
//* Valeur de retour : bool, vrai si la commande a �t� envoy�e avec succ�s, sinon faux.
//---------------------------------------------------------------------------------------------
bool ControleCamera::writeToPort(const QString& command)
{
    if (!checkPort())
    {
        return false;
    }

    QByteArray data = QByteArray::fromHex(command.toLatin1());
    qint64 bytesWritten = port->write(data);

    if (bytesWritten == -1)
    {
        qDebug() << "Erreur lors de l'envoi de la commande: " << port->errorString();
        return false;
    }

    // Attente de confirmation (port peut �tre en mode asynchrone)
    this->waitingForConfirmation = true;

    return true;
}

//---------------------------------------------------------------------------------------------
//* Fonction appel�e lorsque des donn�es sont re�ues depuis le port s�rie, permettant de traiter la r�ponse
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void ControleCamera::onSerialPortReadyRead()
{
    QByteArray receivedData = port->readAll();

    // Traitement des donn�es re�ues
    qDebug() << "Donn�es re�ues: " << receivedData;

    // Si la confirmation est re�ue (par exemple, un 'Q' ou 'R' dans la r�ponse)
    if (receivedData.contains('Q') || receivedData.contains('R'))
    {
        waitingForConfirmation = false;
        qDebug() << "R�ponse re�ue, confirmation!";
    }
}