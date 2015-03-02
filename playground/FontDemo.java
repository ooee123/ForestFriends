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
import java.util.*;
import letter.*;

public class FontDemo extends JApplet {

   public static void main(String args[]) {
      GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
      Scanner scanner = new Scanner(System.in);

      int currentX = 0;
      int currentY = -300;
      double scalar = 1;
      JFrame frame = new JFrame("Font");
      FontPainter fp = new FontPainter();
      frame.add(fp);
      frame.pack();
      frame.setVisible(true);
      Paths paths = null;

      while (scanner.hasNextLine())
      {
         currentX = 0;
         currentY += 300;
         String s = scanner.nextLine();
         for (int i = 0; i < s.length(); i++)
         {
            char c = s.charAt(i);
            paths = null;
            switch (Character.toUpperCase(c)) {
               case '!':
                  fp.close();
                  System.exit(0);
                  break;
               case ']':
                  scalar += 0.1;
                  break;
               case '[':
                  scalar -= 0.1;
                  break;
               case 'A':
                  paths = new A().getPaths();
                  break;
               case 'B':
                  paths = new B().getPaths();
                  break;
               case 'C':
                  paths = new C().getPaths();
                  break;
               case 'D':
                  paths = new D().getPaths();
                  break;
               case 'E':
                  paths = new E().getPaths();
                  break;
               case 'F':
                  //paths = new F().getPaths();
                  break;
               case 'G':
                  paths = new G().getPaths();
                  break;
               case 'H':
                  paths = new H().getPaths();
                  break;
               case 'I':
                  paths = new I().getPaths();
                  break;
               case 'J':
                  //paths = new J().getPaths();
                  break;
               case 'K':
                  //paths = new K().getPaths();
                  break;
               case 'L':
                  paths = new L().getPaths();
                  break;
               case 'M':
                  //paths = new M().getPaths();
                  break;
               case 'N':
                  //paths = new N().getPaths();
                  break;
               case 'O':
                  paths = new O().getPaths();
                  break;
               case 'P':
                  //paths = new P().getPaths();
                  break;
               case 'Q':
                  paths = new Q().getPaths();
                  break;
               case 'R':
                  //paths = new R().getPaths();
                  break;
               case 'S':
                  //paths = new S().getPaths();
                  break;
               case 'T':
                  paths = new T().getPaths();
                  break;
               case 'U':
                  paths = new U().getPaths();
                  break;
               case 'V':
                  paths = new V().getPaths();
                  break;
               case 'W':
                  //paths = new W().getPaths();
                  break;
               case 'X':
                  //paths = new X().getPaths();
                  break;
               case 'Y':
                  //paths = new Y().getPaths();
                  break;
               case 'Z':
                  //paths = new Z().getPaths();
                  break;
               case '<':
                  paths = new Left().getPaths();
                  break;
               case '>':
                  paths = new Right().getPaths();
                  break;
               case '^':
                  //paths = new Up().getPaths();
                  break;
               case ' ':
                  currentX += Letter.WIDTH;
                  break;
            }
            if (paths != null)
            {
               fp.addLetter(paths.scale(scalar).moveOffset(currentX, currentY));
               currentX += paths.getWidth();
               currentX += 20;
            }
         }
         fp.repaint();
      }

   }
}
