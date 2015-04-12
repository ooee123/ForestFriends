package letter;

import java.util.*;

public class PathConverter {

   public static final int KERNING = Letter.INCH / 8;
   //public static final int KERNING = 0;
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
      currentX = 0;
      for (int i = 0; i < s.length(); i++)
      {
         char c = s.charAt(i);
         Paths p = convertToPaths(c);
         if (p != null)
         {
            p.translatePoints(strokeWidth / 2, strokeWidth / 2);
            System.err.println("Translating by: " + (strokeWidth / 2));
            p.translateOffset(currentX, currentY).scale(scalar);
            paths.add(p);
            currentX += p.getWidth() + KERNING;
         }
      }
      currentY += Dimensions.HEIGHT;
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
            //paths = new J().getPaths();
            break;
         case 'K':
            //paths = new K().getPaths();
            break;
         case 'L':
            paths = new L().getPaths();
            break;
         case 'M':
            //paths = new M().getPaths();
            break;
         case 'N':
            //paths = new N().getPaths();
            break;
         case 'O':
            paths = new O().getPaths();
            break;
         case 'P':
            //paths = new P().getPaths();
            break;
         case 'Q':
            paths = new Q().getPaths();
            break;
         case 'R':
            //paths = new R().getPaths();
            break;
         case 'S':
            //paths = new S().getPaths();
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
            //paths = new W().getPaths();
            break;
         case 'X':
            //paths = new X().getPaths();
            break;
         case 'Y':
            //paths = new Y().getPaths();
            break;
         case 'Z':
            //paths = new Z().getPaths();
            break;
         case '1':
            //paths = new One().getPaths();
            break;
         case '2':
            //paths = new Two().getPaths();
            break;
         case '3':
            //paths = new Three().getPaths();
            break;
         case '4':
            paths = new Four().getPaths();
            break;
         case '5':
            //paths = new Five().getPaths();
            break;
         case '6':
            //paths = new Six().getPaths();
            break;
         case '7':
            //paths = new Seven().getPaths();
            break;
         case '8':
            //paths = new Eight().getPaths();
            break;
         case '9':
            //paths = new Nine().getPaths();
            break;
         case '0':
            //paths = new Zero().getPaths();
            break;
         case '<':
            paths = new Left().getPaths();
            break;
         case '>':
            paths = new Right().getPaths();
            break;
         case '^':
            //paths = new Up().getPaths();
            break;
         case ' ':
            currentX += Dimensions.MAX_WIDTH;
            break;
         default:
            System.err.println("Not a pathable character: " + c);
            break;
      }
      return paths;
   }

   public static void main(String args[]) {
   }
}
