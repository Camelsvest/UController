package cn.urmet.ucontroller;

import android.util.Log;

public class Node {
    protected BusAddress    m_busAddress;
    protected HRBBus        m_bus;
    protected String        m_description;
    protected int           m_nodeType;

    protected static int UNKNOWN_NODE = -1;
    protected static int NORMAL_LAMP = 0;
    protected static int STAIR_LAMP = 1;
    protected static int SHUTTER = 2;
    protected static int SCENARIO = 3;
    protected static int DIMMER = 4;
    protected static int BLIND = 5;

    public static short LAMP_OPEN = 0;
    public static short LAMP_CLOSE = 1;
    public static short STAIR_LAMP_OPEN = 1;
    public static short STAIR_LAMP_CLOSE = 0;
    public static short SHUTTER_DOWN = 1;
    public static short SHUTTER_UP = 0;
    public static short SHUTTER_STOP = 1;
    public static short SCENARIO_OPEN = 1;
    public static short SCENARIO_CLOSE = 2;
    public static short DIMMER_LUMI = 25;
    public static short BLIND_UP = 0;
    public static short BLIND_DOWN = 1;

    protected static String LOG_TAG = "UCONTROLLER";

    public short getDeviceStatus() {
        return deviceStatus;
    }

    public void setDeviceStatus(short deviceStatus) {
        this.deviceStatus = deviceStatus;
    }

    protected short deviceStatus;

    public Node() {
        m_bus = HRBBus.createInstance();
        m_nodeType = UNKNOWN_NODE;
    }

    public Node(int nodeType) {
        m_bus = HRBBus.createInstance();
        m_nodeType = nodeType;
    }

    public Node(int nodeType, BusAddress addr) {
        m_bus = HRBBus.createInstance();
        m_nodeType = nodeType;
        m_busAddress = new BusAddress(addr);
    }

    public boolean setAddress(byte addr0, byte addr1, byte addr2) {
        if (m_busAddress == null)
            m_busAddress = new BusAddress();

        if (addr0 >= 32) {
            Log.e(LOG_TAG,"Illegal value(" + addr0 + ") for addr0");
            return false;
        } else
            m_busAddress.address0 = addr0;

        if (addr1 >= 8) {
            Log.e(LOG_TAG,"Illegal value(" + addr1 + ") for addr1");
        } else
            m_busAddress.address1 = addr1;

        m_busAddress.address2 = addr2;

        return true;
    }

    public void setDescription(String description) {
        m_description = new String();
        m_description = description;
    }

    public void finalize() {
    }

}
