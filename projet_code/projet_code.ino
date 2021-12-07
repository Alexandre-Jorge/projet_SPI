#include <EEPROM.h>
#include <LiquidCrystal_AIP31068_I2C.h>
//Nombre de pas pour une révolution
const int stepsPerRevolution = 32;

//
LiquidCrystal_AIP31068_I2C lcd(0x3E,16,2);

//Permet de stocker la valeur de la position actuelle du moteur
int currentstep;
int steps = 64;
int pin1 = 8;
int pin2 = 9;
int pin3 = 10;
int pin4 = 11;

int bouton =0;

int aleatoire;
int etat = 0;
int victoire = 0;

int p_execution = 0;

boolean done = false;
//mise en place

void setup() {
  pinMode(0,INPUT);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);

  EEPROM.get(8008,p_execution);
  if(p_execution != 2345) {
    EEPROM.put(8008,2345);
    EEPROM.put(8000,0); 
  }
  EEPROM.get(8000,currentstep);
  Serial.begin(9600); 
  Serial.println(currentstep);
  lcd.init();
  lcd.setCursor(0,0);
}
void loop() {
  EEPROM.put(8000, currentstep);
  currentstep = currentstep % (steps/4);

  if(etat ==0) {
    if(currentstep != 0) {
      step(16-currentstep);
    }
    bouton = digitalRead(0);
    if(bouton == HIGH) {
      Serial.println("BOUTON 1");
      etat=1;
    }
  }
  if(etat ==1) {
    lcd.print("lancer en cour");
    Serial.println("ETAT 1");
    victoire = 0;
    aleatoire = random(16*10,16*20);
    Serial.println(aleatoire);
    step(aleatoire);
    //Appel de la fonction
    aleatoire = aleatoire % 16;
    //Variable gangant ou pas16
    //Condition gagnant
    
    if((aleatoire >= 2 && aleatoire <4) || (aleatoire >= 10 && aleatoire < 12)) {
       victoire = 1;
    }
    etat = 2;
  }
  if(etat ==2) {
    //Condition gangant ou perdant
    if(victoire) {
      gagner();
    }
    else{
      perdu();
    }
    bouton = digitalRead(0);
    if(bouton==HIGH) {
      etat = 0;
      done=false;
      lcd.clear();
    }
    //Affichage des resultats
    //Bouton reset
  }
  
 }

void step(int x){
  
  for(int i=0;i<x;i++){
      delay(100);    
      digitalWrite(pin1,1);
      digitalWrite(pin4,0);
      digitalWrite(pin2,1);
      digitalWrite(pin1,0);
      digitalWrite(pin3,1);
      digitalWrite(pin2,0);
      digitalWrite(pin4,1);
      digitalWrite(pin3,0);
      currentstep = (currentstep + 1) % (steps/4);
    }
  
}

void gagner(){
  if(!done){
    done=true;
    Serial.println("gagner !!");
    digitalWrite(1,HIGH);
    digitalWrite(2,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAGNER !");
    delay(2000);
    digitalWrite(1,LOW);
    digitalWrite(2,LOW);
  }
  
}
void perdu(){
  if(!done){
    done=true;
    Serial.println("perdu...");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("perdu...");
  }
}
