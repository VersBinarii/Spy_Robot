/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package spyrobot;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 *
 * @author KrisTheSavage
 */
public class BMP {

    public static enum ImageType {
        QCIF,
        CIF
    }

    private final int BITMAP_HEADER_SIZE = 14;
    private final int BITMAP_INFOHEADER_SIZE = 40;
    private final int COLOR_TABLE_SIZE = 4 * 256;

    private byte bmSignature[] = {'B', 'M'};
    private int bmFileSize;
    private int bmReserved = 0;
    private int bmOffset = COLOR_TABLE_SIZE + BITMAP_HEADER_SIZE + BITMAP_INFOHEADER_SIZE;

    private int bmIHSize = 40;
    private int bmWidth;
    private int bmHeight;
    private int bmPlanes = 1;   //one plane(default)
    private int bmBitCount = 8; //256 colors used
    private int bmCompression = 0;
    private int bmImageSize = 0;
    private int bmXpixelsPerM = 0;
    private int bmYpixelsPerM = 0;
    private int bmColorsUsed = 0;
    private int bmColorsImportant = 0;

    private File f;


    BMP(ImageType type) {
        switch (type) {
            case QCIF:
                bmWidth = 144;
                bmHeight = 176;
                break;
            case CIF:
                bmWidth = 352;
                bmHeight = 244;
                break;
        }
        /*
         * the maximum size of BMP file for this image size
         * 14(file header) + 40(info header) + 1024(color table)
         * + (352(image height) * 244(image width)) = 86966 -> 0x0153b6
         */
        bmFileSize = BITMAP_HEADER_SIZE + BITMAP_INFOHEADER_SIZE + COLOR_TABLE_SIZE + bmWidth * bmHeight;
    }

    public void BMPtoFile(String filename, byte[] rawData) {
        FileOutputStream fos;
        try {
            f = new File(filename);
            fos = new FileOutputStream(f);
        } catch (FileNotFoundException ex) {
            System.err.println("File not found...");
            return;
        }

        writeHeaders(fos);
        writeColorTable(fos);
        writeRawBytes(rawData, fos);
        try {
            fos.close();
        } catch (IOException ex) {
            System.err.println("Problem closing file");
        }
    }

    public File getFile(){
        return f;
    }
    public boolean isThere(){
        if(f.exists()){
            return true;
        }
        return false;
    }
    private void writeHeaders(FileOutputStream fos){

            //at the start of BMP file headers must be written
            //first file header
            try {
                fos.write(bmSignature);
                fos.write(intAsDWORD(bmFileSize));
                fos.write(intAsDWORD(bmReserved));
                fos.write(intAsDWORD(bmOffset));
            } catch (IOException ex) {
                System.err.println("Problem writing header data to file");
                }
            try {
                fos.write(intAsDWORD(bmIHSize));
                fos.write(intAsDWORD(bmWidth));
                fos.write(intAsDWORD(bmHeight));
                fos.write(intAsWORD(bmPlanes));
                fos.write(intAsWORD(bmBitCount));
                fos.write(intAsDWORD(bmCompression));
                fos.write(intAsDWORD(bmImageSize));
                fos.write(intAsDWORD(bmXpixelsPerM));
                fos.write(intAsDWORD(bmYpixelsPerM));
                fos.write(intAsDWORD(bmColorsUsed));
                fos.write(intAsDWORD(bmColorsImportant));
            } catch (IOException ex) {
                System.err.println("Problem writing info header data to file");
            }
    }

    private void writeColorTable(FileOutputStream fos){

        for(int i = 0; i < 256; i++){
            try {
                fos.write(i);
                fos.write(i);
                fos.write(i);
                fos.write(0);
            } catch (IOException ex) {
                System.err.println("Problem writing color data to file");
            }
        }
    }

    private void writeRawBytes(byte[] rawData, FileOutputStream fos){
        try {
            fos.write(rawData);
        } catch (IOException ex) {
            System.err.println("Problem writing picture data to file");
        }
    }

    private byte[] intAsWORD(int intVal){

        byte byteVal[] = new byte[2];

        byteVal[0] = (byte)(intVal & 0xff);
        byteVal[1] = (byte)((intVal >> 8) & 0xff);

        return byteVal;
    }

    private byte[] intAsDWORD(int intVal){

        byte byteVal[] = new byte[4];

        byteVal[0] = (byte)(intVal & 0xff);
        byteVal[1] = (byte)((intVal >> 8) & 0xff);
        byteVal[2] = (byte)((intVal >> 16) & 0xff);
        byteVal[3] = (byte)((intVal >> 24) & 0xff);

        return byteVal;
    }
}
