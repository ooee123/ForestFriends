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
public class FileParser extends JApplet
{
   public static void main(String args[])
   {
      /* Java Applet Stuff */
      boolean useComs = false;
      boolean isFromFile = args.length > 1;
      JFrame frame = new JFrame("Font");
      FontPainter fp = new FontPainter();
      frame.add(fp);
      frame.pack();
      frame.setVisible(true);
      Scanner file = null;
      /* End Java Applet Stuff */

      List<Paths> allPaths = new ArrayList<Paths>();
      int width = 0;
      int height = 1;
      int fontHeight = 0;

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
      PathConverter verter = new PathConverter(height);

      while (file.hasNextLine()) {
         int x = 0;
         int y = 0;
         if (isFromFile)
         {
            x = file.nextInt();
            y = file.nextInt();
         }
         String text = file.nextLine();
         System.out.println(text);
         List<Paths> paths = verter.convertToPaths(x, y, text); 
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
}
