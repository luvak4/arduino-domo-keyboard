#include <IRremote.h>
#include <VirtualWire.h>

const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 11;
// IR
const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;
const long responseA[]={16753245,16736925,16769565,\
			16720605,16712445,16761405,\
			16769055,16754775,16748655,\
			16738455,16750695,16756815,\
			16724175,16718055,16743045,\
			16716015,16726215,16734885,\
			16728765,16730805,16732845};
// max lenght of my message
const int MSG_LEN = 7;

void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_setup(2000);       // Bits per sec
    vw_rx_start(); // receive ON
    //
    pinMode(led_pin, OUTPUT);
    // IR
    irrecv.enableIRIn(); // Start the receiver
   
}

byte ir_decode(decode_results *results){
  long gg =results->value;
  byte temp=B11111111;
  // find if in array
  for (byte r=0; r<21 ; r++){
    if (responseA[r]==gg){      
      temp=r+1;
    }
  }
  return temp;
}

void loop(){
  ///////////////
  // rilevato IR
  ///////////////
  if (irrecv.decode(&results)) {
    // receive IR command
    byte irCommand=ir_decode(&results);
    if (irCommand<255){
      // IR is correct

	txPulsantePremuto(irCommand);	
   
    }
    //
    irrecv.resume();
  }
}



void txPulsantePremuto(byte nPushButton){
  char msg[MSG_LEN] = {0xAA,0,0,0,0,0,0};
  msg[1]=nPushButton;
  //digitalWrite(led_pin, HIGH);
  vw_send((uint8_t *)msg,MSG_LEN);
  vw_wait_tx(); // Wait until the whole message is gone
}
