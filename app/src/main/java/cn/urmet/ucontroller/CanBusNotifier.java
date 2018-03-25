package cn.urmet.ucontroller;

/**
 * Created by shiyo on 2018/3/24.
 */

public interface CanBusNotifier {
    public void onRxMsg(int node, short status, byte addr0, byte addr1, byte addr2);
}
