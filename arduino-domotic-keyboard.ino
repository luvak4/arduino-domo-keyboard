#include <IRremote.h> 
#include <VirtualWire.h>
#define indirDISPLAYindirizzo 0
#define indirDISPLAYcolonna   1
#define indirDISPLAYriga      2
#define indirDISPLAYnCaratteri 3
#define indirDISPLAYinizioTesto 4
String caratteri;
//#define VW_MAX_MESSAGE_LEN 30
byte BYTEradioindirDISPLAY[VW_MAX_MESSAGE_LEN];
//
/*
#define KEY_OK    16769565
#define KEY_1     16724175
#define KEY_2     16718055
#define KEY_3     16743045
#define KEY_4     16716015
#define KEY_5     16726215
#define KEY_6     16734885
#define KEY_7     16728765
#define KEY_8     16730805
#define KEY_9     16732845
#define KEY_0     16738455
*/
#define KEY_1 -983386969
#define KEY_2 -1519975698
#define KEY_3 1007641363
#define KEY_4 11835754
#define KEY_5 -374420087
#define KEY_6 1316801890
#define KEY_7 -1829136225
#define KEY_8 -1395652082
#define KEY_9 497848941
#define KEY_0 -1975719008
#define KEY_OK -1812574087
#define KEY_CLEAR -477592334
//
#define INDIRIZZO  0
#define DATOa      1
#define DATOb      2
#define DATOc      3
//
#define indirMAESTRO 1234
#define indirCANTINA 1235
#define indirDISPLAY 1236
//
#define BYTEStoTX  8
//
int  INTERIlocali[4]={0,0,0,0};
byte BYTEradio[BYTEStoTX];
byte CIFR[]={223,205,228,240,43,146,241,87,213,48,235,131,6,81,26,70,34,74,224,27,111,150,22,138,239,200,179,222,231,212};
//
const unsigned long mask=0x0000FFFF;
//
const int led_pin        = 13;      
const int receive_pin    = 11;
const int transmit_pin   = 12;
uint8_t buflen = BYTEStoTX;  //for rx
const int RECV_PIN = 2;      // ir pin
IRrecv irrecv(RECV_PIN);     // ir initialize library
decode_results results;      // ir variable
/*
  16753245 16736925 16769565
  16720605 16712445 16761405
  16769055 16754775 16748655
  16738455 16750695 16756815
  16724175 16718055 16743045
  16716015 16726215 16734885
  16728765 16730805 16732845
*/
unsigned long tempo=0;
int NumeroComposto=0;
////////////////////////////////
// setup
////////////////////////////////
void setup()
{
  pinMode(led_pin, OUTPUT);
  vw_set_tx_pin(transmit_pin); // radio set tx pin
  vw_set_rx_pin(receive_pin);  // radio set rx pin
  vw_setup(500);              // radio speed
  vw_rx_start();               // radio rx ON
  irrecv.enableIRIn();         // ir rx ON
  Serial.begin(9600);
}
////////////////////////////////
// loop
////////////////////////////////
void loop(){
/*
      vw_setup(2000);
      //////////////////////
      for (int n=0;n<4;n++){
      caratteri="Ciao Luca Bravissimo";
      radioindirDISPLAY(0,n);
      delay(300);
      caratteri="                    ";
      radioindirDISPLAY(0,n);     
      delay(300);
      }
      
      vw_setup(500);
      */
  testIR();
  //
  if (vw_get_message(BYTEradio, &buflen)){
    decodeMessage();
    if (INTERIlocali[INDIRIZZO]==indirCANTINA){
      //Serial.println(INTERIlocali[DATOb]);
      // trasmette un testo al indirDISPLAY
    }
  } 
  //
  
  
}


