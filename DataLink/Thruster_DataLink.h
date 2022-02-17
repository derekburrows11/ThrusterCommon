#ifndef _Thruster_DataLink_h_
#define _Thruster_DataLink_h_

#include <Vector.h>


//Define Data Link structures
/* id
packet id byte - bits 'ddd sss vv'
sss/ddd - source or destination
0	
1	drive 1
2 	drive 2
3	drives all
4	controller
5	
6	
7	

vv - packet version
0	fast update
1	slow update
2
3
*/

#define DL_ID_TYPE_FAST       0x00
#define DL_ID_TYPE_SLOW       0x01
#define DL_IDbit_SRC_DRIVE1   0x04
#define DL_IDbit_SRC_DRIVE2   0x08
#define DL_ID_SRC_CTRL        0x10
#define DL_IDbit_DEST_DRIVE1  0x20
#define DL_IDbit_DEST_DRIVE2  0x40
#define DL_ID_DEST_CTRL       0x80
#define DL_IDmask_TYPE        0x03
#define DL_IDmask_SRC         0x1c
#define DL_IDmask_DEST        0xe0


struct dataControllerToDrive {
	uint8_t	id;
	uint8_t	packetCount;
	uint8_t	packetsRx;
	int8_t	rssi;			// dB of previous received packet
	uint8_t	status;
	
	int16_t	throttle;		// 0.01 %
	uint8_t	dataID;
	int16_t	dataVar[2];
};
struct dataControllerToDrive_byID {
	int16_t	voltageBattery;		//	0 - 0.01 V - don't really need to send to drive 
								//	1 - 
};

struct dataDriveToController {
	uint8_t	id;
	uint8_t	packetCount;
	uint8_t	packetsRx;
	int8_t	rssi;			// dB of previous received packet
	uint8_t	status;
	
	int16_t	currentMotor;	// 0.01 A
	int16_t	currentBattery;	// 0.01 A	gets 0.01A resolution from Vesc
	int16_t	dutyCycle;		// 0.01 %   0 to 1, 0.0001 resolution (0.01%)
	int16_t	rpm;			// 1 rpm
	uint8_t	dataID;
	int16_t	dataVar[2];
};
enum ENUMdataDriveToController_byID {
	voltageBattery = 0,	//  0 - 0.01 V		gets 0.1V resolution from Vesc
	chargeBattery,		//  1 - 0.01 %
	voltageMCUBatt,		//  2 - 0.01 V
	tempMosfet,			//  3 - 0.01 deg
	tempMotor,			//  4 - 0.01 deg
	tempHeatsink,		//  5 - 0.01 deg
	ampHours,			//  6 - 1 mA.Hr
	ampHoursCharged,	//	7 - 1 mA.Hr
	fault,				//	8 - code
						//	9 - 

	BMEtemperature = 10,//	10 - 0.01 deg
	BMEpressure,		//	11 - 1 mbar
	BMEhumidity,		//	12 - 0.01%
	BMEdewPoint,		//	13 - 0.01 deg

	tempBattery1 = 20,	//  20 - 0.01 deg
	tempBattery2,		//  21 - 0.01 deg
	tempBattery3,		//  22 - 0.01 deg
	tempBattery4,		//  23 - 0.01 deg
};

struct dataDriveToController_byID {
	int16_t		voltageBattery;	//  0 - 0.01 V		gets 0.1V resolution from Vesc
	int16_t		chargeBattery;	//  1 - 0.01 %
	int16_t		voltageMCUBatt;	//  2 - 0.01 V
	int16_t		tempMosfet;		//  3 - 0.01 deg
	int16_t		tempMotor;		//  4 - 0.01 deg
	int16_t		tempHeatsink;	//  5 - 0.01 deg
	int16_t		ampHours;		//  6 - 1 mA.Hr
	int16_t		ampHoursCharged;//	7 - 1 mA.Hr
	int16_t		fault;			//	8 - code
							//	9 - 

	int16_t		BMEtemperature;		//	10 - 0.01 deg
	int16_t		BMEpressure;		//	11 - 1 mbar
	int16_t		BMEhumidity;		//	12 - 0.01%
	int16_t		BMEdewPoint;		//	13 - 0.01 deg

