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
  
#include "../../LibFolder/RF24Mesh/RF24Mesh.h"  
#include "../../LibFolder/RF24/RF24.h"
#include "../../LibFolder/RF24Network/RF24Network.h"
#include "../../LibFolder/AES/AES.h"
#include "../../MeshClient/Arduino/MeshClient/Mesh.h"
#include <fstream>


DeviceT Device;
DeviceT DeviceAll[255];



RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer=0;
uint32_t AllocateDevice=0;

void SaveDevices(){
	std::ofstream outfile ("devices.txt",std::ofstream::binary | std::ofstream::trunc);
	for(int i=1; i< 255; i++){
		outfile.write( (char*)&DeviceAll[i],sizeof(DeviceT));
	}
	outfile.close();
}

void LoadDevices(){
	while(1){
		std::ifstream infile ("devices.txt",std::ifstream::binary);
		if(!infile){
			SaveDevices();
			printf("Creating Devices File\n");
		}else{
			break;
			printf("Found Devices File\n");
		}
	}
	printf("Reading Devices File\n");
}

bool FindFreeNodeID(int k){
	for(int i=0; i<mesh.addrListTop; i++){
		if(mesh.addrList[i].nodeID==k){
			printf("found\%u",k);
			return false;
			break;
		}
	}
	printf("Not found %u\n",k);
	return true;
}



int main(int argc, char** argv) {
	
	LoadDevices();
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
			
			uint8_t dat[16];
			switch(header.type){
				case 'M': network.read(header,&dat,sizeof(dat)); 
					printf("Received From-->%u Value-->",mesh.getNodeID(header.from_node));
					for(int zi=0;zi<16;zi++){
						printf("%u ",dat[zi]);
					}
					printf("\n");
					break;
				case 0x7D:
					network.read(header,&Device,sizeof(Device));
					if((millis() - AllocateDevice > 10000) | (AllocateDevice == 0)){
						printf("Join Request From-->%u DeviceID-->%u DeviceType-->%u DeviceVersion-->%u RandomID-->%u\n",mesh.getNodeID(header.from_node),Device.NodeID,Device.Type,Device.Ver,Device.RandomID);
						for(int k=1;k<255;k++){
							printf("Finding node %u\n",k);
							if(FindFreeNodeID(k)){
								Device.NodeID=k;
								Device.EncrKey[0]=0;
								srand(millis());
								for(int j=1;j<9;j++){
									Device.EncrKey[j]=rand() % 255; // millis();
									printf("Encrip-->%u -->%u; ",j,Device.EncrKey[j]);
								}
								printf("\n");
								header.to_node=header.from_node;
								header.from_node=0;
								if(network.write(header,&Device,sizeof(Device))){
									AllocateDevice=millis();
									printf("Send Successfull\n");
									DeviceAll[k]=Device;
									SaveDevices();
								}else{
									printf("Retry sending..\n");
								}
								printf("send Node id %u and keys before breakout\n",k);
								//if send is succussfull then update dhcplist with encryption keys;
								break;
							}
						}
					}else{
						printf("Discarding Join Request!!\n");
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

      
      
      
