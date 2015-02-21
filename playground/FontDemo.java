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

      int currentX = 0;
      int currentY = -400;
      JFrame frame = new JFrame("Font");
      FontPainter fp = new FontPainter();
      frame.add(fp);
      frame.pack();
      Paths paths = null;

      paths = new A().getPaths();
      currentX += paths.getWidth();
      fp.addLetter(paths.moveOffset(currentX, currentY));

      paths = new E().getPaths();
      currentX += paths.getWidth();
      fp.addLetter(paths.moveOffset(currentX, currentY));
      
      paths = new I().getPaths();
      currentX += paths.getWidth();
      fp.addLetter(paths.moveOffset(currentX, currentY));

      paths = new T().getPaths();
      currentX += paths.getWidth();
      fp.addLetter(paths.moveOffset(currentX, currentY));

      paths = new H().getPaths();
      currentX += 200;
      fp.addLetter(paths.moveOffset(currentX, currentY));
      frame.setVisible(true);
   }
}
