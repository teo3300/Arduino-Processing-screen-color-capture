import java.awt.Robot;
import java.awt.Rectangle;
import java.awt.AWTException;
import processing.serial.*;
//Importa librerie per screenshot e seriale
 
PImage screenshot;
int L, cr, cg, cb;
long r,g,b;
byte [] data;
//Dichiara le variabili screenshoot, somma di valori e lunghezza dell'array dei pixel(dimensione dello schermo

//Serial port;
private static final int
SERIAL_PORT = 1,
REF_TIME = 10,
TEXT_SIZE = 15;
private static final float
LUM_R = 0,
LUM_G = 0,
LUM_B = 0,
DOM_MAX = 0.8,
DOM_MED = 0.4;
private static final boolean
POST_COLOR = false;
//Imposta costanti:
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

void setup() {
  L = displayWidth * displayHeight;
//numero di pixel dello schermo

  size(200,100);
//dimensione finestra

 printArray(Serial.list());
  //port = new Serial(this, Serial.list()[SERIAL_PORT],9600);
//seleziona porta (dalla console)
data = new byte[3];
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
//acquisisce uno screenshot

  loadPixels(); 
  screenshot.loadPixels();
  r = 0; g = 0; b = 0;
  for(int i= 0; i<L; i++){
    r += red(screenshot.pixels[i]);
    g += green(screenshot.pixels[i]);
    b += blue(screenshot.pixels[i]);
  }
//somma i valori r,g,b per ogni pixel
  cr=round(r/L);
  cg=round(g/L);
  cb=round(b/L);
//calcola il valore medio(r,g,b) dividendo la somma per il numero di pixel

  if(!POST_COLOR){
    cr+=cr * LUM_R;
    cg+=cg * LUM_G;
    cb+=cb * LUM_B;
  }
//elaborazione per colore singolo

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
//elaborazione per colore dominante
  
  if(POST_COLOR){
    cr+=cr * LUM_R;
    cg+=cg * LUM_G;
    cb+=cb * LUM_B;
  }
//elaborazione per colore singolo

  if(cr>255)cr=255;
  if(cg>255)cg=255;
  if(cb>255)cb=255;
//se i valori superano la soglia massima riportali a 255 come valore massimo

data[0]=(byte)cr;
data[1]=(byte)cg;
data[2]=(byte)cb;
 // port.write(data);
//  print(port.read()+ " ");
  //print(port.read()+ " ");
 // println(port.read());
  
  
  textSize(TEXT_SIZE);
  fill(cr,cg,cb);
  rect(0,0,width/2,height);
  fill(0,0,0);
  text("24-b",5,TEXT_SIZE);
//disegna finestra per il canale a 24-bit, valori massimi (da 0): (255,255,255), bit per colore:(8,8,8)

  byte tmp = (byte)((byte)(map(cr,0,255,0,7) * 8)+(byte)(map(cg,0,255,0,7) * 4)+(byte)map(cb,0,255,0,3));
//comprime i 24-bit r,g,b in un solo byte per il canale a 8-bit, valori massimi (da 0): (7,7,4), bit per colore:(3,3,4)

  cr=round(map(round(map(cr,0,255,0,7)),0,7,0,255));
  cg=round(map(round(map(cg,0,255,0,7)),0,7,0,255));
  cb=round(map(round(map(cb,0,255,0,3)),0,3,0,255));
//mappa i valori del canale ad 8-bit sul canale a 24-bit per visualizzarli a schermo

  fill(cr,cg,cb);
  rect(width/2,0,width/2,height);
  fill(0,0,0);
  text("8-b",width/2+5,TEXT_SIZE);
//disegna finestra per il canale a 8-bit

//al posto di tmp=... -> invia sulla seriale prima dichiarata il colore compresso ad 8-bit

  delay(REF_TIME);
//Diminuisce l'uso del processore: aspetta un numero specificato di millisecondi prima di ripetere l'operazione

}
