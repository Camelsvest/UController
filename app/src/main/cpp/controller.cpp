//
// Created by Sean Zhang on 2018/2/28.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <assert.h>
#include "controller.h"
#include "debug_log.h"

Node::Node(node_t type)
        : m_NodeType(type)
        , m_pBus(NULL)
        , m_pszDescription(NULL)
        , m_iLength(0){
    m_Address.addr1 = 0;
    m_Address.addr2 = 0;
    m_Address.addr3 = 0;
}

Node::~Node() {

}

bool Node::setNodeAddress(uint8_t addr1, uint8_t addr2, uint8_t addr3) {
    if (addr1 < 16) //according to HRB_Programer  
        m_Address.addr1 = addr1;
    else
        return false;

    if (addr2 < 8)
        m_Address.addr2 = addr2;
    else
        return false;

    return true;
}

unsigned short Node::getNodeAddress() {
    unsigned short addr = 0;

	addr = (m_Address.addr3 << 8) | (m_Address.addr1 << 3) | m_Address.addr2;

	LOG_DEBUG("node address:0x%x.\r\n",addr);
	
    return addr;
}

bool Node::setDescription(char *pszDescription, int length) {
    if (m_pszDescription) {
        m_iLength = 0;
        free(m_pszDescription);
    }

    m_pszDescription = (char *)malloc(length + 1);
    if (m_pszDescription == NULL)
        return false;

    memset(m_pszDescription, 0, length+1);
    memcpy(m_pszDescription, pszDescription, length);
    m_iLength = length + 1;

    return true;
}

int Node::getDescription(char *pszDescription, int length) {
    if (length < m_iLength)
        return 0;

    memcpy(pszDescription, m_pszDescription, m_iLength);
    return m_iLength;
}

Lamp::Lamp():Node(NORMAL_LAMP) {

}

Lamp::~Lamp() {

}

bool Lamp::switchOn() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), LAMP_OPEN, getNodeAddress()) > 0);
    else
        return false;
}

bool Lamp::switchOff() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), LAMP_CLOSE, getNodeAddress()) > 0);
    else
        return false;
}

StairLamp::StairLamp()
    : Node(STAIR_LAMP){

}

StairLamp::~StairLamp() {

}

bool StairLamp::switchOn() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), STAIR_LAMP_OPEN, getNodeAddress()) > 0);
    else
        return false;
}

LampDimmer::LampDimmer()
    : Node(DIMMER){
}

LampDimmer::~LampDimmer() {

}

bool LampDimmer::adjust(unsigned short int percent) {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), percent, getNodeAddress()) > 0);
    else
        return false;
}

Shutter::Shutter()
    : Node(SHUTTER){

}

Shutter::~Shutter() {

}

bool Shutter::up() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
    {

    	return (pBus->sendMsg(getNodeType(), SHUTTER_UP, getNodeAddress()) > 0);
    }
        
    else
    {
    	LOG_WARN("Bus pointer is invalid.\r\n");
    	return false;
    }
        
}

bool Shutter::down() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), SHUTTER_DOWN, getNodeAddress()) > 0);
    else
	{
		LOG_WARN("Bus poiter is invalid.\r\n");
		return false;
	}
}

bool Shutter::stop() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), SHUTTER_STOP, getNodeAddress()) > 0);
    else
        return false;
}

Blind::Blind()
        : Node (BLIND){

}

Blind::~Blind() {

}

bool Blind::up() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), BLIND_UP, getNodeAddress()) > 0);
    else
        return false;
}

bool Blind::down() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)    
        return (pBus->sendMsg(getNodeType(), BLIND_DOWN, getNodeAddress()) > 0);
    else
        return false;
}

Scenario::Scenario()
    : Node(SCENATIO){

}

Scenario::~Scenario() {

}

bool Scenario::on() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), SCENATIO_OPEN, getNodeAddress()) > 0);
    else
        return false;
}

bool Scenario::off() {
    HRBBus *pBus;

    pBus = getBus();
    if (pBus)
        return (pBus->sendMsg(getNodeType(), SCENATIO_CLOSE, getNodeAddress()) > 0);
    else
        return false;
}