package letter;

import java.util.*;

public class Paths extends ArrayList<Path>
{
   private int width;
   private int height;

   public Paths(int width, int height)
   {
      super();
      this.width = width;
      this.height = height;
   }

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

   public Paths moveOffset(int deltaX, int deltaY)
   {
      for (Path p : this)
      {
         p.offsetX += deltaX;
         p.offsetY += deltaY;
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
         p.y = -p.y + height;
      }
      return this;
   }

   public int getWidth()
   {
      return width;
   }

   public int getHeight()
   {
      return height;
   }
}
