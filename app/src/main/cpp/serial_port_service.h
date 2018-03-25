//
// Created by Sean Zhang on 2018/3/5.
//

#ifndef UCONTROLLER_SERIAL_PORT_SERVICE_H
#define UCONTROLLER_SERIAL_PORT_SERVICE_H

#include <stdint.h>
#include <pthread.h>
#include <queue>

typedef struct _SERIAL_CONFIG{
    int speed;
    int databits;
    int stopbits;
    char parity;
    int flow_ctrl;
    char dev[256];
} SERIAL_CONFIG_T;


typedef enum {
    RET_ERR_OPEN_DEVICE,
    RET_ERR_CONFIG_DEVICE,
    RET_ERR_WRITE_DEVICE,
    RET_ERR_INVALID_PARAM,
    RET_ERR_AGAIN,
    RET_ERR_ERRNO,
    RET_ERR_MALLOC_MEM,
    RET_ERR_UNKNOWN,
    RET_SUCCEED = 0
} RET_VALUE_T;

extern SERIAL_CONFIG_T HRB_SERIAL_CONFIG;

class MutexLock {
public:
    MutexLock(pthread_mutex_t *mutex);
    virtual ~MutexLock();

private:
    pthread_mutex_t *m_pMutex;
};

class Thread {
public:
    Thread();
    virtual ~Thread();

    bool start();

    /*
     *  Attention: You can not invoke fucntion stop from internal, it must be invoked from outside;
     */
    void stop();

    bool isRunning();

protected:
    static void * threadProc(void *args);

    virtual void onThreadStarting();
    virtual bool runOnce();
    virtual void onThreadStopping();

private:
    pthread_t        m_iThreadId;
    pthread_mutex_t  *m_pMutex;
    bool            m_bRunning;
};

typedef struct msg {
    char *buf;
    uint32_t length;
} msg_t;

class SerialPortService : public Thread {
public:
    SerialPortService();
    virtual ~SerialPortService();

    uint8_t  init(SERIAL_CONFIG_T *cfg);
    void    uninit();

protected:
    /*
     *  return bytes pushed into QUEUE
     *  0 - error, no bytes is pushed into QUEUE
     */
    uint32_t pushTxQueue(char *buf, uint32_t length);
    uint32_t pushTxQueue(msg_t *msg);

    virtual bool runOnce();

    virtual void notifyRxBytes(char *buf, int length);
    virtual void notifyTxMsg(msg_t *msg, bool succeed);

private:
    int  m_iFd;
    char *m_pLocalBuf;

    pthread_mutex_t     *m_pMutex;
    std::queue<msg_t *> m_queueTx;
};

#endif //UCONTROLLER_SERIAL_PORT_SERVICE_H
