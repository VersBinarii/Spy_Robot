/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

/**
 *
 * @author KrisTheSavage
 */
public class Utils {

    public static void pause(long ms) {
        try {
            Thread.sleep(ms);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}