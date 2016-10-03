 
 
 /** RF24Mesh_Example_Master.ino by TMRh20
  * 
  * Note: This sketch only functions on -Arduino Due-
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  *
  */
  
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>


struct DeviceT{
	uint8_t NodeID;
	uint8_t Type;
	uint8_t Ver;
	uint32_t EncrKey;
};

DeviceT Device;



RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer=0;

int main(int argc, char** argv) {
  
  
  
  
  
  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  // Connect to the mesh
  printf("starting...\n");
  mesh.begin();
  //radio.setPALevel(RF24_PA_MAX);  
  radio.printDetails();


while(1)
{
  
  // Call network.update as usual to keep the network updated
  mesh.update();

  // In addition, keep the 'DHCP service' running on the master node so addresses will
  // be assigned to the sensor nodes
  mesh.DHCP();
  
  
  // Check for incoming data from the sensors
  while(network.available()){
//    printf("rcv\n");
    RF24NetworkHeader header;
    network.peek(header);
    
    uint32_t dat=0;
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'M': network.read(header,&dat,sizeof(dat)); 
                printf("Received From-->%u Value-->%u\n",mesh.getNodeID(header.from_node),dat);
                 break;
      case 0x7D:
   		network.read(header,&Device,sizeof(Device)); 
                printf("Join Request From-->%u DeviceID-->%u DeviceType-->%u DeviceVersion-->%u\n",mesh.getNodeID(header.from_node),Device.NodeID,Device.Type,Device.Ver);
                 break;
      
      default:  network.read(header,0,0); 
                printf("Rcv bad Data -->%u Type-->%d\n",mesh.getNodeID(header.from_node),header.type); 
                break;
    }
  }
  if(millis() - displayTimer > 5000){
    displayTimer = millis();
    printf("\n");
    printf("********Assigned Addresses********\n");
    for(int i=0; i<mesh.addrListTop; i++){
       printf("NodeID: ");
       printf("%u",mesh.addrList[i].nodeID);
       printf(" RF24Network Address: ");
       printf("%o",mesh.addrList[i].address);
       printf("\n");
     }
    printf("********Assigned Addresses********\n\n");
  }
  
  }
return 0;
}

      
      
      
