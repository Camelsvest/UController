//
// Created by Sean Zhang on 2018/3/8.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <endian.h>
#include "hrbbus.h"

#include "debug_log.h"

HRBBus::HRBBus()
        : m_pFragmentBuf(NULL)
        , m_iTotalLength(0)
        , m_iFragmentBufLength(0)
        , m_iCurrPos(0)
        , m_notifyStatusFunc(NULL)
        , m_pNofifyStatusFuncParam(NULL)
        , m_javaVM(NULL)
        , m_objNotifyStatus(NULL)
        , m_methodNotifyStatus(NULL){

}

HRBBus::~HRBBus() {
    uninit();
}

int HRBBus::init() {
    if (m_pFragmentBuf == NULL) {
        m_iCurrPos = 0;
        m_iTotalLength = 2048;
        m_pFragmentBuf = (char *)malloc(m_iTotalLength);
        if (m_pFragmentBuf == NULL)
            return  RET_ERR_MALLOC_MEM;
    }

    return SerialPortService::init(&HRB_SERIAL_CONFIG);
}

int HRBBus::init(NOTIFY_STATUS_FUNC_T func, void *param) {
    m_pNofifyStatusFuncParam = param;
    m_notifyStatusFunc = func;

    return init();
}

void HRBBus::uninit() {
    msg_t *msg;

    SerialPortService::uninit();

    if (m_pFragmentBuf) {
        free(m_pFragmentBuf);
        m_pFragmentBuf = NULL;
        m_iTotalLength = 0;
        m_iCurrPos = 0;
    }

    while (!m_lstTxVerify.empty()) {
        msg = m_lstTxVerify.front();
        free(msg->buf);
        free(msg);

        m_lstTxVerify.pop_front();
    }

    if (m_objNotifyStatus) {
        JNIEnv *env;
        m_javaVM->AttachCurrentThread(&env, NULL);
        env->DeleteGlobalRef(m_objNotifyStatus);
        m_javaVM->DetachCurrentThread();
    }
}

bool HRBBus::convertToHRBPacket(uint16_t dest_address, uint16_t status,
                                cmd_pkt_t *datapkt) {
    uint32_t i;
    char *pointer;

    if(datapkt == NULL)
    {
        fprintf(stderr, "invalid pointer.\n");
        return -1;
    }

    datapkt->Head1 = 0x55;
    datapkt->Head2 = 0xaa;
    datapkt->MachineID = 0;
    datapkt->Command = 0x0105;
    datapkt->Config = 0x03;
    datapkt->Dest_Address = dest_address;
    datapkt->Length = 1;
    datapkt->InParam = status;
    datapkt->Reserved = 0;
    datapkt->ChkSum = 0;

    pointer = (char *)datapkt;

    for(i=0;i<sizeof(cmd_pkt_t) - sizeof(unsigned short);i++)
    {
        datapkt->ChkSum += pointer[i];
    }
    LOG_DEBUG("make the datas to hrb packet");
    return true;
}

#define ADDR0(addr) (addr & 0x001F)
#define ADDR1(addr) ((addr & 0x00E0) >> 5)
#define ADDR2(addr) ((addr & 0xFF00) >> 8)

uint32_t HRBBus::sendMsg(int obj, uint16_t status, uint8_t addr0, uint8_t addr1, uint8_t addr2) {
    uint16_t addr_value;

    if (addr0 > 16) //according to HRB_Programmer
        return 0;

    if (addr1 > 8)
        return 0;

	addr_value = addr2 << 8 | addr0 << 3 | addr1;

    return sendMsg(obj, status, addr_value);
}

uint32_t HRBBus::sendMsg(int obj, uint16_t status, uint16_t dest_address) {
    cmd_pkt_t cmd;

    if (!convertToHRBPacket(dest_address, status, &cmd))
        return RET_ERR_UNKNOWN;

    return pushTxQueue((char *)&cmd, sizeof(cmd));
}

void HRBBus::notifyRxBytes(char *buf, int length) {
    uint16_t magic_header;
    uint16_t available_length, curr_pos;
    uint16_t bytes = 0;
    bool continued = true;

    ENTER_FUNCTION;

    if (length <= 0)
        return;

    if (length > m_iTotalLength) {
        // print error here...
        return;
    }


    memcpy(m_pFragmentBuf + m_iFragmentBufLength, buf, length);
    m_iFragmentBufLength += length;

    if (m_iFragmentBufLength < 2 )
        return;

    curr_pos = 0;
    do {
        available_length = m_iFragmentBufLength - curr_pos;
		if (available_length < 2) // ensure magic_header size
			break;


        // whether we need to change byte sequence here ???
        LOG_DEBUG("m_iFragmentBufLength:%d, curr_pos:%d\r\n",m_iFragmentBufLength, curr_pos);

        magic_header = *(uint16_t *)(m_pFragmentBuf + curr_pos);

		LOG_DEBUG("print Fragment Buf: 0x%02x 0x%02x \r\n",
				*(unsigned char *)m_pFragmentBuf + curr_pos, *(unsigned char *)(m_pFragmentBuf + curr_pos + 1));	
		LOG_DEBUG("magic header is 0x%x.\r\n",magic_header);
		
        switch (magic_header) {
            case 0x5AA5: // 0xa55a status packet,check the first byte
                LOG_DEBUG("the status packet:%l.\r\n",sizeof(status_pkt_t));
                if (available_length >= sizeof(status_pkt_t)) {
                    bytes = processStatusPacket((char *)m_pFragmentBuf + curr_pos, available_length);
                    if (bytes > 0) {
                        curr_pos += bytes;
						LOG_DEBUG("curr_pos += bytes : %d\r\n", curr_pos);
                    } else
                        continued = false;

                } else {
                    // packet length is not enough
                    // wait for reading more bytes;

                    continued = false;
                }
                break;

            case 0xAA55: // 0x55AA cmd packet,check the first byte
                LOG_DEBUG("the cmd packet:%d.\r\n",sizeof(cmd_pkt_t));
                if (available_length >= sizeof(cmd_pkt_t)) {
                    bytes = processReturnedCmdPacket((char *)m_pFragmentBuf + curr_pos, available_length);
                    if (bytes > 0) {
                        curr_pos += bytes;
						LOG_DEBUG("curr_pos += bytes : %d\r\n", curr_pos);
                    } else
                        continued = false;
                } else {
                    // packet length is not enough
                    // wait for reading more bytes;

                    continued = false;
                }
                break;

            default:
                continued = false;			
                break;
        }

		//LOG_DEBUG("check while in notifyRxBytes.\r\n");
    } while (continued);

    memmove(m_pFragmentBuf, m_pFragmentBuf + curr_pos, available_length);
    m_iFragmentBufLength = available_length;

	LOG_DEBUG("Now available_length is %d, m_iFragmentBufLength: %d\r\n", available_length, m_iFragmentBufLength);

    EXIT_FUNCTION;
}

