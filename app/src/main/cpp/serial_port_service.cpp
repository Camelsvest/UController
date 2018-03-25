//
// Created by Sean Zhang on 2018/3/5.
//
#include "serial_port_service.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <assert.h>

#include "debug_log.h"

#define SERIAL_PORT_MAX_BUF_SIZE 1024

SERIAL_CONFIG_T HRB_SERIAL_CONFIG = {
    38400, // speed
    8, // databits
    1, // stopbit
    'n', // parity
    0, // flow_ctrl
    "/dev/ttyS2"
};

static int serial_port_open(const char *dev, int *fd)
{
    int handle;
    if (dev == NULL || strlen(dev) <= 0 || fd == NULL)
        return RET_ERR_INVALID_PARAM;

    handle = open(dev,O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(handle < 0 )
    {
        fprintf(stderr, "open serial device %s failed.\n", dev);
        return RET_ERR_OPEN_DEVICE;
    }

    *fd = handle;

    return RET_SUCCEED;
}

static int serial_port_set(int fd, int speed,int databits,int stopbits,int parity,int flow_ctrl)
{
    unsigned int i=0;
    struct termios options;

    int speed_arr[] = {B38400,B115200,B19200,B9600,B4800,B2400,B1200,B600,B300};
    int name_arr[] = {38400,115200,19200,9600,4800,2400,1200,600,300};

    if(fd<0)
        return RET_ERR_INVALID_PARAM;

    if(tcgetattr(fd,&options) != 0)
    {
        fprintf(stderr, "failed to get serial attr.\n");
        return RET_ERR_CONFIG_DEVICE;
    }

    for(i=0; i<sizeof(speed_arr)/sizeof(int); i++)
    {
        if(speed == name_arr[i])
        {
            cfsetispeed(&options,speed_arr[i]);
            cfsetospeed(&options,speed_arr[i]);
            break;
        }
        else
        {
            fprintf(stderr, "invalid bit speed value.\n");
            return RET_ERR_CONFIG_DEVICE;
        }
    }

    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;

    switch(flow_ctrl)
    {
        case 0:
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1:
            options.c_cflag |=  CRTSCTS;
            break;
        case 2:
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
        default:
            fprintf(stderr, "Invalid flow ctrl value.\n");
            return RET_ERR_CONFIG_DEVICE;
    }

    options.c_cflag &= ~CSIZE;

    switch(databits)
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |=CS8;
            break;
        default:
            fprintf(stderr, "Invalid databit value.\n");
            return RET_ERR_CONFIG_DEVICE;
    }

    switch(parity)
    {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD|PARENB);
            options.c_iflag |=~INPCK;
            break;
        case 'e':
        case 'E':
            options.c_cflag |=PARENB;
            options.c_cflag &=PARODD;
            options.c_iflag |=INPCK;
            break;
        case 's':
        case 'S':
            options.c_cflag &= ~PARENB;
            options.c_iflag &=~CSTOPB;
            break;
        default:
            fprintf(stderr, "Invalid parity value.\n");
            return RET_ERR_CONFIG_DEVICE;
    }

    switch(stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr, "Invalid stopbit value.\n");
            return RET_ERR_CONFIG_DEVICE;
    }

    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Important here !s
    options.c_oflag &= ~OPOST;
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_iflag &= ~(ICRNL | INLCR);
    options.c_iflag &= ~(IXON |IXOFF | IXANY);
    options.c_cc[VTIME] = 0;
    options.c_cc[VMIN] = 0;

    tcflush(fd,TCIFLUSH);

    if(tcsetattr(fd,TCSANOW,&options) != 0)
    {
        fprintf(stderr, "Failed to invoke tcsetattr, errno = %d, %s\r\n", errno, strerror(errno));
        return RET_ERR_CONFIG_DEVICE;
    }

    return RET_SUCCEED;;
}

static void serial_port_close(int fd) {
    if (fd > 0)
        close(fd);
}

MutexLock::MutexLock(pthread_mutex_t *mutex)
        : m_pMutex(mutex){
    if (m_pMutex)
        pthread_mutex_lock(m_pMutex);
}

MutexLock::~MutexLock(){
    if (m_pMutex)
        pthread_mutex_unlock(m_pMutex);
}

