
//for hall

#include<SPI.h>
#include<RF24.h>
#define RELAY1  7 

// ce, csn pins
RF24 radio(9, 10) ;

//data_map = {'fan1': '1', 'light1': '2', 'fan2': '3', 'light2': '4', 'fan3': '5', 'light3': '6'}
int fan1 = 0;
int light1 = 0;
const uint64_t pipe = 0xF0F0F0F0E1LL ;

void setup(void){
  //while (!Serial) ;
  Serial.begin(9600) ;
  radio.begin() ;
  radio.setPALevel(RF24_PA_MAX) ;
  radio.setChannel(0x76) ;
  //radio.openWritingPipe(pipe);  /*0xF0F0F0F0E1LL);*/
  radio.openReadingPipe(1, pipe) ;
  radio.enableDynamicPayloads() ;
  radio.powerUp() ;
  pinMode(RELAY1, OUTPUT);   
}

String get_state(char app_id) {
  if(app_id == '1' || app_id == '3' || app_id == '5')
    return String(fan1);
  if(app_id == '2' || app_id == '4' || app_id == '6')
    return String(light1);
  return String(0);
}

void set_state(char app_id, int state) {
  if(app_id == '1') {
    fan1 = state;
    if(state == 1)
      digitalWrite(RELAY1,LOW);
    if(state == 0)
      digitalWrite(RELAY1,HIGH);
  }
  if(app_id == '2') {
    light1 = state;
    if(state == 1)
      digitalWrite(RELAY1,LOW);
    if(state == 0)
      digitalWrite(RELAY1,HIGH);
  }
}

void loop(void){
  radio.startListening() ;
  //Serial.println("Starting loop. Radio on.") ;
  char receivedMessage[32] = {0};
  Serial.println(receivedMessage) ;
  if (radio.available()){
    radio.read(receivedMessage, sizeof(receivedMessage));
    //radio.flush_rx();
    Serial.println(radio.getDynamicPayloadSize()) ;
    Serial.println(receivedMessage) ;
    Serial.println("Turning off the radio.") ;
    radio.stopListening() ;

    //delay(1000);
    //String stringMessage(receivedMessage) ;

    if (receivedMessage[0] == '1'){
      String state = get_state(receivedMessage[1]);
      Serial.println("Hi");
      radio.openWritingPipe(pipe);
      radio.write(state.c_str(), state.length()) ;
      Serial.println("Returned the state of ") ;
      if(receivedMessage[1] == '1')
        Serial.println("fan1") ;
      if(receivedMessage[1] == '2')
        Serial.println("light1") ;
    }
    else if(receivedMessage[0] == '0'){
      set_state(receivedMessage[1], int(receivedMessage[2]));
      radio.openWritingPipe(pipe);
      char *p = "0";
      radio.write(p, strlen(p)) ;
      Serial.println("Changed the state of ") ;
      if(receivedMessage[1] == '1')
        Serial.println("fan1") ;
      if(receivedMessage[1] == '2')
        Serial.println("light1") ;
    } 
  }
}
