/*
 * CommandLibrary.h
 *
 * Created: 08.11.2014 15:16:20
 *  Author: GeirW
 */ 


#ifndef COMMANDLIBRARY_H_
#define COMMANDLIBRARY_H_

enum Commands { IMU_CMD = 'a', ATT_CMD = 'b', PID_CMD = 'c', RC_CMD = 'd', MOTOR_CMD = 'e', EEPROM_CMD = 'f', STATUS_CMD = 'g', LOOPT_CMD = 'h'  };
	
enum Directions { ReadDirection = 'r', WriteDirection = 'w', Ack = 'a' };

enum rcChannels { ROLL, PITCH, YAW, THROTTLE, AUX1, AUX2, AUX3, AUX4, NUM_RC_CHANS };

extern qint16 rcData[NUM_RC_CHANS];

typedef struct 
{
    qint16  accSmooth[3]; // Filtered
    qint16  gyroData[3]; // Avg from two readings
    qint16  magADC[3]; // After offset and scale
    qint16  gyroADC[3]; // After offset and scale
    qint16  accADC[3]; // After offset and scale
} imu_t;
extern imu_t imu;

typedef struct 
{
    qint16 angle[2];            // absolute angle inclination in multiple of 0.1 degree 180 deg = 1800. [0] = ROLL , [1] = PITCH
    qint16 heading;             // variometer in cm/s
} att_t;
extern att_t att;

typedef struct  
{
	float kP;
	float kI;
	float kD;
} pid;

typedef struct  
{
    pid rollPID;
    pid pitchPID;
    pid yawPID;
    qint16 iMax;
} p_t;
extern p_t p;

typedef struct  
{
    quint16 front;
    quint16 back;
    quint16 left;
    quint16 right;
} motor_t;
extern motor_t motor;

typedef struct  
{
    quint16 roll;
    quint16 pitch;
    quint16 yaw;
    quint16 throttle;
    quint16 aux1;
    quint16 aux2;
    quint16 aux3;
    quint16 aux4;
} rc_t;
extern rc_t rc;

typedef struct  
{
    quint16 min;
    quint16 max;
    quint16 mid;
} rc_cali_t;

typedef struct
{
		rc_cali_t rcCalibrations[NUM_RC_CHANS];
        qint16 accOffset[3];
        qint16 magOffset[3];  // {81,45,206}; // Calibrated values
} settings_t;
extern settings_t settings;

typedef struct  
{
        quint8 accDoCalibrate  :1;
        quint8 rccDoCalibrate :1;
        quint8 magDoCalibrate :1;
        quint8 checksumOK :1;
        quint8 armed :1;
} status_t;
extern status_t status;

typedef struct
{
    quint16 time;
} loopTime_t;
extern loopTime_t loopTime;


#endif /* COMMANDLIBRARY_H_ */
