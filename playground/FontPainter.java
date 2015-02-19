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

   public void paint(Graphics g2)
   {
      Graphics2D g = (Graphics2D) g2;
      g.setStroke(new BasicStroke(12, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
      java.util.List<Path> paths = new A().getPaths().moveOffset(100, -100);
      int prevX = paths.get(0).x;
      int prevY = paths.get(0).y;
      for (Path p : paths)
      {
         if (p.type == Path.MovementType.LINE)
         {
            g.drawLine(prevX, -prevY, p.x, -p.y);
         }
         prevX = p.x;
         prevY = p.y;
      }
   /*
      Graphics2D g2 = (Graphics2D)g;
      String fontName = "Highway Gothic";
      //String example = "abcdefghijklmnopqrstuvwxyz0123456789".toUpperCase();
      String example = "a".toUpperCase();
      //String fontName = "Arial";
      Font f = new Font(fontName, 0, 144);

      GlyphVector gv = f.createGlyphVector(g2.getFontRenderContext(), example);

      Shape s = gv.getOutline();

      PathIterator pi = s.getPathIterator(null);

      double[] coords = new double[6];
      double[] lastCoord = {0.0, 0.0};

      Letter a = A();
      a.getPath();
      while (!pi.isDone()) {
         if (pi.currentSegment(coords) != PathIterator.SEG_LINETO) {
            System.out.println("Not a line to");
         }
         int i = 0;
         //for (int i = 0; i < coords.length; i+=2)
         //{
            System.out.printf("(%f, %f\n)", coords[i], coords[i+1]);
            //g2.drawOval((int)coords[i] + 100, (int)coords[i+1]+ 100, 5, 5);
            g2.drawLine((int)lastCoord[0] + 100, (int)lastCoord[1] + 100, (int)coords[i] + 100, (int)coords[i + 1] + 100);
         //}
         lastCoord[0] = coords[0];
         lastCoord[1] = coords[1];
         pi.next();
      }


/*

      g.setFont(f);
      FontMetrics fm = g.getFontMetrics();
      g.drawString(example, 0, fm.getAscent() - 29);
      System.out.printf("Height: %d\nAscent: %d\nDescent: %d\nLeading: %d\nMax Ascent: %d\n", fm.getHeight(), fm.getAscent(), fm.getDescent(), fm.getLeading(), fm.getMaxAscent());
      g2.setStroke(new BasicStroke(12));
      g2.drawLine(20, 6, 20, 12);
      //repaint();
      */
   }

   public Dimension getPreferredSize() {
      return new Dimension(1200, 600);
   }

   public void drawLine(int x1, int y1, int x2, int y2)
   {
      Graphics2D g = (Graphics2D)getGraphics();
      g.setStroke(new BasicStroke(12));
      g.drawLine(x1, y1, x2, y2);
   }

   public void drawLetter(Paths paths)
   {
      Graphics2D g = (Graphics2D)getGraphics();
      g.setStroke(new BasicStroke(30));
      int prevX = paths.get(0).x;
      int prevY = paths.get(0).y;
      for (Path p : paths)
      {
         if (p.type == Path.MovementType.LINE)
         {
            g.drawLine(prevX, -prevY, p.x, -p.y);
         }
         prevX = p.x;
         prevY = p.y;
      }
      update(g);
   }
}
