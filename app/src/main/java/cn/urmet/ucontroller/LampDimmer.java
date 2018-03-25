package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Administrator on 2018/3/23.
 */

public class LampDimmer extends Node {

    public LampDimmer(){
        super(DIMMER);
    }

    public void  finalize(){

        super.finalize();
    }

    public boolean ajust(short percent){
        Log.d(LOG_TAG, "Call LampDimmer.ajust function ...");
        return m_bus.sendMsg(m_nodeType, percent, m_busAddress);
    }
}
