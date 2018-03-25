#ifndef _SERIAL_PORT_TO_CANBUS_H_
#define _SERIAL_PORT_TO_CANBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int speed;
	int databits;
	int stopbits;
	int parity;
	int flow_ctrl;
	unsigned char dev[64]; 
}SERIAL_CONFIG_T;

typedef enum {
	NORMAL_LAMP = 0,
	STAIR_LAMP,
	SHUTTER,
	SCENATIO
}OPT_OBJ_T;

typedef enum {
	LAMP_OPEN = 1,
	LAMP_CLOSE = 0,
	SHUTTER_DOWM = 1,
	SHUTTER_UP = 0,
	SCENATIO_OPEN = 1,
	SCENATIO_CLOSE = 2
}OPT_STAT_T;

typedef enum {
	RET_SUCCEED = 0,
	RET_ERR_OPEN_DEVICE,
	RET_ERR_CONFIG_DEVICE,
	RET_ERR_WRITE_DEVICE,
    RET_ERR_INVALID_PARAM
}RET_T;

int start_hrb(SERIAL_CONFIG_T *cfg);

int stop_hrb(void);

int ctrl_hrb_status(OPT_OBJ_T obj, OPT_STAT_T status, unsigned int Dest_Address);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _SERIAL_PORT_TO_CANBUS_H_