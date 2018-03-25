package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Sean Zhang on 2018/3/5.
 */

public class StairLamp extends Lamp {
    public StairLamp() {
        super(STAIR_LAMP);
    }

    public void finalize() {
        // add your deconstruct function here
        // ...
        super.finalize();
    }

    public boolean switchOn(int seconds) {
        Log.d(LOG_TAG, "Call StairLamp.switchOn function ...");
        m_bus.sendMsg(m_nodeType, STAIR_LAMP_OPEN, m_busAddress);
        return true;
    }

}