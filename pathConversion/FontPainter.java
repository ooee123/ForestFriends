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

   private final boolean showGreen = false;
   private java.util.List<Paths> letters;
   private BufferedImage img;
   private Graphics2D imgGraphics;
   private int preferredWidth;
   private int preferredHeight;
   private int strokeWidth;
   private static final boolean displayFrame = false;
   private static final double DISPLAY_SCALE = 0.25;

   public FontPainter(int width, int height, int strokeWidth)
   {
      preferredWidth = (int)Math.round(width * DISPLAY_SCALE);
      preferredHeight = (int)Math.round(height * DISPLAY_SCALE);
      this.strokeWidth = (int)Math.round(strokeWidth * DISPLAY_SCALE);
      letters = new ArrayList<Paths>();
      if (displayFrame)
      {
         JFrame frame = new JFrame("Font");
         frame.add(this);
         frame.pack();
         frame.setVisible(true);
      }
      img = new BufferedImage(preferredWidth, preferredHeight, BufferedImage.TYPE_BYTE_BINARY);
      imgGraphics = img.createGraphics();
      imgGraphics.setColor(Color.WHITE);
      imgGraphics.fillRect(0, 0, preferredWidth, preferredHeight);
   }

   public void paint(Graphics g)
   {
      if (displayFrame)
      {
         drawLetters((Graphics2D)g);
      }
   }

   public void finishDrawing()
   {
      drawLetters(imgGraphics);
      try {
         ImageIO.write(img, "jpg", new File("original.jpg"));
         System.err.println("Done writing image");
      }
      catch (IOException e)
      {
         System.err.println(e.getMessage());
      }
   }

   public Dimension getPreferredSize() {
      return new Dimension(preferredWidth, preferredHeight);
   }

   public void addLetter(Paths paths)
   {
      paths.flipCoordinates();
      letters.add(paths);
   }

   public void drawLetters(Graphics2D g)
   {
      g.setStroke(new BasicStroke(strokeWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
      for (Paths paths : letters)
      {
         int prevX = (int)Math.round(paths.get(0).getX() * DISPLAY_SCALE);
         int prevY = (int)Math.round(paths.get(0).getY() * DISPLAY_SCALE);
         for (Path p : paths)
         {
            int px = (int)Math.round(p.getX() * DISPLAY_SCALE);
            int py = (int)Math.round(p.getY() * DISPLAY_SCALE);
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
