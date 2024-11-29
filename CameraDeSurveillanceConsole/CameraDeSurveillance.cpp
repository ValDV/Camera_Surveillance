//*********************************************************************************************
//* Programme : CameraDeSurveillance.cpp                                      Date : 15/11/2024
//*--------------------------------------------------------------------------------------------
//* Dernière mise à jour : 15/11/2024
//*
//* Programmeurs : Lemaire Kévin                                              Classe : BTSCIEL2
//*                Tellier Néo
//*--------------------------------------------------------------------------------------------
//* But : Interface de contrôle d'une caméra de surveillance avec gestion du port série, 
//*       initialisation, déplacements, mode automatique, et gestion d'alimentation.
//* Programmes associés : ControleCamera.cpp
//*********************************************************************************************

#include "CameraDeSurveillance.h"
#include "ControleCamera.h"
#include <QSerialPortInfo>
#include <QThread>
#include <iostream>
#include <QString>

//---------------------------------------------------------------------------------------------
//* Constructeur de la classe CameraDeSurveillance, initialise l'interface et la connexion des boutons
//* Paramètres :
//*  - QWidget* parent : le widget parent (généralement une fenêtre principale)
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
CameraDeSurveillance::CameraDeSurveillance()
    : controleCamera(nullptr), port(nullptr)
{
    controleCamera = new ControleCamera();  // Création de l'objet ControleCamera


    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : availablePorts)
    {
        // ui.portChoiceComboBox->addItem(info.portName(), QVariant(info.portName()));
    }

    // Initialisation du serveur WebSocket
    webSocketServer = new WebSocketServer(controleCamera, this);
    if (!webSocketServer->startServer(12345)) {  // Port WebSocket
        std::cout << "Échec du démarrage du serveur WebSocket." << std::endl;
    }
}

//---------------------------------------------------------------------------------------------
//* Destructeur de la classe CameraDeSurveillance, libère les ressources allouées
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
CameraDeSurveillance::~CameraDeSurveillance()
{
    if (controleCamera)
    {
        delete controleCamera;
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction pour ouvrir un port série pour la communication avec la caméra
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::openPort()
{

}

//---------------------------------------------------------------------------------------------
//* Fonction pour initialiser la caméra en envoyant une commande spécifique
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::camInitialisation()
{
    controleCamera->camInitialisation();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour allumer la caméra en envoyant une commande spécifique pour l'alimentation
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::powerOn()
{
    controleCamera->powerON();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers le haut
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveUp()
{
    controleCamera->MoveUp();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers le bas
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveDown()
{
    controleCamera->MoveDown();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers la gauche
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveLeft()
{
    controleCamera->MoveLeft();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour déplacer la caméra vers la droite
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveRight()
{
    controleCamera->MoveRight();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour activer le mode automatique de la caméra
//* Paramètres :
//*  Aucun paramètre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::autoMode()
{
    controleCamera->autoMode();
}
