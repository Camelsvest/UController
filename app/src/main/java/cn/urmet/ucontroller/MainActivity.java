package cn.urmet.ucontroller;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

public class MainActivity extends AppCompatActivity implements  CompoundButton.OnCheckedChangeListener {

    private Lamp m_Lamp1, m_Lamp2;
    private Shutter m_Shutter1, m_Shutter2;
    private ToggleButton m_lamp1Btn, m_lamp2Btn;
    private ToggleButton m_Shutter1Btn, m_Shutter2Btn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        //short sh;
        //byte ch;
        //int[] intArr = {6,5,4,3,2};
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        m_lamp1Btn = (ToggleButton)findViewById(R.id.Lamp1);
        m_lamp1Btn.setOnCheckedChangeListener(this);
        m_Lamp1 = new Lamp();
        m_Lamp1.setAddress((byte)1,(byte)1 ,(byte)4);

        m_lamp2Btn = (ToggleButton)findViewById(R.id.Lamp2);
        m_lamp2Btn.setOnCheckedChangeListener(this);
        m_Lamp2 = new Lamp();
        m_Lamp2.setAddress((byte)1, (byte)1, (byte)4);

        m_Shutter1Btn = (ToggleButton)findViewById(R.id.Shutter1);
        m_Shutter1Btn.setOnCheckedChangeListener(this);
        m_Shutter1 = new Shutter();
        m_Shutter1.setAddress((byte)2, (byte)0, (byte)1);

        m_Shutter2Btn = (ToggleButton)findViewById(R.id.Shutter2);
        m_Shutter2Btn.setOnCheckedChangeListener(this);
        m_Shutter2 = new Shutter();
        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());

        //sh = 12345;
        //ch = 56;
        //test(sh, 987654321, 1234567890, 3.123456f, 77.8909, 'A', true, ch, "You're God", this, intArr);
    }

    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean checked) {
        switch (compoundButton.getId()) {
            case R.id.Lamp1:
                if (m_Lamp1 != null) {
                    if (checked)
                        m_Lamp1.switchOn();
                    else
                        m_Lamp1.switchOff();
                }
                break;

            case R.id.Lamp2:
                if (m_Lamp2 != null) {
                    if (checked)
                        m_Lamp2.switchOn();
                    else
                        m_Lamp2.switchOff();
                }
                break;
            case R.id.Shutter1:
                if (m_Shutter1 != null) {
                    if (checked){
                        m_Shutter2.setAddress((byte)2, (byte)0, (byte)1);
                        m_Shutter1.up();
                    }
                    else{
                        m_Shutter2.setAddress((byte)2, (byte)0, (byte)2);
                        m_Shutter1.stop();
                    }
                }
                break;
            case R.id.Shutter2:
                if (m_Shutter2 != null) {
                    if (checked){
                        m_Shutter2.setAddress((byte)2, (byte)0, (byte)1);
                        m_Shutter1.down();
                    }
                    else{
                        m_Shutter2.setAddress((byte)2, (byte)0, (byte)2);
                        m_Shutter2.stop();
                    }
                }
                break;
        }
    }

    @Override
    protected void onDestroy(){
        super.onDestroy();
    }
}
