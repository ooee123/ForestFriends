package letter;

import java.util.*;

public class Paths extends ArrayList<Path>
{
   private int width;

   public Paths(int width)
   {
      super();
      this.width = width;
   }

   public Paths scale(double scalar)
   {
      for (Path p: this)
      {
         p.x *= scalar;
         p.y *= scalar;
      }
      width *= scalar;
      return this;
   }

   public Paths moveOffset(int deltaX, int deltaY)
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
      int maxHeight = getHeight();
      for (Path p : this)
      {
         p.y = -p.y + maxHeight;
      }
      return this;
   }

   public int getWidth()
   {
      return width;
   }

   public int getHeight()
   {
      int maxHeight = -1;
      for (Path p : this)
      {
         maxHeight = p.y > maxHeight ? p.y : maxHeight;
      }
      return maxHeight;
   }
}
