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
   private static int width = 48;
   private static int height = 24;
   private static final int SAFE_ZONE_BORDER_WIDTH = Letter.INCH;
   private static final boolean useComs = false;

   public static void main(String args[])
   {
      /* Java Applet Stuff */
      boolean isFromFile = args.length > 1;
      FontPainter fp = new FontPainter(strokeWidth);
      Scanner file = null;
      /* End Java Applet Stuff */

      List<Paths> allPaths = new ArrayList<Paths>();

      if (isFromFile)
      {
         file = new Scanner(args[1]);
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
      
      /* Serial Coms and Printing */
      SerialComs coms = null;
      PrintWriter printer = null;
      if (useComs)
      {
         coms = new SerialComs();
         try {
            printer = new PrintWriter("Printing.txt");
         }
         catch (FileNotFoundException e)
         {
            System.err.println(e.getMessage());
         }
      }
      PathConverter verter = new PathConverter(fontHeight, strokeWidth);

      /* Convert the entirety of the text file into a List of Paths */
      while (file.hasNextLine()) {
         int x = 0;
         int y = 0;
         if (isFromFile)
         {
            x = file.nextInt();
            y = file.nextInt();
         }
         String text = file.nextLine();
         List<Paths> paths = null;
         try
         {
            paths = verter.convertToPaths(x, y, text); 
         }
         catch (Exception e)
         {
            System.exit(1);
         }
         for (Paths p : paths) {
            fp.addLetter(p);
            if (useComs)
            {
               coms.write(p);
               printPaths(p, printer);
            }
         }
         allPaths.addAll(paths);
         fp.repaint();
         fp.finishDrawing();
      }
      if (useComs)
      {
         printer.close();
         coms.close();
      }
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
