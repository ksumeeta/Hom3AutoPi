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
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "AES/AES.h"


struct DeviceT{
	uint8_t NodeID;
	uint8_t Type;
	uint8_t Ver;
	uint8_t EncrKey[9];
	uint8_t RandomID;
};

DeviceT Device;



RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer=0;

bool FindFreeNodeID(int k){
	for(int i=0; i<mesh.addrListTop; i++){
		if(mesh.addrList[i].nodeID!=i+1){
			printf("%u\n",i+1);
			break;
		}else if(mesh.addrListTop==i+1){
			printf("%u\n",i+1);
			break;
		}
	}
}



int main(int argc, char** argv) {
	mesh.setNodeID(0);
	printf("starting...\n");
	mesh.begin();
	radio.setPALevel(RF24_PA_MAX);  
	radio.printDetails();

	while(1){
		mesh.update();
		mesh.DHCP();
		while(network.available()){
			RF24NetworkHeader header;
			network.peek(header);
			
			uint32_t dat=0;
			switch(header.type){
				case 'M': network.read(header,&dat,sizeof(dat)); 
					printf("Received From-->%u Value-->%u\n",mesh.getNodeID(header.from_node),dat);
					break;
				case 0x7D:
					network.read(header,&Device,sizeof(Device)); 
					printf("Join Request From-->%u DeviceID-->%u DeviceType-->%u DeviceVersion-->%u RandomID-->%u\n",mesh.getNodeID(header.from_node),Device.NodeID,Device.Type,Device.Ver,Device.RandomID);
					for(int i=0; i<mesh.addrListTop; i++){
						if(mesh.addrList[i].nodeID!=i+1){
							printf("%u\n",i+1);
							break;
						}else if(mesh.addrListTop==i+1){
							printf("%u\n",i+1);
							break;
						}
					}
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

      
      
      
