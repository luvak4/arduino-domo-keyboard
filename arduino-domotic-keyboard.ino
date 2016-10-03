#include <IRremote.h>        // load ir library
#include <VirtualWire.h>     // load virtual-wire library
const int led_pin = 13;      // led pin
const int receive_pin = 11;  // radio rx pin
const int transmit_pin = 12; // radio tx pin
const int MSG_LEN = 7;       // radio max message lenght
const int RECV_PIN = 2;      // ir pin
IRrecv irrecv(RECV_PIN);     // ir initialize library
decode_results results;      // ir variable
const long responseA[]={16753245,16736925,16769565,\ // codes keys
			16720605,16712445,16761405,\
			16769055,16754775,16748655,\
			16738455,16750695,16756815,\
			16724175,16718055,16743045,\
			16716015,16726215,16734885,\
			16728765,16730805,16732845};
////////////////////////////////
// setup
////////////////////////////////
void setup()
{
  pinMode(led_pin, OUTPUT);    // led set pin
  vw_set_tx_pin(transmit_pin); // radio set tx pin
  vw_set_rx_pin(receive_pin);  // radio set rx pin
  vw_setup(2000);              // radio speed
  vw_rx_start();               // radio rx ON
  irrecv.enableIRIn();         // ir rx ON
}

////////////////////////////////
// loop
////////////////////////////////
void loop(){
  if (irrecv.decode(&results)) {        // got ir signal
    byte irCommand=ir_decode(&results); // usable?
    if (irCommand<255){                 // yes
      txPulsantePremuto(irCommand);	// transmit via radio
    }
    irrecv.resume();                    // ir-receive resume
  }
}

byte ir_decode(decode_results *results){
  long keyLongNumber = results->value;  // get the long number
  byte key=0;
  for (byte r=0; r<21 ; r++){           // search in array
    if (responseA[r]==keyLongNumber){   // found?
      key=r+1;                          // key in 1-21 range
    }
  }
  return key;                           // return value
}

void txPulsantePremuto(byte nPushButton){
  char msg[MSG_LEN] = {0xAA,0,0,0,0,0,0};  // initialize msg
  msg[1]=nPushButton;                      // set byte
  vw_send((uint8_t *)msg,MSG_LEN);         // send to tx-radio
  vw_wait_tx();                            // wait until message is gone
}
