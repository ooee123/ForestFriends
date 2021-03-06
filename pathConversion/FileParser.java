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
   private static int strokeWidth = Letter.INCH * 1 / 4;
   private static int thickness = 1;
   private static int fontHeight = 1;
   private static int width = 12;
   private static int height = 12;
   private static final int SAFE_ZONE_BORDER_WIDTH = Letter.INCH;
   public static final int MAX_DISTANCE = Letter.INCH / 4;

   public static void main(String args[])
   {
      /* Java Applet Stuff */
      boolean isFromFile = args.length > 0;
      Scanner file = null;
      /* End Java Applet Stuff */

      List<Paths> allPaths = new ArrayList<Paths>();

      if (isFromFile) {
         try {
            file = new Scanner(new File(args[0]));
         }
         catch (FileNotFoundException e) {
            System.err.println(e);
            System.exit(1);
         }
      }
      else {
         file = new Scanner(System.in);
      }

      if (isFromFile) {
         thickness = file.nextInt();
         width = file.nextInt();
         height = file.nextInt();
         fontHeight = file.nextInt();
         if (fontHeight == 1) {
         }
         else if (fontHeight >= 2) {
            strokeWidth = Letter.INCH * 3 / 8;
         }
      }
      FontPainter fp = new FontPainter(width * Letter.INCH, height * Letter.INCH, strokeWidth);
      /* Serial Coms and Printing */
      PrintWriter printer = null;
      try {
         printer = new PrintWriter("coordinates.txt");
      }
      catch (FileNotFoundException e)
      {
         System.err.println(e.getMessage());
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
         List<Paths> paths = verter.convertToPaths(x, y, text); 
         try
         {
            checkPathsWithinBounds(paths, width * Letter.INCH, height * Letter.INCH, strokeWidth);
         }
         catch (BorderException e)
         {
            System.err.println(e);
            System.exit(2);
         }
         for (Paths p : paths) {
            p.flipCoordinates();
            p = PathConverter.breakApart(p, MAX_DISTANCE);
            fp.addLetter(p);
            System.err.println(p);
            printPaths(p, printer);
         }
         allPaths.addAll(paths);
         if (!isFromFile)
         {
            fp.repaint();
         }
      }
      printer.close();
      System.err.println("Done printing coords");
      fp.finishDrawing();
      System.exit(0);
   }

   private static void printPaths(Paths paths, PrintWriter printer)
   {
      String toPrint; 
      toPrint = String.format("# %s\n", paths.getLetter());
      System.err.print(toPrint);
      printer.print(toPrint);
      for (Path p: paths) {
         toPrint = String.format("%d, %d, %d\n", p.getX(), p.getY(), p.type.ordinal());
         System.err.print(toPrint);
         printer.print(toPrint);
      }
   }

   private static boolean checkPathsWithinBounds(List<Paths> paths, int width, int height, int strokeWeight) throws BorderException
   {
      int minWidth = strokeWeight / 2 + SAFE_ZONE_BORDER_WIDTH;
      int minHeight = strokeWeight / 2 + SAFE_ZONE_BORDER_WIDTH;
      int maxWidth = width - strokeWeight / 2 - SAFE_ZONE_BORDER_WIDTH;
      int maxHeight = height - strokeWeight / 2 - SAFE_ZONE_BORDER_WIDTH;
      for (Paths p : paths) {
         for (Path path : p) {
            if (path.getX() < minWidth || path.getX() > maxWidth || path.getY() < minHeight || path.getY() > maxHeight) {
               throw new BorderException("Letter '" + p.getLetter() + "' is out of bounds of coordinates: " + path.getX() + ", " + path.getY());
            }
         }
      }
      return true;
   }
}
