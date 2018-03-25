package cn.urmet.ucontroller;

import android.util.Log;

/**
 * Created by shiyo on 2018/3/24.
 */

public class MsgNotifier implements CanBusNotifier {

    public void onRxMsg(int node, short status, byte addr0, byte addr1, byte addr2) {
        Log.d("UCONTROLLER", "function MsgNotifier.onRxMsg is invoked ");

        return;
    }
}
