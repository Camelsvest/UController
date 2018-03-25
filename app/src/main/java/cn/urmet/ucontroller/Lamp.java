package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Sean Zhang on 2018/3/5.
 */

public class Lamp extends Node {
    public Lamp() {
        super(NORMAL_LAMP);
    }

    public Lamp(int nodeType){
        super(nodeType);
    }
    public void finalize() {
        // add your deconstruct function here
        // ...
        super.finalize();
    }


    public boolean switchOn() {
        Log.d(LOG_TAG, "Call Lamp.switchOn function ...");
        return m_bus.sendMsg(m_nodeType, LAMP_OPEN, m_busAddress);
    }

    public boolean switchOff() {
        Log.d(LOG_TAG, "Call Lamp.switchOff function ...");
        return m_bus.sendMsg(m_nodeType, LAMP_CLOSE, m_busAddress);
    }

    public boolean isOn() {
        return false;
    }

    public boolean isOff() {
        return false;
    }

}
