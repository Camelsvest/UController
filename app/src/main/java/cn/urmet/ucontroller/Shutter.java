package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Administrator on 2018/3/23.
 */

public class Shutter extends Node {
    public Shutter(){
        super(SHUTTER);
    }

    public Shutter(int nodeType){
        super(nodeType);
    }
    public void finalize(){

        super.finalize();
    }

    public boolean up(){
        Log.d(LOG_TAG, "Call Shutter.up function ...");
        m_bus.sendMsg(m_nodeType, SHUTTER_UP, m_busAddress);
        return true;
    }

    public boolean down(){
        Log.d(LOG_TAG, "Call Shutter.down function ...");
        return m_bus.sendMsg(m_nodeType, SHUTTER_DOWN, m_busAddress);
    }

    public boolean stop(){
        Log.d(LOG_TAG, "Call Shutter.stop function ...");
        return m_bus.sendMsg(m_nodeType, SHUTTER_STOP, m_busAddress);
    }
}
