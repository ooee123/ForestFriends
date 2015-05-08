import java.applet.Applet;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import letter.*;
import java.io.*;

/**
 * Parses a CCPF file.
 *
 * A CCPF file is a text file with line separated values. 
 * The format of a CCPF file is as follows:
 * WIDTH OF BOARD IN INCHES
 * HEIGHT OF BOARD IN INCHES
 * FONT HEIGHT IN INCHES
 * ___ Then follows any multiple of 3 line triplet of the follow ___
 * X-COORDINATE
 * Y-COORDINATE
 * TEXT
 */
public class FileParser
{
   private static int strokeWidth = Letter.INCH * 1 / 8;
   private static int fontHeight = 1;
   private static int width = 12;
   private static int height = 12;
   private static final int SAFE_ZONE_BORDER_WIDTH = Letter.INCH;
   private static final boolean useComs = false;
   private static final boolean printToFile = true;
   private static final boolean previewSign = true;

   public static void main(String args[])
   {
      /* Java Applet Stuff */
      boolean isFromFile = args.length > 0;
      Scanner file = null;
      /* End Java Applet Stuff */

      List<Paths> allPaths = new ArrayList<Paths>();

      if (isFromFile)
      {
         try
         {
            file = new Scanner(new File(args[0]));
         }
         catch (FileNotFoundException e)
         {
            System.err.println("NOT FOUND");
            System.exit(-1);
         }
      }
      else
      {
         file = new Scanner(System.in);
      }

      if (isFromFile)
      {
         width = file.nextInt();
         height = file.nextInt();
         fontHeight = file.nextInt();
      }
      
      FontPainter fp = null;
      if (previewSign)
      {
         fp = new FontPainter(width * Letter.INCH, height * Letter.INCH, strokeWidth);
      }
      /* Serial Coms and Printing */
      SerialComs coms = null;
      PrintWriter printer = null;
      if (useComs)
      {
         coms = new SerialComs();
      }
      if (printToFile)
      {
         try {
            printer = new PrintWriter("coordinates.txt");
         }
         catch (FileNotFoundException e)
         {
            System.err.println(e.getMessage());
         }
      }
      PathConverter verter = new PathConverter(fontHeight, strokeWidth);

      /* Convert the entirety of the text file into a List of Paths */
      while (file.hasNext()) {
         int x = 0;
         int y = 0;
         if (isFromFile)
         {
            x = file.nextInt() * Letter.INCH;
            y = (file.nextInt() - 1) * Letter.INCH;
            file.nextLine();
         }
         else
         {
            x = 0;
            y += fontHeight * Letter.INCH;
         }
         String text = file.nextLine();
         List<Paths> paths = null;
         paths = verter.convertToPaths(x, y, text); 
         for (Paths p : paths) {
            if (previewSign)
            {
               fp.addLetter(p);
            }
            if (useComs)
            {
               coms.write(p);
            }
            if (printToFile)
            {
               printPaths(p, printer);
            }
         }
         allPaths.addAll(paths);
         if (previewSign)
         {
            fp.repaint();
            fp.finishDrawing();
         }
      }
      if (useComs)
      {
         coms.close();
      }
      if (printToFile)
      {
         printer.close();
      }
      fp.finishDrawing();
      //System.exit(0);
   }

   private static void printPaths(Paths paths, PrintWriter printer)
   {
      for (Path p: paths)
      {
         printer.print(String.format("(%d, %d, %d)\n", p.getX(), p.getY(), p.type.ordinal()));
      }
   }

   private boolean checkPathsWithinBounds(List<Paths> paths, int width, int height, int strokeWeight) throws BorderException
   {
      int minWidth = strokeWeight / 2 + SAFE_ZONE_BORDER_WIDTH;
      int minHeight = strokeWeight / 2 + SAFE_ZONE_BORDER_WIDTH;
      int maxWidth = width - strokeWeight / 2 - SAFE_ZONE_BORDER_WIDTH;
      int maxHeight = height - strokeWeight / 2 - SAFE_ZONE_BORDER_WIDTH;
      for (Paths p : paths)
      {
         for (Path path : p)
         {
            if (path.getX() < minWidth || path.getX() > maxWidth || path.getY() < minHeight || path.getY() > maxHeight)
            {
               throw new BorderException("Letter '" + p.getLetter() + "' is out of bounds");
            }
         }
      }
      return true;
   }
}
