package letter;

import java.util.*;

public class PathConverter {

   public static final int KERNING = Dimensions.HEIGHT / 8;
   public static final int LINE_SPACING = Letter.INCH / 8;
   public static final int MAX_DISTANCE = Letter.INCH / 4;
   private int currentX;
   private int currentY;
   private double scalar;
   private int strokeWidth;

   /**
    *
    * @param fontHeight The height of the font in inches
    */
   public PathConverter(int fontHeight, int strokeWidth)
   {
      this.strokeWidth = strokeWidth;
      Dimensions.setDimensions(fontHeight, strokeWidth);
      currentX = 0;
      currentY = 0;
      scalar = 1;
   }

   public List<Paths> convertToPaths(int x, int y, String s)
   {
      // TO DO
      // Set up x and y in account for currentX and currentY
      List<Paths> paths = new ArrayList<Paths>();
      currentX = x;
      currentY = y;
      for (int i = 0; i < s.length(); i++)
      {
         char c = s.charAt(i);
         Paths p = convertToPaths(c);
         if (p != null)
         {
            p.translatePoints(strokeWidth / 2, strokeWidth / 2);
            p.translateOffset(currentX, currentY).scale(scalar);
            //p = breakApart(p, MAX_DISTANCE);
            paths.add(p);
            currentX += p.getWidth() + KERNING;
         }
      }

      return paths;
   }

   public void setScale(double s)
   {
      scalar = s;
   }

   public void addScale(double s)
   {
      scalar += s;
   }

   public Paths convertToPaths(char c)
   {
      Paths paths = null;
      switch (Character.toUpperCase(c)) {
         case 'A':
            paths = new A().getPaths();
            break;
         case 'B':
            paths = new B().getPaths();
            break;
         case 'C':
            paths = new C().getPaths();
            break;
         case 'D':
            paths = new D().getPaths();
            break;
         case 'E':
            paths = new E().getPaths();
            break;
         case 'F':
            paths = new F().getPaths();
            break;
         case 'G':
            paths = new G().getPaths();
            break;
         case 'H':
            paths = new H().getPaths();
            break;
         case 'I':
            paths = new I().getPaths();
            break;
         case 'J':
            paths = new J().getPaths();
            break;
         case 'K':
            paths = new K().getPaths();
            break;
         case 'L':
            paths = new L().getPaths();
            break;
         case 'M':
            paths = new M().getPaths();
            break;
         case 'N':
            paths = new N().getPaths();
            break;
         case 'O':
            paths = new O().getPaths();
            break;
         case 'P':
            paths = new P().getPaths();
            break;
         case 'Q':
            paths = new Q().getPaths();
            break;
         case 'R':
            paths = new R().getPaths();
            break;
         case 'S':
            paths = new S().getPaths();
            break;
         case 'T':
            paths = new T().getPaths();
            break;
         case 'U':
            paths = new U().getPaths();
            break;
         case 'V':
            paths = new V().getPaths();
            break;
         case 'W':
            paths = new W().getPaths();
            break;
         case 'X':
            paths = new X().getPaths();
            break;
         case 'Y':
            paths = new Y().getPaths();
            break;
         case 'Z':
            paths = new Z().getPaths();
            break;
         case '.':
            paths = new Period().getPaths();
            break;
         case '1':
            paths = new One().getPaths();
            break;
         case '2':
            paths = new Two().getPaths();
            break;
         case '3':
            paths = new Three().getPaths();
            break;
         case '4':
            paths = new Four().getPaths();
            break;
         case '5':
            paths = new Five().getPaths();
            break;
         case '6':
            paths = new Six().getPaths();
            break;
         case '7':
            paths = new Seven().getPaths();
            break;
         case '8':
            paths = new Eight().getPaths();
            break;
         case '9':
            paths = new Nine().getPaths();
            break;
         case '0':
            paths = new Zero().getPaths();
            break;
         case '<':
            paths = new Left().getPaths();
            break;
         case '>':
            paths = new Right().getPaths();
            break;
         case '^':
            paths = new Up().getPaths();
            break;
         case '\'':
            paths = new Apostrophe().getPaths();
            break;
         case ' ':
            currentX += Dimensions.getRelativeY(3, 5);
            break;
         case '_':
            paths = new Underscore().getPaths();
            break;
         default:
            System.err.println("Not a pathable character: " + c);
            break;
      }
      return paths;
   }

   public static Paths breakApart(Paths paths, int maxDistance)
   {
      Paths newSplitPaths = new Paths(paths.getWidth(), paths.getHeight(), paths.getLetter());

      Path prevPath = paths.get(0);
      newSplitPaths.add(prevPath);
      for (int i = 1; i < paths.size(); i++)
      {
         Path curPath = paths.get(i);

         if (curPath.type == Path.MovementType.LINE)
         {
            if (distance(prevPath, curPath) > maxDistance)
            {
               // Split up the line
               newSplitPaths.addAll(splitUpLine(prevPath, curPath, MAX_DISTANCE));
            }
         }
         newSplitPaths.add(curPath);
         prevPath = curPath;
      }
      return newSplitPaths;
   }

   public static int distance(Path p1, Path p2)
   {
      int d1 = p1.getX() - p2.getX();
      int d2 = p1.getY() - p2.getY();
      return (int)Math.round(Math.sqrt((d1 * d1) + (d2 * d2)));
   }

   public static List<Path> splitUpLine(Path p1, Path p2, int maxDistance)
   {
      List<Path> paths = new ArrayList<Path>();
      int x1 = p1.getX();
      int y1 = p1.getY();
      int x2 = p2.getX();
      int y2 = p2.getY();

      double totalDistance = distance(p1, p2);
      int dX = x2 - x1;
      int dY = y2 - y1;

      if (dX == 0 || dY == 0)
      {
         return paths;
      }
      
      double theta = Math.atan2(dY, dX);

      double xFactor = Math.cos(theta);
      double yFactor = Math.sin(theta);

      int inchingDistance = maxDistance;
      
      while (inchingDistance < totalDistance)
      {
         int newX = (int)Math.round(inchingDistance * xFactor);
         int newY = (int)Math.round(inchingDistance * yFactor);
         paths.add(new Path(x1 + newX, y1 + newY, Path.MovementType.LINE));
         inchingDistance += maxDistance;
      }
      return paths;
   }
   

   public static void main(String args[]) {
   }
}
