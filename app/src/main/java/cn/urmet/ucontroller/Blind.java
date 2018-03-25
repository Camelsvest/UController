package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by Administrator on 2018/3/23.
 */

public class Blind extends Shutter {
    public Blind(){
        super(BLIND);
    }

    public void finalize(){

        super.finalize();
    }

    public boolean rotate(short angle){
        Log.d(LOG_TAG, "Call Blind.rotate function ...");
        return m_bus.sendMsg(m_nodeType, angle, m_busAddress);
    }
}
