
/** RF24MeshClient
   This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
   master node.
   The nodes will refresh their network address as soon as a single write fails. This allows the
   nodes to change position in relation to each other and the master node.
*/


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include "Mesh.h"
#include <SPI.h>
#include <EEPROM.h>



/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
   User Configuration: nodeID - A unique identifier for each radio. Allows addressing
   to change dynamically with physical changes to the mesh.

   In this example, configuration takes place below, prior to uploading the sketch to the device
   A unique value from 1-255 must be configured for each node.
   This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.

 **/


/* Testing for read and write of EEPROM EncrKey
uint16_t kk;
uint16_t rr1;

for(kk=16384;kk<32768;kk++){
EEPROM.write(0x03,(kk >> 8) & 0xff);
EEPROM.write(0x04,kk);
rr1 = (EEPROM.read(3) * 256) + (EEPROM.read(4));
Serial.print(kk); Serial.print("-->"); Serial.print((kk >> 8) & 0xff); Serial.print("-"); Serial.print((kk) & 0xff); Serial.print("-->");
Serial.print("-->"); Serial.println(rr1);
}
*/

uint32_t displayTimer = 0;
uint32_t displayTimer1 = 0;
uint8_t RandomID;

DeviceT Device;


void ResetDevice(){
  asm volatile ("  jmp 0");  
} 

void GetDeviceID(){
  Serial.println("Getting Device ID and Joining the Network!!");
  while(1){
    mesh.update();
    if (millis() - displayTimer >= 5000) {
      displayTimer = millis();
      Device.RandomID=5;//random(0,255);
      RandomID = Device.RandomID;
      Serial.print(RandomID);Serial.print("-->");Serial.print(Device.RandomID);Serial.println("");
      if (!mesh.write(&Device, 0x7D ,sizeof(Device))){
        if ( ! mesh.checkConnection() ) {
          Serial.println("Renewing Address");
          mesh.renewAddress();
        } else {
          Serial.println("Send fail, Test OK");
        }
      } else {
        Serial.print("Send OK: "); Serial.println(0x7D);
      }
    }
    
    while (network.available()) {
      RF24NetworkHeader header;
      network.peek(header);
      switch(header.type){
        case 0x7D: 
          network.read(header,&Device,sizeof(Device)); 
          Serial.print(RandomID);Serial.print("-->");Serial.print(Device.RandomID);Serial.println("");
          //if(RandomID == Device.RandomID){
            //**********************************************************
            Serial.print("Received NodeID-->");Serial.println(Device.NodeID); 
            for(int z=0;z<9;z++){
              Serial.print("key-->");Serial.print(Device.EncrKey[z]);Serial.print("; ");
            }
            Serial.println(" ");
            //**********************************************************
            EEPROM.write(0,Device.NodeID);
            EEPROM.write(1,Device.Type);
            EEPROM.write(2,Device.Ver);
            EEPROM.write(3,Device.Channel);
            EEPROM.write(4,Device.PaPower);
            EEPROM.write(5,Device.RandomID);
            for(int i=0;i<9;i++){
              EEPROM.write(5+i,Device.EncrKey[i-1]);
            }
          //}
          ResetDevice();
          break;
        default: 
          network.read(header,0,0); 
          Serial.println(header.type);
          break;
      }
    }
  }
}



void setup() {
  
  Serial.begin(115200);
  displayTimer = millis();
  randomSeed(analogRead(0));
  
  Device.NodeID=EEPROM.read(0);
  Device.NodeID=255;
  
  Device.Type=EEPROM.read(1);
  Device.Type=2;
  
  Device.Ver=EEPROM.read(2);
  Device.Ver=2;
  
  Device.Channel=EEPROM.read(3);
  Device.Channel=2;
  
  Device.PaPower=EEPROM.read(4);
  Device.PaPower=2;
  
  Device.RandomID=EEPROM.read(5);
  Device.RandomID=1;
  
  for(int i=1;i<=9;i++){
    Device.EncrKey[i-1]=EEPROM.read(5+i);
  }
  mesh.setNodeID(Device.NodeID);
  Serial.println(F("Connecting to the mesh..."));
  while(1){
    if (millis() - displayTimer >= 5000){
      displayTimer = millis();
      if(mesh.begin()){
        Serial.println("Found Mesh");
        break;
      }else{
        Serial.println("Mesh not found");
      }
    }
  }
  if(Device.NodeID==255){
    GetDeviceID();
  }
}


void loop() {
  
  // Send to the master node every second
  if (millis() - displayTimer >= 5000) {
    displayTimer = millis();
    displayTimer1++;

    // Send an 'M' type message containing the current millis()
    if (!mesh.write(&displayTimer1, 'M', sizeof(displayTimer1))) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      Serial.print("Send OK: "); Serial.println(displayTimer1);
    }
  }

//  while (network.available()) {
//    RF24NetworkHeader header;
//    
//    network.peek(header);
//    uint32_t dat=0;
//
//    network.read(header,&dat,sizeof(dat)); 
//    Serial.print("Data Reveived From: --> ");
//    Serial.print(header.from_node);
//    Serial.print(" For: --> ");
//    Serial.print(header.to_node);
//    Serial.print(" Value: --> ");
//    Serial.print(dat); 
//    Serial.println("");    
//  }
}






