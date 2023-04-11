/**
 * \file MyAdafruitIO.h
 * \page adafruitio Adafruit IO
 * \brief Adafruit IO Platform
 * 
 * <H2>Adafruit IO</H2>
 * Nous allons utiliser la plateforme Adafruit IO en guise de première approche de MQTT.
 * Pour ce faire, installez la bibliothèque "Adafruit IO Arduino" ainsi que toutes ses dépendances.
 * - Rendez vous sur la plateforme : https://io.adafruit.com/
 * - Enregistrez vous : https://accounts.adafruit.com/users/sign_up
 * - Une fois connecté, récupérez votre Aadafruit Active Key en cliquant sur "My Key" (clé jaune dans le bandeau),
 * - Copier vos identifiants pour Arduino (IO_USERNAME et IO_KEY) et coller dans la section
 *   configuration ci-dessous
 * - Dans la page des Feeds (topics MQTT), créer les Feeds suivants, sans les rattacher à un groupe : 
 *   - slider 
 *   - onoff
 *   - temperature
 *   - humidity
 *   
 *   Adafruit a défini la notion de Feeds qui est utilisé indifféremment pour des attributs et
 *   des données de télémétrie MQTT.
 * - Dans la page des Dashboards, créer un dashboard et éditer le. Dans celui-ci, ajouter les "blocks" suivants:
 *   - Un Slider associé au feed slider
 *   - Un Toggle associé au feed onoff, en laissant les valeurs par défaut ("ON" et "OFF")
 *   - Un Indicator associé au feed onoff, en indiquant la condition "=ON"
 *   - un Line Chart associé aux feeds temperature et humidity
 * - Tester le code en le téléversant
 * - Installer l'application IFTTT sur votre téléphone mobile
 *   - Tester différentes actions possibles avec Adafruit IO
 *     - If this : Buton widget / Button press
 *     - Then that : Adafruit / Send data to Adafruit IO. Sélectionner votre feed et renseigner
 *       la valeur que vous voulez transmettre (ON par exemple)
 *       A noter que vous pouvez ajouter des "ingrédients" qui sont d'autres données qui sont à
 *       votre disposition (du téléphone ou depuis le service appelé en "If this".
 * 
 * Documentation sur l'API MQTT : https://learn.adafruit.com/adafruit-io/mqtt-api
 * Voici un exemple de clients qui utilisent l'API MQTT : https://learn.adafruit.com/desktop-mqtt-client-for-adafruit-io/overview
 * Vous pouvez aussi utiliser la REST API pour intéragir avec la plateforme Adafruit IO.
 * 
 * Pour mieux comprendre le protocole MQTT, je vous invite à lire la page \ref mqtt
 * 
 * <H2>Bibliothèque à installer</H2>
 * Pour utiliser l'API MQTT d'Adafruit, installer :
 * - Adafruit MQTT library by Adafruit : https://github.com/adafruit/Adafruit_MQTT_Library
 * 
 * Fichier \ref MyAdafruitIO.h
 */

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Configuration *************************************/
// Connexion Adafruit
#define IO_SERVER         "io.adafruit.com"
#define IO_SERVERPORT     1883
#define IO_USERNAME       "maximebui"
#define IO_KEY            "0d12aa10bfca4ed5af13bb71f1213005"
// Feeds
#define FEED_ES_MOI       "/feeds/maxime.etat-de-sante"
#define FEED_ES_CONTACT   "/feeds/contact.etat-de-sante"

/************************** Variables ****************************************/
// Instanciation du client WiFi qui servira à se connecter au broker Adafruit
WiFiClient client;
// Instanciation du client Adafruit avec les informations de connexion
Adafruit_MQTT_Client MyAdafruitMqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_USERNAME, IO_KEY);

/****************************** Feeds ****************************************/
// Création des Feed auxquels nous allons souscrire :
Adafruit_MQTT_Subscribe subEsMoi = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_ES_MOI, MQTT_QOS_1);
Adafruit_MQTT_Subscribe subEsContact = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_ES_CONTACT, MQTT_QOS_1);
Adafruit_MQTT_Publish pubEsMoi = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_ES_MOI);

void EsMoiCallback(char *data, uint16_t len) {
  MYDEBUG_PRINT("-AdafruitIO : Callback sur mon état de santé : ");
  MYDEBUG_PRINTLN(data);
  if (!strcmp(data, "MALADE")){
    MYDEBUG_PRINTLN("-AdafruitIO : Je suis malade");
  } else if (!strcmp(data, "CAS CONTACT")){
    MYDEBUG_PRINTLN("-AdafruitIO : Je suis cas contact");    
  } else { 
    MYDEBUG_PRINTLN("-AdafruitIO : Je suis en pleine forme");
  }
}

void EsContactCallback(char *data, uint16_t len) {
  MYDEBUG_PRINT("-AdafruitIO : Callback sur l'état de santé d'un contact !");
  MYDEBUG_PRINTLN(data);
  if (!strcmp(data, "MALADE")){
    MYDEBUG_PRINTLN("-AdafruitIO : Mon contact est malade");
    pubEsMoi.publish("CAS CONTACT");
  } else if (!strcmp(data, "CAS CONTACT")){
    MYDEBUG_PRINTLN("-AdafruitIO : Mon contact est cas contact"); 
    MYDEBUG_PRINTLN("-AdafruitIO : CA NE CHANGE RIEN POUR MOI");
  } else { 
    MYDEBUG_PRINTLN("-AdafruitIO : Mon contact est en pleine forme");
  }
}

void setupAdafruitIO() {
  if (WiFi.status() != WL_CONNECTED){setupWiFi();}
  subEsMoi.setCallback(EsMoiCallback);
  subEsContact.setCallback(EsContactCallback);
  MyAdafruitMqtt.subscribe(&subEsMoi);
  MyAdafruitMqtt.subscribe(&subEsContact);
}

void connectAdafruitIO() {
  if (MyAdafruitMqtt.connected()) { return; }                      // Si déjà connecté, alors c'est tout bon

  MYDEBUG_PRINT("-AdafruitIO : Connexion au broker ... ");
  int8_t ret;
  while ((ret = MyAdafruitMqtt.connect()) != 0) {                  // Retourne 0 si déjà connecté
     MYDEBUG_PRINT("[ERREUR : ");
     MYDEBUG_PRINT(MyAdafruitMqtt.connectErrorString(ret));
     MYDEBUG_PRINT("] nouvelle tentative dans 10 secondes ...");
     MyAdafruitMqtt.disconnect();                                  // Deconnexion pour être propre
     delay(10000);                                                 // On attend 10 secondes avant de retenter le coup
  }
  MYDEBUG_PRINTLN("[OK]");
}

void loopAdafruitIO() {
  connectAdafruitIO();
  MyAdafruitMqtt.processPackets(10000);
//  if(! MyAdafruitMqtt.ping()) {
//    MyAdafruitMqtt.disconnect();
//  }
}
