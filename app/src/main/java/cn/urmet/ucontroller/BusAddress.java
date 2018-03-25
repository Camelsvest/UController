package cn.urmet.ucontroller;

/**
 * Created by Sean Zhang on 2018/3/5.
 */

public class BusAddress {

    public BusAddress() {
        address0 = 0;
        address1 = 0;
        address2 = 0;
    }

    public BusAddress(byte addr0, byte addr1, byte addr2) {
        address0 = addr0;
        address1 = addr1;
        address2 = addr2;
    }

    public BusAddress(BusAddress addr) {
        address0 = addr.address0;
        address1 = addr.address1;
        address2 = addr.address2;
    }

    public boolean isEqual(BusAddress addr) {
        return (address0 == addr.address0 && address1 == addr.address1 && address2 == address2);
    }

    public boolean isEqual(byte addr0, byte addr1, byte addr2) {
        return (address0 == addr0 && address1 == addr1 && address2 == addr2);
    }

    protected byte address0;
    protected byte address1;
    protected byte address2;
}
