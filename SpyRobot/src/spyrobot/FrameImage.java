/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import javax.swing.*;

/**
 *
 * @author KrisTheSavage
 */
public class FrameImage extends JPanel{

    private Image image = null;

    public void loadImage(BufferedImage image){
        this.image = image;
    }
    public void paint(Graphics g){
        if(image != null)
            g.drawImage(image, 0, 0, this);
    }
}
