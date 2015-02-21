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

   public int getWidth()
   {
      return width;
   }
}
