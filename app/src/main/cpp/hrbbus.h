//
// Created by Sean Zhang on 2018/3/8.
//

#ifndef UCONTROLLER_HRBBUS_H
#define UCONTROLLER_HRBBUS_H

#include "serial_port_service.h"
#include <list>
#include <jni.h>

#pragma pack(push, 1)

typedef enum {
    NORMAL_LAMP = 0,
    STAIR_LAMP,
    SHUTTER,
    SCENATIO,
    DIMMER,
    BLIND
} node_t;

typedef enum {
    LAMP_OPEN = 0,
    LAMP_CLOSE = 1,
    STAIR_LAMP_OPEN = 1,
    STAIR_LAMP_CLOSE = 0,
    SHUTTER_DOWN = 1,
    SHUTTER_UP = 0,
    SHUTTER_STOP = 1,
    SCENATIO_OPEN = 1,
    SCENATIO_CLOSE = 2,
    DIMMER_LUMI = 25,
    BLIND_UP = 0,
    BLIND_DOWN =1
} node_stat_t;

typedef struct address {
    uint16_t addr1;
    uint16_t addr2;
    uint16_t addr3;
} node_address_t;

typedef struct {
    uint8_t      Head1;   // 0x55
    uint8_t      Head2;   // 0xAA
    uint16_t     MachineID; // 0
    uint16_t     Reserved;
    uint16_t     Command;  //
    uint8_t      Config;
	uint8_t      Length;
	uint16_t     Dest_Address;
    uint16_t     InParam;
    uint16_t     ChkSum;
} cmd_pkt_t;

typedef struct {
    uint8_t     Head1;  // 0x5A
    uint8_t     Head2;  // 0xA5
    uint16_t    MachineID;
    uint16_t    SRC_ADDR;
    uint16_t    Dest_ADDR;
    uint8_t     nlength;
    uint16_t    R_Flag;
    uint8_t     Data[1];
    uint16_t    ChkSum;
} status_pkt_t;

typedef struct {
    uint8_t     Head1;
    uint8_t     Head2;
    uint16_t    MachineID;
    uint16_t    Reserved;
    uint16_t    Result;
    uint16_t    outParam;
    uint16_t    ChkSum;
} result_pkt_t;

typedef struct {
    uint8_t     Head1;
    uint8_t     Head2;
    uint16_t    MachineID;
    uint16_t    cmdResponse;
    uint16_t    ChkSum;
} ack_pkt_t;

#pragma pack(pop)

typedef void (*NOTIFY_STATUS_FUNC_T)(uint16_t status, uint16_t dest_address, void *param);

class HRBBus : public SerialPortService {
public:
    HRBBus();
    virtual ~HRBBus();

    int init();
	int init(NOTIFY_STATUS_FUNC_T func, void *param);
	bool setNotifier(JNIEnv *env, jobject thiz, jobject notifier);

    void uninit();

    uint32_t sendMsg(int obj, uint16_t status, uint8_t addr0, uint8_t addr1, uint8_t addr2);
    uint32_t sendMsg(int obj, uint16_t status, uint16_t dest_address);

protected:
    bool convertToHRBPacket(uint16_t dest_address, uint16_t status, cmd_pkt_t *datapkt);
    void callNotifyFunc(uint16_t status, uint16_t dest_address,  void *param);

    // running in thread
    virtual void notifyRxBytes(char *buf, int length);
    virtual void notifyTxMsg(msg_t *msg, bool succeed);

    int processStatusPacket(char *buf, uint32_t length);
    int processReturnedCmdPacket(char *buf, uint32_t length);

private:
    std::list<msg_t *> m_lstTxVerify;

    char *m_pFragmentBuf;
    uint16_t m_iTotalLength, m_iFragmentBufLength;
    uint16_t m_iCurrPos;

	NOTIFY_STATUS_FUNC_T m_notifyStatusFunc;
	void * m_pNofifyStatusFuncParam;

    JavaVM* m_javaVM;
    jobject m_objNotifyStatus;
    jmethodID m_methodNotifyStatus;
};
#endif //UCONTROLLER_HRBBUS_H
