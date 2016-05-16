import java.awt.*;
import java.awt.geom.*;
import java.awt.font.*;
import java.awt.event.*;
import java.awt.image.*;
import letter.*;
import java.util.*;
import java.io.*;
import java.nio.*;
import javax.imageio.ImageIO;

public class Painter {

   private int currentX;
   private int currentY;
   private boolean lowered;
   private BufferedImage image;
   private BufferedImage imageCopy;
   private Graphics2D blackCanvas;
   int preferredWidth;
   int preferredHeight;
   int strokeWidth;
   private boolean showGreen;

   public Painter(int width, int height, int strokeWidth, boolean showGreen) {
      this.preferredWidth = (int)Math.round(width);
      this.preferredHeight = (int)Math.round(height);
      this.strokeWidth = (int)Math.round(strokeWidth);
      this.showGreen = showGreen;

      if (showGreen) {
         imageCopy = new BufferedImage(preferredWidth, preferredHeight, BufferedImage.TYPE_INT_RGB);
         blackCanvas = imageCopy.createGraphics();
      } else {
         image = new BufferedImage(preferredWidth, preferredHeight, BufferedImage.TYPE_INT_RGB);
         blackCanvas = image.createGraphics();
      }
      blackCanvas.setStroke(new BasicStroke(strokeWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
      blackCanvas.setColor(new Color(222, 184, 135));
      blackCanvas.fillRect(0, 0, preferredWidth, preferredHeight);
      blackCanvas.setColor(Color.BLACK);
      currentX = currentY = 0;
   }

   public Dimension getPreferredSize() {
      return new Dimension(preferredWidth, preferredHeight);
   }

   public int getX() {
      return currentX;
   }

   public int getY() {
      return currentY;
   }

   public void setPen(int x, int y) {

      if (showGreen) {
         image = new BufferedImage(imageCopy.getColorModel(), imageCopy.copyData(null), false, new Hashtable());
         Graphics2D canvas = image.createGraphics();
         canvas.setStroke(new BasicStroke(strokeWidth, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
         if (lowered) {
            canvas.setColor(Color.RED);
            blackCanvas.drawLine(currentX, currentY, x, y);
         } else {
            canvas.setColor(Color.GREEN);
         }
         canvas.drawLine(currentX, currentY, x, y);
      } else {
         if (lowered) {
            blackCanvas.drawLine(currentX, currentY, x, y);
         }
      }
      currentX = x;
      currentY = y;
   }

   public void movePen(int x, int y) {
      setPen(currentX + x, currentY + y);
   }

   public void lowerPen() {
      lowered = true;
   }

   public void raisePen() {
      lowered = false;
   }

   public RenderedImage getImage() {
      return image;
   }

   public static void main(String argv[]) {
      double width = Double.valueOf(argv[0]);
      double height = Double.valueOf(argv[1]);
      double fontHeight = Double.valueOf(argv[2]);
      int strokeWidth = Letter.INCH * 1 / 4;
      if (fontHeight == 1) {
      }
      else if (fontHeight >= 2) {
         strokeWidth = Letter.INCH * 3 / 8;
      }

      Painter painter = new Painter(
         (int)Math.round(width * Letter.INCH * FontPainter.DISPLAY_SCALE),
         (int)Math.round(height * Letter.INCH * FontPainter.DISPLAY_SCALE),
         (int)Math.round(strokeWidth * FontPainter.DISPLAY_SCALE), true);

      Scanner scanner = new Scanner(System.in);
      while (scanner.hasNextInt()) {
         int x = scanner.nextInt();
         int y = scanner.nextInt();
         int z = scanner.nextInt();
         if (z == 1) {
            painter.lowerPen();
         } else {
            painter.raisePen();
         }
         painter.setPen(x, y);
         try {
            RenderedImage img = painter.getImage();
            ImageIO.write(img, "jpg", new File("original.jpg"));
            System.out.println("!");
         }
         catch (IOException e)
         {
            System.err.println(e.getMessage());
         }
      }
   }
}
