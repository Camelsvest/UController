//
// Created by Sean Zhang on 2018/2/28.
//

#ifndef UCONTROLLER_CONTROLLER_H
#define UCONTROLLER_CONTROLLER_H

#include <stdint.h>
#include "hrbbus.h"

class Node {
public:
    Node(node_t type);
    virtual ~Node();

    node_t getNodeType() { return m_NodeType; }

    bool setNodeAddress(uint8_t addr1, uint8_t addr2, uint8_t addr3);
    unsigned short getNodeAddress();

    void setBus(HRBBus *pBus) { m_pBus = pBus; }
    HRBBus* getBus(void) { return m_pBus; }

    bool setDescription(char *pszDescription, int length);
    int getDescription(char *pszDescription, int length);

private:
    node_address_t m_Address;
    node_t  m_NodeType;
    HRBBus *m_pBus;

    char *m_pszDescription;
    int m_iLength;
};

class Lamp : public Node {
public:
    Lamp();
    virtual ~Lamp();

    bool switchOn();
    bool switchOff();

private:
};

class StairLamp : public Node {
public:
    StairLamp();
    virtual ~StairLamp();

    bool switchOn();
};

class LampDimmer : public Node {
public:
    LampDimmer();
    virtual ~LampDimmer();

    bool adjust(unsigned short int percent);
};

class Shutter : public Node {
public:
    Shutter();
    virtual ~Shutter();

    bool up();
    bool down();
    bool stop();
};

class Blind : public Node {
public:
    Blind();
    virtual ~Blind();

    bool up();
    bool down();
};

class Scenario : public Node {
public:
    Scenario();
    virtual ~Scenario();

    bool on();
    bool off();
};

#endif //UCONTROLLER_CONTROLLER_H
