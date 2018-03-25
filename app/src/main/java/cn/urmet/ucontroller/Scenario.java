package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Administrator on 2018/3/23.
 */

public class Scenario extends Node {
    public Scenario(){
        super(SCENARIO);
    }

    public void finalize(){

        super.finalize();
    }

    public boolean on(){
        Log.d(LOG_TAG, "Scenario.open function ...");
        return m_bus.sendMsg(m_nodeType, SCENARIO_OPEN, m_busAddress);
    }

    public boolean off(){
        Log.d(LOG_TAG, "Call Scenario.off function ...");
        return m_bus.sendMsg(m_nodeType, SCENARIO_CLOSE, m_busAddress);
    }
}
