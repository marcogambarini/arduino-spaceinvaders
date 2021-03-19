
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte griglia[12][2];

int casuale=15; //"4/casuale" è la probabilità che esca qualcosa
                //più la variabile è bassa, più il gioco è difficile (perché?)
int val_piu=5;

byte corsia=0; //riga di percorrenza: si parte dall'alto
byte vite=3;
int punteggio=0;

unsigned long tempo;
int turno=500; //durata di un turno in millisecondi

bool prec_su=false;
bool prec_giu=false;

//custom characters:
byte freccia_alta[8] = {
  0b00000,
  0b11110,
  0b00001,
  0b11110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte freccia_bassa[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11110,
  0b00001,
  0b11110,
  0b00000
};

byte lampadina[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b10101,
  0b10101,
  0b01110,
  0b00100,
  0b00100
};

byte piu_alto[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte piu_basso[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b00100,
  0b00000
};

byte meno_alto[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte meno_basso[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b00000,
  0b00000
};

byte vuoto[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  // initialize LCD and set up the number of columns and rows: 
  lcd.createChar(0, freccia_alta);
  lcd.createChar(1, freccia_bassa);
  lcd.createChar(2, lampadina);
  lcd.createChar(3, piu_alto);
  lcd.createChar(4, piu_basso);
  lcd.createChar(5, meno_alto);
  lcd.createChar(6, meno_basso);
  lcd.createChar(7, vuoto);

  lcd.begin(16, 2);
  
  lcd.clear();
  
  while(analogRead(A0)>658 || analogRead(A0)<618){
    lcd.setCursor(0,0);
    lcd.print("Per iniziare");
    lcd.setCursor(0,1);
    lcd.print("premi select");
    }
  
  randomSeed(analogRead(A1));
  
  for (int k=0; k<2; k++){ //costruzione iniziale della griglia
    for (int j=0; j<12; j++) griglia[j][k]=random(casuale*2); //l'inizio è più facile
    }
  Serial.begin(9600);
  
  tempo=millis();

}

void loop() {
  if (vite>0) {
    frecce();
    if (millis()>tempo+turno) {
      punti();
      costruttore();
      stampa();
      tempo=millis();
      }
    }
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GAME OVER");
    lcd.setCursor(0,1);
    lcd.print("Punteggio=");
    lcd.print(punteggio);
    delay(10000);
    }
}

void punti()
{
  switch(corsia){
    case 0:
    if (griglia[0][0]==1) punteggio+=val_piu;
    else if (griglia[0][0]==3) vite--;
    break;
    
    case 1:
    if (griglia[0][0]==2) punteggio+=val_piu;
    else if (griglia[0][0]==4) vite--;
    break;
    
    case 2:
    if (griglia[0][1]==1) punteggio+=val_piu;
    else if (griglia[0][1]==3) vite--;
    break;
    
    case 3:
    if (griglia[0][1]==2) punteggio+=val_piu;
    else if (griglia[0][1]==4) vite--;
    break;
    }
  }

void frecce()
{
  int lett=analogRead(A0);
  Serial.println(corsia);
  if (lett>80 && lett<120) {
    if (prec_su==false && corsia>0) corsia--;
    prec_su=true;
    }
  else prec_su=false;
  if (lett>236 && lett<276) {
    if (prec_giu==false && corsia<3) corsia++;
    prec_giu=true;
    }
  else prec_giu=false;
  switch(corsia){
      case 0:
      lcd.setCursor(0,0);
      lcd.write(byte(0));
      lcd.setCursor(0,1);
      lcd.write(7);
      break;
      
      case 1:
      lcd.setCursor(0,0);
      lcd.write(1);
      lcd.setCursor(0,1);
      lcd.write(7);
      break;
      
      case 2:
      lcd.setCursor(0,1);
      lcd.write(byte(0));
      lcd.setCursor(0,0);
      lcd.write(7);
      break;
      
      case 3:
      lcd.setCursor(0,1);
      lcd.write(1);
      lcd.setCursor(0,0);
      lcd.write(7);
      break;
    }
  }

void costruttore()
{
  for (int k=0; k<2; k++){
    for (int j=0; j<11; j++) griglia[j][k]=griglia[j+1][k];
    } //sposta a sinistra di 1 posto gli elementi fino a 11
  griglia[11][0]=random(casuale); //genera 2 elementi casuali per il posto 12
  griglia[11][1]=random(casuale);
  }
  
void stampa()
{
  for (int k=0; k<2; k++){
    for (int j=0; j<12; j++){
      lcd.setCursor(j+1, k);
        switch(griglia[j][k]){
          case 1:  //+ alto
          lcd.write(3);
          break;
          
          case 2:  //+ basso
          lcd.write(4);
          break;
          
          case 3:  //- alto
          lcd.write(5);
          break;
          
          case 4:  //- basso
          lcd.write(6);
          break;
          
          default:
          lcd.write(7);
          break;
        }
      }
    }
    
    lcd.setCursor(14,0);
    lcd.print(vite);
    lcd.write(2);
    
    lcd.setCursor(13,1);
    lcd.print(punteggio);
  }
  
