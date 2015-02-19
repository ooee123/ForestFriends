import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.image.*;
import java.io.*;
import java.net.URL;
import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.event.*;

import letter.*;
public class FontDemo extends JApplet {

   public static void main(String args[]) {
      GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
      String[] fontFamilies = ge.getAvailableFontFamilyNames();

      for (String s : fontFamilies) {
         System.out.println(s);
      }

      JFrame frame = new JFrame("Font");
      FontPainter fp = new FontPainter();
      frame.add(fp);
      frame.pack();
      //fp.drawLetter(new A().getPaths().moveOffset(400, 400));
      frame.setVisible(true);
   }
}
