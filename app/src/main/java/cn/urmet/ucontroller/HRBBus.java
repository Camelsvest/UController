package cn.urmet.ucontroller;

import android.util.Log;

import static android.content.ContentValues.TAG;

/**
 * Created by Sean Zhang on 2018/3/5.
 */
public final class HRBBus {

    private static HRBBus m_bus;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static HRBBus createInstance() {

        if (m_bus == null) {
            m_bus = new HRBBus();
        }

        return m_bus;
    }

    protected HRBBus() {
        // initial BUS here
        // ......
        init();
    }

    public void finalize() {
        // uninitial BUS here
        uninit();
    }

    public boolean sendMsg(int node, short status, BusAddress addr) {
        Log.d("UCONTROLLER", "sendMsg is invoked ");
        return (sendMsg(node, status, addr.address0, addr.address1, addr.address2) > 0);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void test(short s, int i,  long l, float f, double d, char c, boolean z, byte b, String str, Object obj, int[] array);

    protected native boolean init();
    protected native boolean setNotifier(CanBusNotifier notifier);
    protected native void uninit();
    protected native int sendMsg(int node, short status, byte addr0, byte addr1, byte addr2);
}
