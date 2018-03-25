#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#include "serial_port_to_canbus.h"

typedef struct {
        unsigned char      Head1;
        unsigned char      Head2;
        unsigned short int MachineID;
        unsigned short int Reserved;
        unsigned short int Command;
        unsigned short int Dest_Address;
        unsigned char      nLength;
        unsigned char      nConfig;
        unsigned short int InParam;
        unsigned short int ChkSum;
}CMDPKT;

typedef struct {
        unsigned char      Head1;
        unsigned char      Head2;
        unsigned short int MachineID;
        unsigned short int SRC_ADDR;
        unsigned short int Dest_ADDR;
        unsigned char  nlength;
        unsigned short int R_Flag;
        unsigned int Data[1];
        unsigned short int ChkSum;
}CMDPKT_Status;

typedef struct {
        unsigned char       Head1;
        unsigned char       Head2;
        unsigned short int MachineID;
        unsigned short int Reserved;
        unsigned short int Result;
        unsigned int outParam;
        unsigned short int ChkSum;
}RESULT;

typedef struct {
        unsigned char      Head1;
        unsigned char      Head2;
        unsigned short int MachineID;
        unsigned short int cmdResponse;
        unsigned short int ChkSum;
}ACKPKT;

int fd = -1;
int config_flag = 0;




int serial_port_read(unsigned char *buf,int count)
{
	int ret = 0;
	int len = 0;

	len = read(fd,buf,count);

	return len;
}

int serial_port_write(const unsigned char *buf,int count)
{
	int ret = 0;
	int len = 0;

	len = write(fd,buf,count);
	
	return len;
}

int serial_port_close(void)
{
	 close(fd);
	 config_flag = 0;
	 return RET_SUCCEED;
}

int uartToHrbDataPkt(unsigned int Dest_Address,unsigned int InParam,CMDPKT *datapkt)
{
	CMDPKT *pp = NULL;
	int i,sum=0;
	unsigned char temp[sizeof(CMDPKT)];

	if(datapkt == NULL)
	{
		printf("valid pointer.\n");
		return -1;
	}

	pp = datapkt;
	
	pp->Head1 = 0x55;
	pp->Head2 = 0xaa;
	pp->MachineID = 0;
	pp->Command = 0x0105;
	pp->nConfig = 0x03;
	pp->Dest_Address = Dest_Address;
	pp->nLength = 1;
	pp->InParam = InParam;
	pp->Reserved = 0;

	memcpy(temp,pp,sizeof(CMDPKT));
	for(i=0;i<14;i++)
	{
		sum += temp[i];
	}

	pp->ChkSum = sum;

	return 0;
		
}

int ctrl_hrb_status(OPT_OBJ_T obj , OPT_STAT_T status , unsigned int Dest_Address)
{
	
	CMDPKT cmdPkt;
   	int len = 0;	
	if(fd < 0)
	{
		return RET_ERR_OPEN_DEVICE;
	}

	if(config_flag < 0)
	{
		return RET_ERR_CONFIG_DEVICE;
	}

	uartToHrbDataPkt(Dest_Address,status,&cmdPkt);
	
	len = serial_port_write((unsigned char *)&cmdPkt,sizeof(CMDPKT));
	if(len == 0)
	{
		return RET_ERR_WRITE_DEVICE;
	}
	
	return RET_SUCCEED;
}

int  start_hrb(SERIAL_CONFIG_T *cfg)
{
	int ret = -1;
	
	ret = serial_port_open(cfg->dev);
	if(ret != RET_SUCCEED)
		return ret;
	ret = serial_port_set(cfg->speed,cfg-> databits,cfg->stopbits,cfg->parity,cfg->flow_ctrl);
	
	return ret;
}

int  stop_hrb(void)
{
	close(fd);
	return RET_SUCCEED;
}
 
