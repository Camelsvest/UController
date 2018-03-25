#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <linux/input.h>
#include <pthread.h>
#include <queue>
#include "serial_port_service.h"
#include "native.h"
#include "debug_log.h"
#include "hrbbus.h"
#include "controller.h"
using namespace std;

HRBBus *bus;

int ucontroller_HRBBus_init(void)
{
	return bus->init();	
	
}
void ucontroller_HRBBus_uninit(void)
{
	bus->uninit();
}

int ucontroller_HRBBUus_sendMsg(node_t obj, unsigned short int status, unsigned short int dest_address)
{
	return bus->sendMsg(obj, status, dest_address);
}

int main(int argc,char**argv)
{
	int ret = -1;
	char ch = 0;

    	verboslevel = 99;

	bus = new HRBBus();
	
	ret = ucontroller_HRBBus_init();
	
	LOG_DEBUG("ucontroller_HRBBus_init return:%d.\r\n",ret);

	LOG_DEBUG("please select the select correct char:q->QUIT PROGRAM.\r\n");
	LOG_DEBUG("--------------------------------------1->OPEN NORMAL LAMP.\r\n");
	LOG_DEBUG("--------------------------------------2->CLOSE NORMAL LAMP.\r\n");
	LOG_DEBUG("--------------------------------------3->OPEN STAIR LAMP.\r\n");
	LOG_DEBUG("--------------------------------------5->OPEN SCENATIO LAMP.\r\n");
	LOG_DEBUG("--------------------------------------6->CLOSE SCENATIO LAMP.\r\n");
	LOG_DEBUG("--------------------------------------7->OPEN LAMP.\r\n");
	LOG_DEBUG("--------------------------------------8->CLOSE LAMP.\r\n");
	LOG_DEBUG("--------------------------------------9->DIMMER 50 percent.\r\n");	
	LOG_DEBUG("--------------------------------------0->DIMMER 10 percent.\r\n");

	Shutter mShutter;
	mShutter.setBus(bus);
	
	LampDimmer mLampDimmer;
	mLampDimmer.setBus(bus);

	Lamp mLamp;
	mLamp.setBus(bus);

	StairLamp mStairLamp;
	mStairLamp.setBus(bus);

	Scenario mScenario;
	mScenario.setBus(bus);

	while(ch != 'q')
	{

        	LOG_DEBUG("please select the select correct char:q->QUIT PROGRAM.\r\n");
	        LOG_DEBUG("--------------------------------------1->OPEN NORMAL LAMP.\r\n");
	        LOG_DEBUG("--------------------------------------2->CLOSE NORMAL LAMP.\r\n");
	        LOG_DEBUG("--------------------------------------3->OPEN STAIR LAMP.\r\n");
	        LOG_DEBUG("--------------------------------------5->OPEN SCENATIO LAMP.\r\n");
	        LOG_DEBUG("--------------------------------------6->CLOSE SCENATIO LAMP.\r\n");
			LOG_DEBUG("--------------------------------------7->OPEN LAMP.\r\n");
			LOG_DEBUG("--------------------------------------8->CLOSE LAMP.\r\n");			
			LOG_DEBUG("--------------------------------------9->DIMMER 50 percent.\r\n");	
			LOG_DEBUG("--------------------------------------0->DIMMER 10 percent.\r\n");	

 	        ch = getchar();
	
		switch(ch)
		{
			case '1'://80D
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x109); //#1 lamp
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x209); //#2 lamp
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x309); //#3 lamp
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x409); //#4 lamp
				mLamp.setNodeAddress(1,1,4);
				mLamp.switchOn();
				break;
			case '2':
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x109); 
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x209);
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x309);
				//ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x409);
				mLamp.setNodeAddress(1,1,4);
				mLamp.switchOff();
				break;
			case '3':
				//ucontroller_HRBBUus_sendMsg(STAIR_LAMP,LAMP_OPEN,0x609); //#4 lamp
				mStairLamp.setNodeAddress(1,1,6);
				mStairLamp.switchOn();
				break;
			case '5':
				//ucontroller_HRBBUus_sendMsg(SCENATIO,SCENATIO_OPEN,0x509); // #4 lamp
				mScenario.setNodeAddress(1,1,5);
				mScenario.on();
				break;
			case '6':
				//ucontroller_HRBBUus_sendMsg(SCENATIO,SCENATIO_CLOSE,0x509);// #4 lamp
				mScenario.setNodeAddress(1,1,5);
				mScenario.off();
				break;
			case '7'://1073/81, as NORMAL_LAMP
				
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x108); // # L4 lamp
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x308); // # L3 lamp
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x508); // # L2 lamp
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_OPEN,0x708); // # L1 lamp
				break;
			case '8':
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x108);
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x308);
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x508);
				ucontroller_HRBBUus_sendMsg(NORMAL_LAMP,LAMP_CLOSE,0x708);
				break;
			case '9':// as DIMMER
				//ucontroller_HRBBUus_sendMsg(DIMMER,10,0x808);
				mLampDimmer.setNodeAddress(1,0,8);
				mLampDimmer.adjust(0);
				break;
			case '0':
				//ucontroller_HRBBUus_sendMsg(DIMMER,0,0x608);
				mLampDimmer.setNodeAddress(1,0,6);
				mLampDimmer.adjust(0);
				break;
			case 'a': //set shutter 
				//ucontroller_HRBBUus_sendMsg(SHUTTER,SHUTTER_UP,0x110);
				mShutter.setNodeAddress(2,0,1);
				mShutter.up();
				break;
			case 'b':
				//ucontroller_HRBBUus_sendMsg(SHUTTER,SHUTTER_DOWN,0x110);
				mShutter.setNodeAddress(2,0,1);
				mShutter.down();
				break;
			case 'c':
				//ucontroller_HRBBUus_sendMsg(SHUTTER,SHUTTER_STOP,0x210);
				mShutter.setNodeAddress(2,0,2);
				mShutter.stop();
				break;
			default:
				break;
			
			
		}
	}
	
	LOG_DEBUG("ucontroller_HRBBus_uninit!\r\n");
	
	ucontroller_HRBBus_uninit();

	delete bus;
	
	
	return 0;
}
