#include <IRremote.h>        // load ir library
#include <VirtualWire.h>     // load virtual-wire library
const int led_pin = 13;      // led pin
//const int receive_pin = 11;  // radio rx pin
const int transmit_pin = 12; // radio tx pin
const int MSG_LEN = 7;       // radio max message lenght
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
const char NUMERI[]={'0','1','2','3','4','5','6','7','8','9'};
const byte KEY_OK = 21;
 char SEQUENZA[]={0,0,0,0};
byte cifra=0;
unsigned long tempo=0;
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
  if (irrecv.decode(&results)) {        // got ir signal
    byte key=ir_decode(&results); // usable?
    if (key>0){
      //
      delay(100);
      tempo=millis();
      //
      if (key==KEY_OK){
	//////////////////////////////////
	char msg[MSG_LEN] = {0xAA,0,0,0,0,0,0};  // initialize msg
	msg[1]=SEQUENZA[0];
	msg[2]=SEQUENZA[1];
	msg[3]=SEQUENZA[2];
	msg[4]=SEQUENZA[3];
	digitalWrite(led_pin,HIGH);
	vw_send((uint8_t *)msg,MSG_LEN);         // send to tx-radio
	vw_wait_tx();
	digitalWrite(led_pin,LOW);
	//////////////////////////////////
	resetSEQUENZA();
      } else {
	if (key > 10){
	  // exit
	} else {
	  // compone il numero
	  SEQUENZA[cifra]=NUMERI[key-1];
	  if (cifra > 3){
	    // massimo 4 cifre
	    resetSEQUENZA();
	  } else {
	    cifra+=1;
	  }
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

void resetSEQUENZA(){
  SEQUENZA[0]=0;
  SEQUENZA[1]=0;
  SEQUENZA[2]=0;
  SEQUENZA[3]=0;
  cifra=0;
}
