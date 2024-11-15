//*********************************************************************************************
//* Programme : CameraDeSurveillance.cpp                                      Date : 15/11/2024
//*--------------------------------------------------------------------------------------------
//* Derni�re mise � jour : 15/11/2024
//*
//* Programmeurs : Lemaire K�vin                                              Classe : BTSCIEL2
//*                Tellier N�o
//*--------------------------------------------------------------------------------------------
//* But : Interface de contr�le d'une cam�ra de surveillance avec gestion du port s�rie, 
//*       initialisation, d�placements, mode automatique, et gestion d'alimentation.
//* Programmes associ�s : ControleCamera.cpp
//*********************************************************************************************

#include "CameraDeSurveillance.h"
#include "ControleCamera.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QDebug>

//---------------------------------------------------------------------------------------------
//* Constructeur de la classe CameraDeSurveillance, initialise l'interface et la connexion des boutons
//* Param�tres :
//*  - QWidget* parent : le widget parent (g�n�ralement une fen�tre principale)
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
CameraDeSurveillance::CameraDeSurveillance(QWidget* parent)
    : QMainWindow(parent), controleCamera(nullptr), port(nullptr)
{
    ui.setupUi(this);
    controleCamera = new ControleCamera();  // Cr�ation de l'objet ControleCamera

    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo& info : availablePorts)
    {
        ui.portChoiceComboBox->addItem(info.portName(), QVariant(info.portName()));
    }

    setupConnections();  // Initialisation des connexions entre boutons et slots
}

//---------------------------------------------------------------------------------------------
//* Destructeur de la classe CameraDeSurveillance, lib�re les ressources allou�es
//* Param�tres :
//*  Aucun param�tre
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
//* Fonction pour �tablir les connexions entre les boutons de l'interface et les slots correspondants
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::setupConnections()
{
    connect(ui.OpenPortButton, &QPushButton::clicked, this, &CameraDeSurveillance::openPort);
    connect(ui.initbutton, &QPushButton::clicked, this, &CameraDeSurveillance::camInitialisation);
    connect(ui.powerbutton, &QPushButton::clicked, this, &CameraDeSurveillance::powerOn);
    connect(ui.moveUpButton, &QPushButton::clicked, this, &CameraDeSurveillance::moveUp);
    connect(ui.moveDownButon, &QPushButton::clicked, this, &CameraDeSurveillance::moveDown);
    connect(ui.moveLeftButton, &QPushButton::clicked, this, &CameraDeSurveillance::moveLeft);
    connect(ui.moveRightButon, &QPushButton::clicked, this, &CameraDeSurveillance::moveRight);
    connect(ui.autobutton, &QPushButton::clicked, this, &CameraDeSurveillance::autoMode); 
    connect(ui.zoomVerticalSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustZoom(int)));
}

//---------------------------------------------------------------------------------------------
//* Fonction pour ouvrir un port s�rie pour la communication avec la cam�ra
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::openPort()
{
    if (ui.portChoiceComboBox->currentIndex() < 0)
    {
        ui.portStatusLabel->setText("Veuillez s�lectionner un port.");
        return;
    }

    // Cr�ez un nouveau port s�rie et passez-le � ControleCamera
    port = new QSerialPort(ui.portChoiceComboBox->currentText());

    // V�rifiez si le port peut �tre ouvert via ControleCamera
    if (controleCamera->openPort(port))  // Appel de la m�thode openPort() dans ControleCamera
    {
        ui.portStatusLabel->setText("Statut port: Ouvert");
    }
    else
    {
        ui.portStatusLabel->setText("Erreur: " + port->errorString());
    }
}

//---------------------------------------------------------------------------------------------
//* Fonction pour initialiser la cam�ra en envoyant une commande sp�cifique
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::camInitialisation()
{
    controleCamera->camInitialisation();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour allumer la cam�ra en envoyant une commande sp�cifique pour l'alimentation
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::powerOn()
{
    controleCamera->powerON();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour d�placer la cam�ra vers le haut
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveUp()
{
    controleCamera->MoveUp();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour d�placer la cam�ra vers le bas
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveDown()
{
    controleCamera->MoveDown();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour d�placer la cam�ra vers la gauche
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveLeft()
{
    controleCamera->MoveLeft();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour d�placer la cam�ra vers la droite
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::moveRight()
{
    controleCamera->MoveRight();
}

//---------------------------------------------------------------------------------------------
//* Fonction pour activer le mode automatique de la cam�ra
//* Param�tres :
//*  Aucun param�tre
//*
//* Valeur de retour : aucun
//---------------------------------------------------------------------------------------------
void CameraDeSurveillance::autoMode()
{
    controleCamera->autoMode();
}