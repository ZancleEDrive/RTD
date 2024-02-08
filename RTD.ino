#include <mcp_can.h>
#include <mcp_can_dfs.h>
#define LED 2
#define BUTTON 3

#include <mcp_can.h>
#include <SPI.h>


// CAN RX Variables
long unsigned rxId;
unsigned char len;
byte rxBuf[8];
long unsigned int rxId2;
unsigned char len2;
byte rxBuf2[8];
boolean brake = false;
boolean TS = false;
boolean button_pressed = false;

// CAN0 INT and CS
MCP_CAN CAN0(10);   // Set CS to pin 10


void setup() {
  Serial.begin(115200);
  if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  // Since we do not set NORMAL mode, we are in loopback mode by default.
  CAN0.setMode(MCP_NORMAL);

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  /*
   * Button Check
   */
  if (digitalRead(BUTTON) == 0){
    button_pressed = true;
    Serial.println("Button Pressed");
  }
  //Get a message
  CAN0.readMsgBuf(&rxId, &len, rxBuf);
  
  /*
   * BRAKE CHECK
   */
   if((rxId & 0xF1906) == 0xF1906){
     if(rxBuf[0]!=0){
       brake = true;
       Serial.println("Brake pressed");
     }
   }
   
  /*
   * TS CHECK
   */
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

}
