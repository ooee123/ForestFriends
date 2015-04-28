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
   private int currentX;
   private int currentY;
   private boolean saveImage = true;
   private BufferedImage img;
   private Graphics2D imgGraphics;
   private int preferredWidth = 1400;
   private int preferredHeight = 900;
   private int strokeWidth;

   public FontPainter(int strokeWidth)
   {
      JFrame frame = new JFrame("Font");
      frame.add(this);
      frame.pack();
      frame.setVisible(true);
      this.strokeWidth = strokeWidth;
      letters = new ArrayList<Paths>();
      //img = new BufferedImage(preferredWidth, preferredHeight, BufferedImage.TYPE_INT_RGB);
      img = new BufferedImage(preferredWidth, preferredHeight, BufferedImage.TYPE_BYTE_BINARY);
      imgGraphics = img.createGraphics();
      imgGraphics.setColor(Color.WHITE);
      imgGraphics.fillRect(0, 0, preferredWidth, preferredHeight);
   }

   public void paint(Graphics g2)
   {
      Graphics2D g = (Graphics2D) g2;
      drawLetters(g);
      if (saveImage)
      {
         drawLetters(imgGraphics);
      }
      finishDrawing();
   }

   public void finishDrawing()
   {
      try {
         //img.createGraphics().drawImage(createImage(preferredWidth, preferredHeight), 0, 0, null);
         ImageIO.write(img, "jpg", new File("image.jpg"));
         if (saveImage)
         {
            ImageIO.write((BufferedImage)createImage(preferredWidth, preferredHeight), "jpg", new File("image2.jpg"));
         }
      }
      catch (IOException e)
      {
         System.err.println(e.getMessage());
      }
   }

   public Dimension getPreferredSize() {
      return new Dimension(preferredWidth, preferredHeight);
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

   public void addLetter(Paths paths)
   {
      paths.flipCoordinates();
      letters.add(paths);
   }

   private void drawLetters(Graphics2D g)
   {
      g.setStroke(new BasicStroke(strokeWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
      for (Paths paths : letters)
      {
         int prevX = paths.get(0).getX();
         int prevY = paths.get(0).getY();
         for (Path p : paths)
         {
            int px = p.getX();
            int py = p.getY();
            if (p.type == Path.MovementType.LINE)
            {
               //g.setColor(drawingColor);
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
            System.out.printf("(%d, %d, %d)\n", prevX, prevY, p.type.ordinal());
         }
      }
   }
}
