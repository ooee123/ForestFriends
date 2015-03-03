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

import java.nio.*;

import letter.*;

public class FontPainter extends Component {

   private java.util.List<Paths> letters;
   private int currentX;
   private int currentY;
   private SerialComs coms;

   private PrintWriter printer;
   private boolean useComs = true;

   public FontPainter()
   {
      letters = new ArrayList<Paths>();
      if (useComs)
      {
         /*
         Thread t = new Thread() {
            public void run() {
               coms = new SerialComs();
               while (true);
            }
         };
         t.start();
         */
         coms = new SerialComs();
         try {
            printer = new PrintWriter("Printing.txt");
         }
         catch (FileNotFoundException e)
         {
            System.err.println(e.getMessage());
         }
      }
   }

   public void close()
   {
      coms.close();
      if (useComs)
      {
         printer.close();
      }
   }

   public void paint(Graphics g2)
   {
      Graphics2D g = (Graphics2D) g2;
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
      
   public void addLetter(Paths paths)
   {
      paths.flipCoordinates();
      letters.add(paths);
      if (useComs)
      {
         for (Path p : paths)
         {
            coms.write(p.getX());
            coms.write(p.getY());
            coms.write((short)p.type.ordinal());
            coms.flush();
            printer.print("(");
            printer.print(p.getX());
            printer.print(", ");
            printer.print(p.getY());
            printer.print(", ");
            printer.print(p.type.ordinal());
            printer.println(")");
         }
      }
   }

   private void drawLetters(Graphics2D g)
   {
      final boolean showGreen = false;
      for (Paths paths : letters)
      {
         //g.setStroke(new BasicStroke(paths.getHeight() / 8, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
         g.setStroke(new BasicStroke(Letter.INCH / 8, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
         int prevX = paths.get(0).getX();
         int prevY = paths.get(0).getY();
         for (Path p : paths)
         {
            int px = p.getX();
            int py = p.getY();
            if (p.type == Path.MovementType.LINE)
            {
               g.setColor(Color.BLACK);
               g.drawLine(prevX, prevY, px, py);
            }
            else if (showGreen && p.type == Path.MovementType.MOVE)
            {
               g.setColor(Color.GREEN);
               g.drawLine(prevX, prevY, px, py);
            }
            prevX = px;
            prevY = py;
         }
      }
   }
}
