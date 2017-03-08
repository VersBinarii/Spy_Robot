/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

/**
 *
 * @author KrisTheSavage
 */
public class Main{

    /**
     * @param args the command line arguments
     */
    static SpyUI g;
    public static void main(String[] args) {
        g = new SpyUI();
        g.requestFocusInWindow();
    }
}