	int16_t		tempBattery1;	//  20 - 0.01 deg
	int16_t		tempBattery2;	//  21 - 0.01 deg
	int16_t		tempBattery3;	//  22 - 0.01 deg
	int16_t		tempBattery4;	//  23 - 0.01 deg
};


struct dataController {
	uint16_t	packetCount;	// for sending
	uint16_t	packetsRx;
	int16_t		rssi;			// dB of received
	uint8_t		status;
	float	throttle;		// %
	float	voltageBattery;	// V
	float	tempMagSensor;			// deg

	// At Transmitting End - Controller
	void SetData(dataControllerToDrive& data) const {
		data.packetCount = packetCount;
		data.status     = status;
		data.packetsRx  = packetsRx;
		data.rssi       = rssi;
		data.throttle   = throttle * 100;

		//increment dataID for transmission
		if ((data.dataID+= 2) > 2)
			data.dataID = 0;
		uint8_t i = 0;
		switch (data.dataID) {
		case 0:
			data.dataVar[i++]	= voltageBattery * 100;
			data.dataVar[i++]	= 0 * 100;
			break;
		case 2:
			data.dataVar[i++]	= 0 * 100;
			data.dataVar[i++]	= 0 * 100;
			break;
		}
	}

	// At Receiving End - Drive
	void GetData(const dataControllerToDrive& data) {
		packetCount = data.packetCount;
		status     = data.status;
		packetsRx  = data.packetsRx;
		rssi       = data.rssi;
		throttle   = data.throttle * 0.01;

		uint8_t i = 0;
		switch (data.dataID) {
		case 0:
			voltageBattery  = data.dataVar[i++] * 0.01;
			break;
		case 2:
			break;
		}
	}
	void SetRxTimedOut() {
		throttle   = 0;
	}
};




struct dataDrive {
	uint16_t	packetCount;	// for sending
	uint16_t	packetsRx;
	int16_t		rssi;			// dB of received
	uint8_t		status;
	uint8_t	dataID;


	float		currentMotor;	// 0.01 A
	float		currentBattery;	// 0.01 A	gets 0.01A resolution from Vesc
	float		dutyCycle;		// 0 to 1, 0.0001 resolution (0.01%)
	float		rpm;			// 1 rpm

	// Sent as variable data by ID
	float		voltageBattery;	//  0 - 0.01 V		gets 0.1V resolution from Vesc
	float		chargeBattery;	//  1 - 0.01 %
	float		voltageMCUBatt;	//  2 - 0.01 V
	float		tempMosfet;		//  3 - 0.01 deg
	float		tempMotor;		//  4 - 0.01 deg
	float		tempHeatsink;	//  5 - 0.01 deg
	int			ampHours;		//  6 - 1 mA.Hr
	int			ampHoursCharged;//	7 - 1 mA.Hr
	int			fault;			//	8 - code

	float	BMEtemperature;		//	10 - 0.01 deg
	float	BMEpressure;		//	11 - 1 mbar
	float	BMEhumidity;		//	12 - 0.01%
	float	BMEdewPoint;		//	13 - 0.01 deg

	float	tempBattery[4];		// deg
	
	
	
