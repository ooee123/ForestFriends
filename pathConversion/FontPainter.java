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
   private Painter painter;
   private int preferredWidth;
   private int preferredHeight;
   private int strokeWidth;
   private static final boolean displayFrame = false;
   public static final double DISPLAY_SCALE = 1.0 / 64.0;
   public static final int MAX_DISTANCE = Letter.INCH / 4;
   //private static final double DISPLAY_SCALE = 1.0 / 4.0;


   public FontPainter(int width, int height, int strokeWidth)
   {
      preferredWidth = (int)Math.round(width * DISPLAY_SCALE);
      preferredHeight = (int)Math.round(height * DISPLAY_SCALE);
      this.strokeWidth = (int)Math.round(strokeWidth * DISPLAY_SCALE);
      letters = new ArrayList<Paths>();
      painter = new Painter(preferredWidth, preferredHeight, this.strokeWidth, false);
   }

   public void finishDrawing()
   {
      drawLetters();
      try {
         RenderedImage img = painter.getImage();
         ImageIO.write(img, "jpg", new File("original.jpg"));
         System.err.println("Done writing image");
      }
      catch (IOException e)
      {
         System.err.println(e.getMessage());
      }
   }

   public void addLetter(Paths paths)
   {
      letters.add(paths);
   }

   public void drawLetters()
   {
      for (Paths paths : letters)
      {
         for (Path p : paths)
         {
            int x = (int)Math.round(p.getX() * DISPLAY_SCALE);
            int y = (int)Math.round(p.getY() * DISPLAY_SCALE);
            if (p.type == Path.MovementType.LINE) {
               painter.lowerPen();
            } else {
               painter.raisePen();
            }
            painter.setPen(x, y);
         }
      }
   }
}
