int
REF_TIME = 10,
TEXT_SIZE = 15,
SERIAL_PORT = 1;

float
LUM_R = 0,
LUM_G = 0,
LUM_B = 0,
DOM_MAX = 0,
DOM_MED = 0;

boolean
POST_COLOR = false;

////Imposta costanti:
/* REF_TIME      tempo di refresh (in millisecondi);
 * TEXT_SIZE     dimensione testo;
 * SERIAL_PORT   selettore porta seriale
 *
 * LUM_R         Luminosità Rosso
 * LUM_G         Luminosità Verde
 * LUM_B         Luminosità Blu
 * DOM_MAX       Aumento valore massimo
 * DOM_MED       Aumento valore medio
 * POST_COLOR    Scegli se applicare le modifiche di colore specifico DOPO quelle di dominanza
 */

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

import java.awt.Robot;
import java.awt.Rectangle;
import java.awt.AWTException;
import processing.serial.*;
////Importa librerie per screenshot e seriale

PImage screenshot;
int L, cr, cg, cb;
long r,g,b;
byte[] data;
Serial port;
//Dichiara le variabili screenshoot, somma di valori e lunghezza dell'array dei pixel(dimensione dello schermo

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  L = displayWidth * displayHeight;
////numero di pixel dello schermo

  //printArray(Serial.list());
  port = new Serial(this, Serial.list()[SERIAL_PORT],9600);
////seleziona porta (dalla console)

  data = new byte [3];
////dichiara "data" arrat di 3 byte

}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void draw() {
  try {
    Robot robot = new Robot();
    screenshot = new PImage(robot.createScreenCapture(new Rectangle(0, 0, displayWidth, displayHeight)));
  } 
  catch (AWTException e) {
  }
////acquisisce uno screenshot

  loadPixels(); 
  screenshot.loadPixels();
  r = 0; g = 0; b = 0;
  for(int i= 0; i<L; i++){
    r += red(screenshot.pixels[i]);
    g += green(screenshot.pixels[i]);
    b += blue(screenshot.pixels[i]);
  }
////somma i valori r,g,b per ogni pixel

  cr=round(r/L);
  cg=round(g/L);
  cb=round(b/L);
////calcola il valore medio(r,g,b) dividendo la somma per il numero di pixel

  if(!POST_COLOR){
    cr+=cr * LUM_R;
    cg+=cg * LUM_G;
    cb+=cb * LUM_B;
  }
////elaborazione per colore singolo

   if(cr>cg){
    if(cr>cb){
      if(cg>cb){
        cr+=cr*DOM_MAX;
        cg+=cg*DOM_MED;
      }else{
        cr+=cr*DOM_MAX;
        cb+=cb*DOM_MED;
      }
    }else{
      cr+=cr*DOM_MED;
      cb+=cb*DOM_MAX;  
    }
  }else{
    if(cg>cb){
      if(cr>cb){
        cr+=cr*DOM_MED;
        cg+=cg*DOM_MAX;
      }else{
        cr+=cr*DOM_MED;
        cg+=cg*DOM_MAX;
      }
    }else{
      cg+=cg*DOM_MED;
      cb+=cb*DOM_MAX;
    }
  }
////elaborazione per colore dominante

  if(POST_COLOR){
    cr+=cr * LUM_R;
    cg+=cg * LUM_G;
    cb+=cb * LUM_B;
  }
////elaborazione per colore singolo

  if(cr>255)cr=255;
  if(cg>255)cg=255;
  if(cb>255)cb=255;
////se i valori superano la soglia massima riportali a 255 come valore massimo

  data[0] = (byte)cr;
  data[1] = (byte)cg;
  data[2] = (byte)cb;
  /*print(port.read()+" ");
  print(port.read()+" ");
  println(port.read());*/
  port.write(data);
////valori RGB nell' array per essere inviati allaporta seriale

  delay(REF_TIME);
////Diminuisce l'uso del processore: aspetta un numero specificato di millisecondi prima di ripetere l'operazione

}
