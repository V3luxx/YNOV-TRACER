/**\file IoT-B2-0.ino 
 * \page arduino Fichier principal Arduino
 * 
 * \section fonc Fonctions principales
 * Il se constitue de 2 fonctions 
 *  - setup() qui permet dans un premier temps de configurer la carte. C'est la toute première fonction appelée par la carte à son démarrage 
 *  - loop() qui est ensuite appelée en "boucle" comme son nom l'indique
 * 
 * \section port Port Série
 * Le port série est la connexion avec la carte qui va permettre d'échanger des données avec elle :
 * - Transférer le code de l'ordinateur vers la carte,
 * - Récupérer les données que la carte envoie sur ce port de données.
 * 
 * Il est donc primordial que les vitesses soient les mêmes :
 * - Dans le code lors de l'initialisation du port série,
 * - Dans le moniteur série pour qu'il lise les informations "à la bonne vitesse".
 * 
 * Si les vitesses ne sont pas synchronisées alors le moniteur n'arrivera pas à lire les informations et affichera n'importe quoi.
 * 
 * Arduino utilise la fonction Serial pour échanger les données avec la carte : 
 * https://www.arduino.cc/reference/en/language/functions/communication/serial/<br>
 * Les principales fonctions sont :
 * - Serial.begin(speed) en indiquant la vitesse (baud ou bits par secondes) de communication à utiliser sur le port série
 * - Serial.print(val) pour que la carte envoie un message sur le port série
 * - Serial.println(val) idem, avec un un caractère retour chariot à la fin de val
 * - Serial.end()
 * 
 * \section modules Modules
 * Nous aborderons différents "modules" indépendants qui vous permettront d'appréhender différents fonctionnalités 
 * des objets connectés et de réaliser le projet :
 * - \ref port
 * - \ref debug
 * - \ref wifi
 * - \ref led
 * - \ref dht
 * - \ref tickers
 * - \ref deepsleep
 * - \ref spiffs
 * - \ref ntp
 * - \ref webserver
 * - \ref ota
 * - \ref mqtt
 * - \ref adafruitio
 * - \ref ble
*/

// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// SETUP
// ------------------------------------------------------------------------------------------------
/**
* Cette est appelée au démarrage du programme, avant tout autre fonction.
* Elle n'est exécutée qu'une seule fois après chaque mise sous tension ou réinitialisation (reset).
* Elle est à utiliser pour initialiser les variables, le sens des broches, les librairies utilisées. 
*/
void setup() {
  Serial.begin(230400); // Initialisation de la vitesse de communication du port série
  Serial.println("Ouverture du port série");
}

int i=0;
// ------------------------------------------------------------------------------------------------
// LOOP
// ------------------------------------------------------------------------------------------------
/**
 * Après avoir créé une fonction setup(), qui initialise et fixe les valeurs de démarrage du programme, 
 * la fonction loop () fait exactement ce que son nom suggère et s'exécute en boucle 
 * sans fin, permettant à votre programme de s'exécuter et de répondre.
*/
void loop() {
  Serial.print("[");
  Serial.print(i++);
  Serial.print("]");
  Serial.println("Loop");
  delay (2000);
}
