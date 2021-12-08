//Librairies =========================================================================================

#include <EEPROM.h>
#include <LiquidCrystal_AIP31068_I2C.h>

//Initialisation des varialbles ======================================================================

//Initialisation de l'écran lcd (adresse i2c, colonnes , lignes)
LiquidCrystal_AIP31068_I2C lcd(0x3E,16,2);

//valeur de la position actuelle du moteur
int currentstep;

//Nombes de pas nécéssaires pour que le moteur fasse un tour complet
int steps = 64;

//Numéros des borches connectés au moteur
int pin1 = 8;
int pin2 = 9;
int pin3 = 10;
int pin4 = 11;

//statut du bouton poussoir
int bouton =0;

//représente les valeurs tirés aléatoirement
int aleatoire;

//état de l'algorithme 
int etat = 0;

//définis si l'utilisateur à remporté la partie
int victoire = 0;

//définis si il s'agit ou non de la première exécution du programme
int p_execution = 0;

//détermine si les conditions en cas de victoire ou de défaite ont étés éxécutés
boolean done = false;

//Initialisation ====================================================================================
void setup()
{
  //définis la broche comme étant une entrée
  pinMode(0,INPUT);

  //définis les broches comme étant des sorties
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  //initialise la communication avec le port série
  Serial.begin(9600);

  //Récupère la valeur contenue à l'adresse 8008 de l'EEPROM
  EEPROM.get(8008,p_execution);

  //Vérifie si il s'agit ou non de la première execution du code
  if(p_execution != 1234) 
  {  
    //Stocke les valeurs 2345 et 0 , respectivement aux adresses 8008 et 8000
    EEPROM.put(8008,1234);
    EEPROM.put(8000,0); 
  }
  
  //Stocke la valeur contenue a l'adresse 8000 dans currentstep  
  EEPROM.get(8000,currentstep);
  Serial.println(currentstep);

  //initialise l'écran LCD et positionne son curseur
  lcd.init();
  lcd.setCursor(0,0);

  //pour generer plus tard des nombres vraiment aléatoire
  randomSeed(generate_seed());
}

//Fonction Loop() ======================================================================================
void loop() 
{

  //Stocke la valeur de currentstep à l'adresse 8000
  EEPROM.update(8000, currentstep);
  
  //convertis la valeur de currentstep à une valeur comprise entre 0 et 16
  currentstep = currentstep % (steps/4);

  //Etat d'initialisation et de réinitialisation de la partie
  if(etat ==0) 
  {
    //condition vérifiant si la roue est dans sa position de départ et replace celle-ci dans le cas contraire 
    if(currentstep != 0)
    {
      step(16-currentstep);
    }

    //récupère la valeur de la broche connectée au bouton
    bouton = digitalRead(0);

    //si on appuie sur le bouton 
    if(bouton == HIGH) 
    {
      Serial.println("BOUTON 1");
      etat=1;
    }
  }

  //Etat de lancement de la roue et détermination du résultat
  if(etat ==1)
  {

    //Affichage sur l'écran et le moniteur
    lcd.print("lancer en cours");
    Serial.println("ETAT 1");
    //reinitialisation du statut de la partie 
    victoire = 0;
    //Nombre de pas compris entre 0 et 16 + 10 à 20 tours complets
    aleatoire = random(16*10,16*20);
    Serial.println(aleatoire);

    //appel de la fonction permettant de réaliser le nombre de pas tiré aléatoirement
    step(aleatoire);

    //conversion du résultat pour une valeur comprise entre 0 et 16 
    aleatoire = aleatoire % 16;

    //condition vérifiant si la roue tombe sur une part gagnante
    if((aleatoire >= 2 && aleatoire <4) || (aleatoire >= 10 && aleatoire < 12))
    {
       victoire = 1;
    }
    etat = 2;
    
  }

  //Etat de fin de partie et affichage des résultats
  if(etat ==2)
  {
    
    //Condition vérifiant si la partie est gagnée
    if(victoire)
    {
      //appel de la fonction en cas de victoire
      gagner();     
    }
    
    else
    {
      //appel de la fonction en cas de défaite 
      perdu();
    }

    //on récupère le statut du bouton
    bouton = digitalRead(0);

    //si on appuie sur le bouton
    if(bouton==HIGH) 
    {
      etat = 0;
      done=false;
      lcd.clear();
    }
  } 
 }

//Fonctions ===================================================================================================

//fonction permettant de faire avancer le moteur du nombre de pas passés en paramètres 
void step(int x)
{
   
  for(int i=0;i<x;i++){
      delay(100); 
      //on active les unes après les autres les broches liés au moteur afin d'avancer d'un pas (4 pas moteur)    
      digitalWrite(pin1,1);
      digitalWrite(pin4,0);
      digitalWrite(pin2,1);
      digitalWrite(pin1,0);
      digitalWrite(pin3,1);
      digitalWrite(pin2,0);
      digitalWrite(pin4,1);
      digitalWrite(pin3,0);
      //on convertis pour avoir une valeur entre 0 et 16
      currentstep = (currentstep + 1) % (steps/4);
    } 
}

//fonction contenant les instructioin a éxecuter en cas de victoire
void gagner()
{
  //si les conditions n'ont pas déja étés exécutés
  if(!done)
  {
    //passe done sur true pour indiquer que les actions on étés exécutés 
    done=true;

    //Affiche le message de victoire sur le moniteur
    Serial.println("gagner !!");

    //alimente les broches liés au buzzer et aux leds
    digitalWrite(1,HIGH);
    digitalWrite(2,HIGH);

    //affiche le message de victoire sur l'écran LCD
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAGNER !");
    
    delay(2000);

    //Arrête d'alimenter les broches liés aux leds et au buzzer
    digitalWrite(1,LOW);
    digitalWrite(2,LOW);
    
  }  
}

//fonction contenant les instructioin a éxecuter en cas de défaite
void perdu()
{
  if(!done)
  {
    //si les conditions n'ont pas déja étés exécutés
    done=true;

    //Affiche le message de victoire sur le moniteur et sur l'écran LCD
    Serial.println("perdu...");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("perdu...");
  }
}

//fonction pour genrer plus tard un nombre vraiment aléatoire
unsigned long generate_seed()
{
  uint32_t seed = 0;
  for (int i=0; i< 32; i++)
  {
    //fait un decalage a gauche de 1 bit et le compare avec un port analogique vide qui donne un nombre aleatoire
    //ainsi on a la variable seed complètement aléatoire et donc plus tard un nombre tiré vraiment aléatoirment
     seed = (seed << 1) | (analogRead(0) & 0x01);
  }
  return seed;
}