void HRBBus::notifyTxMsg(msg_t *msg, bool succeed) {
	
	if (succeed) {
        m_lstTxVerify.push_back(msg);
    } else {
        // do some re-send operation here...
        // re-push it into queue
    	//�����ط�succeedΪfalse    
        pushTxQueue(msg);
    }
    LOG_DEBUG("notifyTxMsg is invoked.\r\n");
    return;
}

int HRBBus::processReturnedCmdPacket(char *buf, unsigned int length) {
    cmd_pkt_t  *cmd, *sent_cmd;
    std::list<msg_t *>::iterator itera;

    if (length < sizeof(cmd_pkt_t))
        return 0;

    cmd  = (cmd_pkt_t *)(buf);
    for (itera = m_lstTxVerify.begin(); itera != m_lstTxVerify.end(); itera++ ) {
        sent_cmd = (cmd_pkt_t *)((*itera)->buf);
        if ( 0 == memcmp(cmd, sent_cmd, sizeof(cmd_pkt_t)) ) {
            // release msg buffer here
            free((*itera)->buf);
            free(*itera);

            itera = m_lstTxVerify.erase(itera);
            break;
        }
    }

    return sizeof(cmd_pkt_t);
}

int HRBBus::processStatusPacket(char *buf, unsigned int length) {
    status_pkt_t *status;

    if (length < sizeof(status))
        return 0;

    status = (status_pkt_t *)buf;

	LOG_DEBUG("Head1:%02x ",status->Head1);
	LOG_DEBUG("Head2:%02x ",status->Head2);
	LOG_DEBUG("MachineID:%02x ",status->MachineID);
	LOG_DEBUG("SRC_ADDR:%02x ",status->SRC_ADDR);
	LOG_DEBUG("Dest_ADDR:%02x ",status->Dest_ADDR);
	LOG_DEBUG("nlength:%02x ",status->nlength);
	LOG_DEBUG("R_Flag:%02x ",status->R_Flag);
	LOG_DEBUG("Data :%02x ",status->Data[0]);
	LOG_DEBUG("ChkSum :%02x ",status->ChkSum);

	LOG_DEBUG("\r\n");
		
    // send status notify to upper layer
    callNotifyFunc(status->Data[0], status->Dest_ADDR, m_pNofifyStatusFuncParam);

    return sizeof(status_pkt_t);
}

void HRBBus::callNotifyFunc(uint16_t status, uint16_t dest_address,  void *param) {

    ENTER_FUNCTION;

    LOG_INFO("\r\n status packet Dest_ADDR:%d, Data:%d.\r\n",dest_address, status);

    if (m_notifyStatusFunc){
        m_notifyStatusFunc(status, dest_address, param);
    } else if (m_javaVM && m_objNotifyStatus && m_methodNotifyStatus) {
        JNIEnv *env = NULL;

        m_javaVM->AttachCurrentThread(&env, NULL);
        if (env != NULL) {
            env->CallVoidMethod(m_objNotifyStatus, m_methodNotifyStatus, 0, status,
                                   ADDR0(dest_address), ADDR1(dest_address), ADDR2(dest_address));
            m_javaVM->DetachCurrentThread();
        }
    } else {
        LOG_WARN("NOTIFY_STATUS_FUNC or m_methodNotifyStatus isn't set, cannot notify upper \
			layer as receiving a status packet (status = %02x, dest_addr = %02x).\r\n",
                 status, dest_address);
    }

    EXIT_FUNCTION;
}

bool HRBBus::setNotifier(JNIEnv *env, jobject thiz, jobject notifier) {

    jclass clazz;

    ENTER_FUNCTION;

    env->GetJavaVM(&m_javaVM);
    m_objNotifyStatus = env->NewGlobalRef(notifier);

    clazz = env->GetObjectClass(notifier);
    m_methodNotifyStatus = env->GetMethodID(clazz, "onRxMsg", "(ISBBB)V");
    if (m_methodNotifyStatus == NULL) {
        LOG_ERROR("Cannot find method onRxMsg\r\n");

        // below is important
        env->DeleteLocalRef(clazz);
        env->DeleteGlobalRef(m_objNotifyStatus);
        m_objNotifyStatus = NULL;
        return false;
    }

    env->DeleteLocalRef(clazz); // important
    EXIT_FUNCTION;
    return true;
}
