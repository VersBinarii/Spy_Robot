/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;

/**
 *
 * @author KrisTheSavage
 */
public class SerialPortReader implements Runnable {

    private static final DateFormat DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HHmm");

    private static final int TYPE_QCIF_IMAGE = 1;
    private static final int TYPE_CIF_IMAGE = 0;
    private static final int TYPE_ROBOT_INFO = 3;

    private final InputStream inputStream;
    private final RobotListener robotListener;
    private File file;
    private volatile boolean running;
    private int vol = 0;
    private Float battVolt = (float) 0;
   // private String  filename;

    public SerialPortReader(InputStream inputStream) {
        this(inputStream, null);
    }

    public SerialPortReader(InputStream inputStream, RobotListener robotListener) {
        this.inputStream = inputStream;
        this.robotListener = robotListener;
        this.running = false;
    }

    public boolean isRunning() {
        return running;
    }

    public void run() {
        this.running = true;
        System.out.println("Reading data from serial port...");
        while (this.running) {
            System.out.println(Thread.currentThread().getName() + " - Waiting for new TLV");
            try {
                // the first byte contains the type of the object to read
                int type = readType();
                if (type == -1) {
                    // there is no more information to read
                    this.running = false;
                    break;
                }
                System.out.println("type = " + type);
                int length = readLength();
                if (length == -1) {
                    System.out.println("Could not read TLV length. Halting the reader thread!");
                    return;
                }
                System.out.println("length = " + length);
                switch (type) {
                    case TYPE_QCIF_IMAGE:
                    case TYPE_CIF_IMAGE:
                        System.out.println("Receiving " + length + " bytes of raw picture data...");
                        int percentage = 0;
                        byte[] imageData = new byte[length];
                        int copiedBytes = 0, count;
                        while ((count = inputStream.read(imageData, copiedBytes, imageData.length - copiedBytes)) > 0) {
                            copiedBytes += count;
                            percentage = (int) (copiedBytes * 1.0 / length * 100);
                            System.out.println(percentage + "%");
                            if (this.robotListener != null) {
                                this.robotListener.imageDownloadProgress(percentage);
                            }
                        }
                        if (count == -1) {
                            System.out.println("Could not read TLV value. Halting the reader thread!");
                            return;
                        }

                        // just save the image to the disk...
                        BMP bmpImage = new BMP(type == TYPE_CIF_IMAGE ? BMP.ImageType.CIF : BMP.ImageType.QCIF);
                        String filename = new String("Image " + DATE_FORMAT.format(new Date(System.currentTimeMillis())) + ".bmp");
                        bmpImage.BMPtoFile(filename, imageData);

                        if(bmpImage.isThere()){
                            Image i = ImageIO.read(bmpImage.getFile());
                            ImageIcon image = new ImageIcon(i);
                            this.robotListener.displayImage(image);
                        }
                        System.out.println("Image saved!");


                        break;
                    case TYPE_ROBOT_INFO:
                        StringBuilder builder = new StringBuilder();
                        for (int i = 0; i < length; i++) {
                            if(i == 9){
                                vol = this.inputStream.read();      //might be battery info
                                continue;
                            }
                            builder.append((char) this.inputStream.read());
                        }
                        if(builder.charAt(0) == 'B' && builder.charAt(1) == 'a'){
                            battVolt = (float) (2*(0.0127 * vol + 0.0046));
                            this.robotListener.robotMessageReceived(builder.toString().concat(battVolt.toString()));
                            break;
                        }
                        this.robotListener.robotMessageReceived(builder.toString());
                        break;
                    default:
                        System.out.print("Unsupported type: " + type);
                        // just ignore the object value
                        if (length > 0) {
                            long bytesToSkip = length;
                            do {
                                bytesToSkip -= this.inputStream.skip(bytesToSkip);
                            } while (bytesToSkip > 0);
                        }
                }
            } catch (IOException ex) {
                // todo: log the error
                ex.printStackTrace();
            }
        }
        System.out.println("SerialPortReader terminated gracefully");
    }

    private int readType() throws IOException {
        return this.inputStream.read();
    }

    private int readLength() throws IOException {
        int length = this.inputStream.read();
        if (length == -1) {
            return -1;
        }
        length <<= 24;
        for (int i = 1; i < 4; i++) {
            int read = this.inputStream.read();
            if (read == -1) {
                return -1;
            }
            length += (read << (24 - (8 * i)));
        }
        return length;
    }


    public void stop() {
        this.running = false;
        try {
            this.inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
        }
    }

}
