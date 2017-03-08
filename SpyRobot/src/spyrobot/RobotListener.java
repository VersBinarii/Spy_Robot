package spyrobot;

import java.awt.image.BufferedImage;
import javax.swing.Icon;
import javax.swing.ImageIcon;

/**
 * Created by IntelliJ IDEA.
 * User: KrisTheSavage
 * Date: 12/03/11
 * Time: 22:17
 * To change this template use File | Settings | File Templates.
 */
public interface RobotListener {

    /**
     * Called when there's some progress downloading an image from the serial port.
     * @param percentage
     */
    void imageDownloadProgress(int percentage);


    void robotMessageReceived(String message);


    void displayImage(ImageIcon i);
}
