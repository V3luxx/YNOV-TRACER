/**
 * \file MyNTP.h
 * \page ntp Network Time Protocol (NTP)
 * \brief Quelle heure est il ?
 * 
 * Les cartes Arduino, ESP8266 et ESP32 ne disposent pas d’horloge temps réel. 
 * La seule information dont dispose la carte est le nombre de microsecondes écoulées depuis son dernier démarrage.
 * 
 * Nous allons donc récupérer l'heure auprès d'un serveur de temps NTP : Network Time Protocol.
 * Network Time Protocol (« protocole de temps réseau ») est un protocole qui permet de synchroniser,
 * via un réseau informatique, l'horloge locale d'ordinateurs sur une référence d'heure.
 * 
 * Nous pourrons ainsi horodater (timestamp) des mesures, connaître le temps écoulé entre deux événements, 
 * afficher l’heure courante sur une interface WEB, déclencher une action programmée ...
 * 
 * La bibliothèque NTPClient est nécessaire.
 * 
 * Fichier \ref MyNTP.h
 */

#include <NTPClient.h>           //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void getNTP(){
  MYDEBUG_PRINT("-NTP : ");
  // mise à jour de l'heure
  timeClient.update();
  // Affichage de l'heure
  MYDEBUG_PRINTLN(timeClient.getFormattedTime());

}

void setupNTP(){
  // On a besoin d'une connexion à Internet !
  if (WiFi.status() != WL_CONNECTED){
    setupWiFi();
  }  
  timeClient.begin();
  // Avec l'heure d'été, nous avons en France 2h (7200s) de décalage avec le méridien de Greenwich (Greenwich Meridian Time : GMT).
  timeClient.setTimeOffset(7200);
}