long ir_decode(decode_results *results){
  long keyLongNumber = results->value;  // get the long number
  return keyLongNumber;
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

void testIR(){
  if (irrecv.decode(&results)) {
    long key=ir_decode(&results);     
    switch (key){
    case KEY_OK:
      stampaNc();
      INTERIlocali[INDIRIZZO]=indirMAESTRO;
      INTERIlocali[DATOa]=NumeroComposto;
      INTERIlocali[DATOb]=0;
      INTERIlocali[DATOc]=0;
      encodeMessage();
      digitalWrite(led_pin,HIGH);
      vw_rx_stop();
      vw_send((uint8_t *)BYTEradio,BYTEStoTX);
      vw_wait_tx();
      vw_rx_start();
      digitalWrite(led_pin,LOW);
      NumeroComposto=0;
      break;
    case KEY_1:
      NumeroComposto=NumeroComposto*10+1;
      stampaNc();
      break;
    case KEY_2:
      NumeroComposto=NumeroComposto*10+2;
      stampaNc();
      break;
    case KEY_3:
      NumeroComposto=NumeroComposto*10+3;
      stampaNc();
      break;
    case KEY_4:
      NumeroComposto=NumeroComposto*10+4;  
      stampaNc();
      break;
    case KEY_5:
      NumeroComposto=NumeroComposto*10+5;
      stampaNc();
      break;
    case KEY_6:
      NumeroComposto=NumeroComposto*10+6; 
      stampaNc();
      break;
    case KEY_7:
      NumeroComposto=NumeroComposto*10+7; 
      stampaNc();
      break;
    case KEY_8:
      NumeroComposto=NumeroComposto*10+8; 
      stampaNc();
      break;
    case KEY_9:
      NumeroComposto=NumeroComposto*10+9; 
      stampaNc();
      break;
    case KEY_0:
      NumeroComposto=NumeroComposto*10;
      stampaNc();
      break;
    case KEY_CLEAR:
      NumeroComposto=0;
      stampaNc();
      break;
    }
    delay(100);
    tempo=millis();
    irrecv.resume();
  }
  //
  if ((millis()-tempo)>5000){
    tempo=millis();
    NumeroComposto=0;
  }
  //
  if (NumeroComposto<0){
    NumeroComposto=0;
  }
}


void pulisciMsgindirDISPLAY(){
  for (byte n=0;n<20;n++){
    BYTEradioindirDISPLAY[n]=0;
  }
}

void radioindirDISPLAY(byte colonna, byte riga){
  pulisciMsgindirDISPLAY();
  


  int g=indirDISPLAY;
  BYTEradioindirDISPLAY[0]=g & mask;
  g=g >> 8;
  BYTEradioindirDISPLAY[1]=g & mask;
  
  //BYTEradioindirDISPLAY[indirDISPLAYindirizzo]=indirDISPLAY;
  BYTEradioindirDISPLAY[2]=colonna;
  BYTEradioindirDISPLAY[3]=riga;
  //
  BYTEradioindirDISPLAY[4]=caratteri.length();
  //
  if (BYTEradioindirDISPLAY[4]>20){
    BYTEradioindirDISPLAY[4]=20;
  }
  //
  for (byte n=5;n<BYTEradioindirDISPLAY[4]+5;n++){
    BYTEradioindirDISPLAY[n]=caratteri[n-5];
  }
  // cifratura
  for (byte n=0; n<27;n++){
    BYTEradioindirDISPLAY[n]=BYTEradioindirDISPLAY[n] ^ CIFR[n];
  }


/*
     encodeMessage();
      digitalWrite(led_pin,HIGH);
      vw_rx_stop();
      vw_send((uint8_t *)BYTEradio,BYTEStoTX);
      vw_wait_tx();
      vw_rx_start();
      digitalWrite(led_pin,LOW);
*/

  
  // tx via radio
  digitalWrite(led_pin,HIGH);
  vw_rx_stop();
  vw_setup(2000);
  vw_send((uint8_t *)BYTEradioindirDISPLAY,VW_MAX_PAYLOAD);
  vw_wait_tx();
  vw_setup(500);
  vw_rx_start();
  digitalWrite(led_pin,LOW);
}

void stampaNc(){
      caratteri="     ";
      radioindirDISPLAY(0,3);
      delay(300);
      caratteri=String(NumeroComposto);
      radioindirDISPLAY(0,3);
}
