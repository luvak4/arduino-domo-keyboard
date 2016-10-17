#include <IRremote.h> 
#include <VirtualWire.h>
#define indirDISPLAYindirizzo 0
#define indirDISPLAYcolonna   1
#define indirDISPLAYriga      2
#define indirDISPLAYnCaratteri 3
#define indirDISPLAYinizioTesto 4
//
#define VELOCITAstd 500
#define VELOCITAhi  2000
String caratteri;
//
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
#define indirMAESTROdisplay 1236
//
#define BYTEStoTX  8
//
int  INTERIlocali[4]={0,0,0,0};
byte BYTEradio[BYTEStoTX];
byte BYTEradioindirDISPLAY[VW_MAX_MESSAGE_LEN];
byte CIFR[]={223,205,228,240,43,146,241,//
	     87,213,48,235,131,6,81,26,//
	     70,34,74,224,27,111,150,22,//
	     138,239,200,179,222,231,212};
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
unsigned long tempo=0;
int NUMcomp=0;
////////////////////////////////
// setup
////////////////////////////////
void setup()
{
  pinMode(led_pin, OUTPUT);
  vw_set_tx_pin(transmit_pin); // radio set tx pin
  vw_set_rx_pin(receive_pin);  // radio set rx pin
  vw_setup(VELOCITAstd);              // radio speed
  vw_rx_start();               // radio rx ON
  irrecv.enableIRIn();         // ir rx ON
  //Serial.begin(9600);
}
////////////////////////////////
// loop
////////////////////////////////
void loop(){
  testIR();
  if (vw_get_message(BYTEradio, &buflen)){
    decodeMessage();
    switch (INTERIlocali[INDIRIZZO]){
    case indirCANTINA:
      break;
    }
    if (INTERIlocali[INDIRIZZO]==indirCANTINA){
      //Serial.println(INTERIlocali[DATOb]);
      // trasmette un testo al indirDISPLAY
    }
  } 
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
      ///////////////
      // premuto ok
      ///////////////
      stampaNc();
      //
      INTERIlocali[INDIRIZZO]=indirMAESTRO;
      INTERIlocali[DATOa]=NUMcomp;
      INTERIlocali[DATOb]=0;
      INTERIlocali[DATOc]=0;
      //
      encodeMessage();
      //
      digitalWrite(led_pin,HIGH);
      vw_rx_stop();
      vw_send((uint8_t *)BYTEradio,BYTEStoTX);
      vw_wait_tx();
      vw_rx_start();
      digitalWrite(led_pin,LOW);
      //
      NUMcomp=0;
      break;
    case KEY_1: scorriNumero(1);break;
    case KEY_2: scorriNumero(2);break;
    case KEY_3: scorriNumero(3);break;
    case KEY_4: scorriNumero(4);break;
    case KEY_5: scorriNumero(5);break;
    case KEY_6: scorriNumero(6);break;
    case KEY_7: scorriNumero(7);break;
    case KEY_8: scorriNumero(8);break;
    case KEY_9: scorriNumero(9);break;
    case KEY_0: scorriNumero(0);break;
    case KEY_CLEAR:
      NUMcomp=0;
      stampaNc();
      break;
    }
    // ritardo
    delay(100);
    // prende nota del tempo
    tempo=millis();
    // continua a ricevere ir
    irrecv.resume();
  }
  //
  if ((millis()-tempo)>5000){
    tempo=millis();
    NUMcomp=0;
  }
  //
  if (NUMcomp<0){
    NUMcomp=0;
  }
}

void pulisciMsgindirDISPLAY(){
  for (byte n=0;n<20;n++){
    BYTEradioindirDISPLAY[n]=0;
  }
}

void radioindirDISPLAY(byte colonna, byte riga){
  // pulisce bytes
  pulisciMsgindirDISPLAY();
  // indirizzo display
  int g=indirMAESTROdisplay;
  // caricamento int su bytes
  BYTEradioindirDISPLAY[0]=g & mask;
  g=g >> 8;
  BYTEradioindirDISPLAY[1]=g & mask;
  // caricamento riga e colonna su bytes
  BYTEradioindirDISPLAY[2]=colonna;
  BYTEradioindirDISPLAY[3]=riga;
  // caricamento lunghezza testo su bytes
  BYTEradioindirDISPLAY[4]=caratteri.length();
  // la lunghezza deve essere non superiore a 20
  if (BYTEradioindirDISPLAY[4]>20){
    BYTEradioindirDISPLAY[4]=20;
  }
  // caricamento messaggio su bytes
  for (byte n=5;n<BYTEradioindirDISPLAY[4]+5;n++){
    BYTEradioindirDISPLAY[n]=caratteri[n-5];
  }
  // cifratura
  for (byte n=0; n<27;n++){
    BYTEradioindirDISPLAY[n]=BYTEradioindirDISPLAY[n] ^ CIFR[n];
  }
  // tx via radio
  digitalWrite(led_pin,HIGH);
  vw_rx_stop();
  // cambio di velocita
  vw_setup(VELOCITAhi);
  vw_send((uint8_t *)BYTEradioindirDISPLAY,VW_MAX_PAYLOAD);
  vw_wait_tx();
  // ripristino velocita
  vw_setup(VELOCITAstd);
  vw_rx_start();
  digitalWrite(led_pin,LOW);
  // ritardo necessario
  delay(300);
}

void stampaNc(){
  pulisciDisplay(0,3,6);
  //
  caratteri=String(NUMcomp);
  radioindirDISPLAY(0,3);
}

void pulisciDisplay(byte colonna, byte riga, byte numCaratteri){
  caratteri="";
  for (byte n=1; n<numCaratteri;n++){
    caratteri+=" ";
  }
  radioindirDISPLAY(colonna,riga);
}

void scorriNumero(byte aggiungi){
  NUMcomp=NUMcomp*10+aggiungi;
  stampaNc();
}
