#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>

#define LED 2
#define BUTTON 3


//ciao prova
// CAN RX Variables
long unsigned rxId;
unsigned char len;
byte rxBuf[8];
unsigned int error = 0;
//long unsigned int rxId2;
//unsigned char len2;
//byte rxBuf2[8];
//boolean brake = false;
//boolean TS = false;
//boolean button_pressed = false;
// CAN0 INT and CS
MCP_CAN CAN0(10);   // Set CS to pin 10


void setup() {
  Serial.begin(115200);
  if (CAN0.begin(MCP_STDEXT, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.init_Mask(0,1,0x000FFFFF);                // Init first mask...
  CAN0.init_Filt(0,1,0x000F1901);                // Init first filter...
  CAN0.init_Filt(1,1,0x000F1901);                // Init second filter...

  
  CAN0.init_Mask(1,1,0x000FFFFF);                // Init second mask...
  CAN0.init_Filt(2,1,0x000F1906);                // Init third filter...
  CAN0.init_Filt(3,1,0x000F1906);                // Init fourth filter...
  CAN0.init_Filt(4,1,0x000F1906);                // Init fifth filter...
  CAN0.init_Filt(5,1,0x000F1906);                // Init sixth filter...


  // Since we do not set NORMAL mode, we are in loopback mode by default.
  CAN0.setMode(MCP_NORMAL);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
do{
    if (digitalRead(BUTTON) == 0){
      Serial.println("Button Pressed");
      CAN0.readMsgBuf(&rxId, &len, rxBuf);
      if((rxId & 0xF1901) == 0xF1901){
        error = 1;
        }
      else if ((rxId & 0xF1906) == 0xF1906){
       //Turn on LED   
        digitalWrite(LED, HIGH);
        delay(2000);
        //Turn off LED   
        digitalWrite(LED, LOW);
      }
    }
  } while (!error);
}




    
  /*}
  //Get a message
  CAN0.readMsgBuf(&rxId, &len, rxBuf);
  

   if((rxId & 0xF1906) == 0xF1906){
     if(rxBuf[0]!=0){
       brake = true;
       Serial.println("Brake pressed");
     }
   }
   
  /*
   * TS CHECK
   
   else if((rxId & 0xF1901) == 0xF1901){
    if(rxBuf[0]!=0){
      Serial.println("TS ACTIVE");
      TS = true;
    }
    else{
      TS = false;
    }
   }

   //Overall check
   if(TS && brake && button_pressed){
     //Turn on LED   
      digitalWrite(LED, HIGH);
      delay(2000);
      //Turn off LED   
      digitalWrite(LED, LOW);

      //RESET
      TS = false;
      brake = false;
      button_pressed = false;
      rxId = 0;
   }

}*/
