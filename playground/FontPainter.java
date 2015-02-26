import java.applet.Applet;
import java.awt.*;
import java.awt.geom.*;
import java.awt.font.*;
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

public class FontPainter extends Component {

   private java.util.List<Paths> letters;
   private int currentX;
   private int currentY;

   public FontPainter()
   {
      letters = new ArrayList<Paths>();
   }

   public void paint(Graphics g2)
   {
      Graphics2D g = (Graphics2D) g2;
      g.setStroke(new BasicStroke(Letter.MAX_HEIGHT / 8, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
      drawLetters(g);

/*
      String fontName = "Highway Gothic";
      String example = "abcdefghijklmnopq".toUpperCase();
      String example2 = "rstuvwxyz0123456789".toUpperCase();

      Font f = new Font(fontName, 0, 100);
      g.setFont(f);
      g.drawString(example, 0, 200);
      g.drawString(example2, 0, 500);
*/
   }

   public Dimension getPreferredSize() {
      return new Dimension(1200, 600);
   }

   public void moveOffset(int deltaX, int deltaY)
   {
      currentX += deltaX;
      currentY += deltaY;
   }
   
   public void setOffset(int x, int y)
   {
      currentX = x;
      currentY = y;
   }

   public void drawString(int x, int y, String s)
   {
   }
      
   public void drawLine(int x1, int y1, int x2, int y2)
   {
      Graphics2D g = (Graphics2D)getGraphics();
      g.setStroke(new BasicStroke(12));
      g.drawLine(x1, y1, x2, y2);
   }

   public void addLetter(Paths paths)
   {
      letters.add(paths);
   }

   private void drawLetters(Graphics2D g)
   {
      final boolean showGreen = false;
      for (Paths paths : letters)
      {
         int prevX = paths.get(0).x;
         int prevY = paths.get(0).y;
         for (Path p : paths)
         {
            if (p.type == Path.MovementType.LINE)
            {
               g.setColor(Color.BLACK);
               g.drawLine(prevX, -prevY, p.x, -p.y);
            }
            else if (showGreen && p.type == Path.MovementType.MOVE)
            {
               g.setColor(Color.GREEN);
               g.drawLine(prevX, -prevY, p.x, -p.y);
            }
            prevX = p.x;
            prevY = p.y;
         }
      }
   }
}