Thread::Thread()
        : m_iThreadId(0)
        , m_pMutex(NULL)
        , m_bRunning(false)
        {
}

Thread::~Thread() {
    assert(m_iThreadId == 0);
    assert(m_bRunning == false);
    assert(m_pMutex == NULL);
}

bool Thread::start() {

    m_pMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (m_pMutex == NULL)
        return false;

    pthread_mutex_init(m_pMutex, NULL);

    if ( 0 == pthread_create(&m_iThreadId, NULL, threadProc, this) ) {
        LOG_DEBUG("the pthread is created.");
        return true;
    } else {
        pthread_mutex_destroy(m_pMutex);
        free(m_pMutex);
        return false;
    }

}

/*
 *  Attention: You can not invoke fucntion stop from internal, it must be invoked from outside;
 */
void Thread::stop() {
    void *ret = NULL;

    
    if (m_iThreadId != 0)
    {
        pthread_mutex_lock(m_pMutex);
        m_bRunning = false;
        pthread_mutex_unlock(m_pMutex);

        LOG_DEBUG("pthread_join is invoked.\n");
        pthread_join(m_iThreadId, &ret);
        if (ret)
            free(ret);

        m_iThreadId = 0;

        pthread_mutex_destroy(m_pMutex);
        free(m_pMutex);
        m_pMutex = NULL;
    }

}

void* Thread::threadProc(void *args) {
    Thread *pThis;
    bool bContinued;

    ENTER_FUNCTION;

    bContinued = true;
    pThis = (Thread *)args;

    pThis->onThreadStarting();

    pthread_mutex_lock(pThis->m_pMutex);
    while (pThis->m_bRunning) {
        pthread_mutex_unlock(pThis->m_pMutex);

        bContinued = pThis->runOnce();
        pthread_mutex_lock(pThis->m_pMutex);
        if (!bContinued)
            pThis->m_bRunning = false;
    }
    pthread_mutex_unlock(pThis->m_pMutex);


    pThis->onThreadStopping();
    
    EXIT_FUNCTION;

    return NULL;
}

void Thread::onThreadStarting() {
    assert(m_pMutex);
    MutexLock autoLock(m_pMutex);
    m_bRunning = true;
}

void Thread::onThreadStopping() {
    assert(m_pMutex);
    MutexLock autoLock(m_pMutex);
    assert(m_bRunning == false);
}

bool Thread::runOnce() {
    return false;
}

bool Thread::isRunning() {
    assert(m_pMutex);
    MutexLock autoLock(m_pMutex);
    return m_bRunning;
}

SerialPortService::SerialPortService()
        : m_iFd(0)
        , m_pLocalBuf(NULL){

}

SerialPortService::~SerialPortService() {
    uninit();

    if (m_pLocalBuf)
        free(m_pLocalBuf);
}

uint8_t SerialPortService::init(SERIAL_CONFIG_T *cfg) {
    int error;

    ENTER_FUNCTION;

    if (strlen(cfg->dev) <= 0) {
        EXIT_FUNCTION;
        return RET_ERR_INVALID_PARAM;
    }

    m_pMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (m_pMutex == NULL) {
        EXIT_FUNCTION;
        return RET_ERR_AGAIN;
    }

    pthread_mutex_init(m_pMutex, NULL);

    m_pLocalBuf = (char *)malloc(SERIAL_PORT_MAX_BUF_SIZE);
    if (m_pLocalBuf == NULL)
    {
        free(m_pMutex);
        EXIT_FUNCTION;
        return RET_ERR_AGAIN;
    }

    error = serial_port_open(cfg->dev, &m_iFd);
    if (error != RET_SUCCEED)
    {
        free(m_pLocalBuf);
        m_pLocalBuf = NULL;

		EXIT_FUNCTION;
        return error;
    }

    error = serial_port_set(m_iFd, cfg->speed, cfg->databits, cfg->stopbits, cfg->parity, cfg->flow_ctrl);
    if (error != RET_SUCCEED)
    {
        free(m_pLocalBuf);
        m_pLocalBuf = NULL;

        pthread_mutex_destroy(m_pMutex);
        free(m_pMutex);
        m_pMutex = NULL;

	EXIT_FUNCTION;
        return error;
    }


    if (start()) {
	EXIT_FUNCTION;
        return RET_SUCCEED;
    }
    else {
        EXIT_FUNCTION;
        return RET_ERR_ERRNO;
    }
        
}

