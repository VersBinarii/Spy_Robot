/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

import java.io.*;
import java.util.Enumeration;
import java.util.LinkedHashSet;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import javax.comm.*;


/**
 * @author KrisTheSavage
 */
public class RobotController implements SerialPortEventListener {

    public final static int ROBOT_MOVE_FORWARD = 0xF3;
    public final static int ROBOT_MOVE_BACK = 0xF4;
    public final static int ROBOT_MOVE_STOP = 0xF5;
    public final static int ROBOT_MOVE_LEFT = 0xF7;
    public final static int ROBOT_MOVE_RIGHT = 0xF8;
    private static final int ROBOT_MOVE_COMMAND = 0x6d;    //ascii for 'm'
    private final int GRAB_CIF = 0x63;  //ascii for 'C'
    private final int GRAB_QCIF = 0x71; //ascii for 'Q'
    private static final int DEFAULT_BAUD_VALUE = 115200;


    private final static ExecutorService executorService = Executors.newSingleThreadExecutor();

    private SerialPort serialPort;
    private OutputStream outputStream;
    private SerialPortReader serialPortReader;

    public RobotController(String portName) throws NoSuchPortException {
        init(portName, DEFAULT_BAUD_VALUE, null);
    }

    public RobotController(String portName, int baud) throws NoSuchPortException {
        init(portName, baud, null);
    }

    public RobotController(String portName, RobotListener robotListener) throws NoSuchPortException {
        init(portName, DEFAULT_BAUD_VALUE, robotListener);
    }

    public static Set<String> getPorts(){
        Set<String> result = new LinkedHashSet<String>();
        Enumeration portIdentifiers = CommPortIdentifier.getPortIdentifiers();
        while (portIdentifiers.hasMoreElements()) {
            CommPortIdentifier portId = (CommPortIdentifier) portIdentifiers.nextElement();
            if (!portId.isCurrentlyOwned() && portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                result.add(portId.getName());
            }
        }
        return result;
    }

    private void init(String portName, int baud, RobotListener robotListener) throws NoSuchPortException {

        CommPortIdentifier portIdentifier;
        try {
            portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
        } catch (NoSuchPortException e) {
            System.out.println("No port with given name found: " + portName);
            throw e;
        }
        try {
            serialPort = (SerialPort) portIdentifier.open(this.getClass().getSimpleName(), 3000);
            serialPort.setSerialPortParams(baud, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
            serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_NONE);
            serialPort.enableReceiveTimeout(1500);
            Utils.pause(50);

            //set output stream
            outputStream = serialPort.getOutputStream();
            //set input stream
            serialPortReader = new SerialPortReader(serialPort.getInputStream(), robotListener);

            serialPort.addEventListener(this);
            serialPort.notifyOnDataAvailable(true);
            Thread.sleep(50);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void move(int direction) throws IOException {
        this.putch(ROBOT_MOVE_COMMAND);
        this.move(direction, 0);
    }

    public void move(int direction, int speed) throws IOException {
        this.putch(ROBOT_MOVE_COMMAND);
        this.putch(direction);
        this.putch(speed);
    }

    public void getRobotInformation() throws IOException {
        System.out.println("Getting Robot information");
        this.putch(ROBOT_MOVE_COMMAND);
    }

    public void takePicture(BMP.ImageType type) throws IOException {
        System.out.println("Requesting picture: " + type);
        switch (type) {
            case QCIF:
                this.putch(GRAB_QCIF);
                break;
            case CIF:
                this.putch(GRAB_CIF);
                break;
        }
    }

    public void putch(int b) throws IOException {
        outputStream.write(b);
        Utils.pause(50);
    }

    public void close() {
        serialPortReader.stop();
        serialPort.close();
    }

    public void serialEvent(SerialPortEvent event) {
        String logString = "Got serial event: ";
        switch (event.getEventType()) {
            case SerialPortEvent.OE:
                logString += "Overrun Error.";
                break;
            case SerialPortEvent.FE:
                logString += "Flow Error.";
                break;
            case SerialPortEvent.BI:
                logString += "Break interrupt.";
                break;
            case SerialPortEvent.PE:
                logString += "Parity Error";
                break;
            case SerialPortEvent.CD:
                logString += "Carrier detect.";
                break;
            case SerialPortEvent.CTS:
                logString += "Clear to send.";
                break;
            case SerialPortEvent.DSR:
                logString += "Data set ready.";
                break;
            case SerialPortEvent.RI:
                logString += "Ring indicator.";
                break;
            case SerialPortEvent.OUTPUT_BUFFER_EMPTY:
                logString += "Output buffer is empty.";
                break;
            case SerialPortEvent.DATA_AVAILABLE:
                if (!this.serialPortReader.isRunning()) {
                    executorService.execute(serialPortReader);
                }
                // the data available is read by the serial port reader
                return;
        }
        System.out.println(logString);
    }

    public void setDTR(boolean dtr) {
        serialPort.setDTR(dtr);
    }


    public String getName() {
        return serialPort.getName();
    }
}