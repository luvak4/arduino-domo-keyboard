#include <IRremote.h>        // load ir library
#include <VirtualWire.h>     // load virtual-wire library
int  INTERIlocali[4]={0,0,0,0};
byte BYTEradio[8];
byte CIFR[8]={156,33,183,95,230,63,250,215};
const unsigned long mask=0x0000FFFF;
const int led_pin = 13;      // led pin
const int receive_pin = 11;  // radio rx pin
const int transmit_pin = 12; // radio tx pin
const int RECV_PIN = 2;      // ir pin
IRrecv irrecv(RECV_PIN);     // ir initialize library
decode_results results;      // ir variable
const long responseA[]={16753245,16736925,16769565,\
			16720605,16712445,16761405,\
			16769055,16754775,16748655,\
			16738455,16750695,16756815,\
			16724175,16718055,16743045,\
			16716015,16726215,16734885,\
			16728765,16730805,16732845};
#define KEY_OK     3
#define KEY_1     13
#define KEY_2     14
#define KEY_3     15
#define KEY_4     16
#define KEY_5     17
#define KEY_6     18
#define KEY_7     19
#define KEY_8     20
#define KEY_9     21
#define KEY_0     10
#define INDIRIZZO  0
#define DATOa      1
#define DATOb      2
#define DATOc      3

unsigned long tempo=0;
int NumeroComposto=0;

////////////////////////////////
// setup
////////////////////////////////
void setup()
{
  pinMode(led_pin, OUTPUT);    // led set pin
  vw_set_tx_pin(transmit_pin); // radio set tx pin
  //vw_set_rx_pin(receive_pin);  // radio set rx pin
  vw_setup(1000);              // radio speed
  //vw_rx_start();               // radio rx ON
  irrecv.enableIRIn();         // ir rx ON
  //Serial.begin(9600);
}

////////////////////////////////
// loop
////////////////////////////////
void loop(){
  if (irrecv.decode(&results)) {  // got ir signal
    byte key=ir_decode(&results); // usable?


    
    
    if (key>0){
      //
      delay(100);
      tempo=millis();
      //
      switch (key){
      case KEY_OK:
	break;
      case KEY_1:
	NumeroComposto=NumeroComposto*10+1;
	break;
      case KEY_2:
	NumeroComposto=NumeroComposto*10+2;
	break;
      case KEY_3:
	NumeroComposto=NumeroComposto*10+3;
	break;
      case KEY_4:
	NumeroComposto=NumeroComposto*10+4;	
	break;
      case KEY_5:
	NumeroComposto=NumeroComposto*10+5;
	break;
      case KEY_6:
	NumeroComposto=NumeroComposto*10+6;	
	break;
      case KEY_7:
	NumeroComposto=NumeroComposto*10+7;	
	break;
      case KEY_8:
	NumeroComposto=NumeroComposto*10+8;	
	break;
      case KEY_9:
	NumeroComposto=NumeroComposto*10+9;	
	break;
      case KEY_0:
	NumeroComposto=NumeroComposto*10;
	break;
      }



      
      if (key==KEY_OK){
	// premuto carattere di conferma
	//////////////////////////////////
	char msg[MSG_LEN] = {0xAA,0,0,0,0,0,0};  // initialize msg
	char numcomp[2] = {0,0};
	// intero ad array
	numcomp = itoa(NumeroComposto,numcomp,10);
	// mix su msg
	msg[1]=numcomp[0];
	msg[2]=numcomp[1];
	digitalWrite(led_pin,HIGH);
	// tx radio
	vw_send((uint8_t *)msg,MSG_LEN);
	vw_wait_tx();
	digitalWrite(led_pin,LOW);
	//////////////////////////////////
	NumeroComposto=0;
      } else {
	// premuto altro tasto
	if (key > 10){
	  // non preso in considerazione
	} else {
	  // compone il numero intero
	  // dalla sequanza di tasti
	  NumeroComposto=NumeroComposto*10+key-1; // key va da 1-10 per indicare 0-9
	}
      }
    } else {
      // dopo un certo numero di secondi
      // resetta la composizone del numero
      if (tempo > 0){
	if (millis()-tempo > 5000){
	  resetSEQUENZA();
	  tempo=0;
	}
      }
    }
    irrecv.resume();                    // ir-receive resume




    
  }
}

byte ir_decode(decode_results *results){
  long keyLongNumber = results->value;  // get the long number
  //Serial.println(keyLongNumber);
  byte key=0;
  for (byte r=0; r<21 ; r++){           // search in array
    if (responseA[r]==keyLongNumber){   // found?
      key=r+1;                          // key in 1-21 range
      break;
    }
  }
  return key;                           // return value
}

void decodeMessage(){
  // de-cifratura
  byte m=0;
  cipher();
  for (int n=0; n<4;n++){
    INTERIlocali[n]=BYTEradio[m+1];
    INTERIlocali[n]=INTERIlocali[n] << 8;
    INTERIlocali[n]=INTERIlocali[n]+BYTEradio[m];
    m+=2;
  }
}

void encodeMessage(){
  // from struct to byte
  byte m=0;
  for (int n=0; n<4;n++){
    BYTEradio[m]=INTERIlocali[n] & mask;
    INTERIlocali[n]=INTERIlocali[n] >> 8;
    BYTEradio[m+1]=INTERIlocali[n] & mask;
    m+=2;
  }
  cipher();
}

void cipher(){
  for (byte n=0;n<8;n++){
    BYTEradio[n]=BYTEradio[n]^CIFR[n];
  }
}