void SerialPortService::uninit() {
    
    ENTER_FUNCTION;

    if (m_iFd > 0)
    {
        stop();

        serial_port_close(m_iFd);
        m_iFd = 0;

        free(m_pLocalBuf);
        m_pLocalBuf = NULL;

        pthread_mutex_destroy(m_pMutex);
        free(m_pMutex);
        m_pMutex = NULL;

        while (!m_queueTx.empty() ) {
            msg_t *msg = m_queueTx.front();
            free(msg->buf);
            free(msg);

            m_queueTx.pop();
        }

    }

    EXIT_FUNCTION;
}

bool SerialPortService::runOnce() {
    int num;
    fd_set rdset;
    unsigned int read_bytes, write_bytes, total_bytes;
    struct timeval timeout;
    msg_t *msg;
    bool tx_succeed;


    //ENTER_FUNCTION;

    FD_ZERO(&rdset);
    FD_SET(m_iFd, &rdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    num = select(m_iFd+1, &rdset, NULL, NULL, &timeout);
    if (num > 0) {

        LOG_DEBUG("fd(%x) is readable.\r\n", m_iFd);

        read_bytes = read(m_iFd, m_pLocalBuf, SERIAL_PORT_MAX_BUF_SIZE);
        LOG_DEBUG("Read %d bytes:\r\n", read_bytes);
        LOG_DEBUG_DUMP(m_pLocalBuf, read_bytes);
		
        if (read_bytes > 0)
        {
        	LOG_DEBUG("read bytes:%d\r\n",read_bytes);
		   	notifyRxBytes(m_pLocalBuf, read_bytes);
        }    
        else
        {
            LOG_ERROR("select error: %d, %s", errno, strerror(errno));
            EXIT_FUNCTION;
            return false;
        }
    } 

    pthread_mutex_lock(m_pMutex);
    if (!m_queueTx.empty()) { // send only one frame every time

        msg = m_queueTx.front();
        m_queueTx.pop();
        pthread_mutex_unlock(m_pMutex);

        write_bytes = 0;
        total_bytes = 0;
        tx_succeed = true;

        do
        {   
            write_bytes = write(m_iFd, msg->buf + total_bytes, msg->length - total_bytes);
            if (write_bytes > 0)
            {	
            	LOG_DEBUG("send %d bytes to hrb\r\n", write_bytes);
                LOG_DEBUG_DUMP(msg->buf + total_bytes, write_bytes);

                total_bytes += write_bytes;
            }
            else
            {	
            	
                tx_succeed = false;
				
                break;
            }
        }
        while (total_bytes < msg->length);

        // callback
        // please note msg memory isn't released here, so function
        // notifyTxMsg must handle whether to deleted memory
        notifyTxMsg(msg, tx_succeed);
		
	LOG_DEBUG("ckeck while loop.\r\n");
		
        pthread_mutex_lock(m_pMutex);
    }
    pthread_mutex_unlock(m_pMutex);

    //EXIT_FUNCTION;

    return true;
}

void SerialPortService::notifyRxBytes(char *buf, int length) {
    return;
}

uint32_t SerialPortService::pushTxQueue(
        char *buf,
        uint32_t length) {
    msg_t *msg;

    ENTER_FUNCTION;

    msg = (msg_t *)malloc(sizeof(msg_t));
    if (msg == NULL) {
	
        return 0;
    }

    msg->buf = (char *)malloc(length);
    if (msg->buf == NULL)
    {
        free(msg);
        return 0;
    }

    msg->length = length;

    memcpy(msg->buf, buf, length);
	
    LOG_DEBUG("push Tx Queue: ");
    LOG_DEBUG_DUMP(buf, length);
	
    pushTxQueue(msg);

    EXIT_FUNCTION;
    return length;
}

uint32_t SerialPortService::pushTxQueue(msg_t *msg) {
    ENTER_FUNCTION;

    MutexLock autoLock(m_pMutex);
    m_queueTx.push(msg);

    EXIT_FUNCTION;

    return msg->length;
}

void SerialPortService::notifyTxMsg(msg_t *msg, bool succeed) {
    return;
}