	// At Transmitting End - Drive
//	void SetData(dataDriveToController& data) const {
	void SetData(dataDriveToController& data) {
		data.packetCount	 = packetCount;
		data.status          = status;
		data.packetsRx       = packetsRx;
		data.rssi            = rssi;
		data.currentMotor    = currentMotor   * 100;
		data.currentBattery  = currentBattery * 100;
		data.dutyCycle       = dutyCycle      * 10000;
		data.rpm             = rpm;

		//increment dataID for transmission
		if ((dataID+= 2) > 12)
			dataID = 0;
		data.dataID = dataID;
		uint8_t i = 0;
		switch (data.dataID) {
		case ENUMdataDriveToController_byID::voltageBattery:
			data.dataVar[i++]	= voltageBattery * 100;
			data.dataVar[i++]	= chargeBattery  * 100;
			break;
		case ENUMdataDriveToController_byID::voltageMCUBatt:
			data.dataVar[i++]	= voltageMCUBatt * 100;
			data.dataVar[i++]	= tempMosfet     * 100;
			break;
		case ENUMdataDriveToController_byID::tempMotor:
			data.dataVar[i++]	= tempMotor      * 100;
			data.dataVar[i++]	= tempHeatsink   * 100;
			break;
		case ENUMdataDriveToController_byID::ampHours:
			data.dataVar[i++]	= ampHours		  * 1;
			data.dataVar[i++]	= ampHoursCharged * 1;
			break;
		case ENUMdataDriveToController_byID::fault:
			data.dataVar[i++]	= fault;
			data.dataVar[i++]	= 0;
			break;

		case ENUMdataDriveToController_byID::BMEtemperature:
			data.dataVar[i++]	= BMEtemperature 	* 100;
			data.dataVar[i++]	= BMEpressure 		* 1;
			break;
		case ENUMdataDriveToController_byID::BMEhumidity:
			data.dataVar[i++]	= BMEhumidity	* 100;
			data.dataVar[i++]	= BMEdewPoint	* 100;
			break;


		case ENUMdataDriveToController_byID::tempBattery1:
			data.dataVar[i++]	= tempBattery[0] * 100;
			data.dataVar[i++]	= tempBattery[1] * 100;
			break;
		case ENUMdataDriveToController_byID::tempBattery3:
			data.dataVar[i++]	= tempBattery[2] * 100;
			data.dataVar[i++]	= tempBattery[3] * 100;
			break;
		default:
			;
		}
	}
	
	// At Receiving End - Controller
	void GetData(const dataDriveToController& data) {
		packetCount		= data.packetCount;
		status          = data.status;
		packetsRx       = data.packetsRx;
		rssi            = data.rssi;
		currentMotor    = data.currentMotor   * 0.01;
		currentBattery  = data.currentBattery * 0.01;
		dutyCycle       = data.dutyCycle      * 0.0001;
		rpm             = data.rpm;
		dataID			= data.dataID;
		
		uint8_t i = 0;
		switch (data.dataID) {
		case ENUMdataDriveToController_byID::voltageBattery:
			voltageBattery  = data.dataVar[i++] * 0.01;
			chargeBattery   = data.dataVar[i++] * 0.01;
			break;
		case ENUMdataDriveToController_byID::voltageMCUBatt:
			voltageMCUBatt  = data.dataVar[i++] * 0.01;
			tempMosfet      = data.dataVar[i++] * 0.01;
			break;
		case ENUMdataDriveToController_byID::tempMotor:
			tempMotor       = data.dataVar[i++] * 0.01;
			tempHeatsink    = data.dataVar[i++] * 0.01;
			break;
		case ENUMdataDriveToController_byID::ampHours:
			ampHours        = data.dataVar[i++] * 1e-4;
			ampHoursCharged = data.dataVar[i++] * 1e-4;
			break;
		case ENUMdataDriveToController_byID::fault:
			fault           = data.dataVar[i++];
			break;

		case ENUMdataDriveToController_byID::BMEtemperature:
			BMEtemperature	= data.dataVar[i++] * 1e-2;
			BMEpressure 	= data.dataVar[i++] * 1;
			break;
		case ENUMdataDriveToController_byID::BMEhumidity:
			BMEhumidity     = data.dataVar[i++] * 1e-2;
			BMEdewPoint     = data.dataVar[i++] * 1e-2;
			break;

		case ENUMdataDriveToController_byID::tempBattery1:
			tempBattery[0]  = data.dataVar[i++] * 0.01;
			tempBattery[1]  = data.dataVar[i++] * 0.01;
			break;
		case ENUMdataDriveToController_byID::tempBattery3:
			tempBattery[2]  = data.dataVar[i++] * 0.01;
			tempBattery[3]  = data.dataVar[i++] * 0.01;
			break;
		default:
			;
		}
	}
	void SetRxTimedOut() {
		currentMotor    = 0;
		currentBattery  = 0;
		dutyCycle       = 0;
		rpm             = 0;
	}

};



extern struct dataController ctrl;
extern struct dataDrive drive;

extern struct magnoData magData;

#endif	// _Thruster_DataLink_h_
