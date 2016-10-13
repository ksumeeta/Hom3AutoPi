struct DeviceT {
	uint8_t NodeID;
	uint8_t Type;
	uint8_t Ver;
	uint8_t Channel;
	uint8_t PaPower;
	uint8_t EncrKey[9];
	uint8_t RandomID;
};