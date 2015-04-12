package letter;

import java.util.*;

/**
 * An iterable collection of paths. Transformations can be applied to it
 * including scaling, translating, and coordinate flipping between cartesian
 * and raster coordinate systems.
 */
public class Paths extends ArrayList<Path>
{
   private int width;
   private int height;

   /**
    * Constructor for a Paths given the total width and height occupied by
    * all the vectors.
    * @param width The width of the Paths.
    * @param height The height of the Paths.
    */
   public Paths(int width, int height)
   {
      super();
      this.width = width;
      this.height = height;
   }

   /*
   public Paths()
   {
      super();
      this.width = -1;
      this.height = -1;
   }
   */

   /**
    * Grows or shrinks each vector this Paths contains given a scalar.
    * Example: providing a scalar of 2 will double the height and width of the
    * Paths.
    * @param scalar A double indicating by what factor the Paths will scale by.
    */
   public Paths scale(double scalar)
   {
      for (Path p: this)
      {
         p.x *= scalar;
         p.y *= scalar;
      }
      width *= scalar;
      height *= scalar;
      return this;
   }

   /**
    * Basic x, y translation of the Paths.
    * @param deltaX The distance to move each path in the x direction.
    * @param deltaY The distance to move each path in the y direction.
    */
   public Paths translateOffset(int deltaX, int deltaY)
   {
      for (Path p : this)
      {
         p.offsetX += deltaX;
         p.offsetY += deltaY;
      }
      return this;
   }

   /**
    * x, y translation of the points of the Paths.
    * This operation is used when the stroke width is in effect.
    * @param deltaX The distance to move each path in the x direction.
    * @param deltaY The distance to move each path in the y direction.
    */
   public Paths translatePoints(int deltaX, int deltaY)
   {
      for (Path p : this)
      {
         p.x += deltaX;
         p.y += deltaY;
      }
      return this;
   }

   /**
    * Flips between the cartesian coordinate system and raster coordinate
    * system. The cartesian coordinate system is where the origin is on the
    * bottom left, and the raster coordinate system is where the origin is
    * on the top left.
    */
   public Paths flipCoordinates()
   {
      int height = getHeight();
      for (Path p : this)
      {
         System.err.println("Old y: " + p.y + " Height: " + height);
         p.y = -p.y + height;
      }
      return this;
   }

   /**
    * The combined width of this collection of Path.
    * Currently returns the width passed during construction * any scalar.
    * @return The width of this Paths.
    */
   public int getWidth()
   {
      return width;
      /*
      int min = Integer.MAX_VALUE;
      int max = Integer.MIN_VALUE;
      for (Path p : this)
      {
         min = p.x < min ? p.x : min;
         max = p.x > max ? p.x : max;
      }
      return max - min;
      */
   }

   /**
    * The combined height of this collection of Path.
    * Currently returns the height passed during construction * any scalar.
    * @return The height of this Paths.
    */
   public int getHeight()
   {
      return height;
      /*
      int min = Integer.MAX_VALUE;
      int max = Integer.MIN_VALUE;
      for (Path p : this)
      {
         min = p.y < min ? p.y : min;
         max = p.y > max ? p.y : max;
      }
      return max - min;
      */
   }
}
