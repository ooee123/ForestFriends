package letter;

import java.util.*;

public class Paths extends ArrayList<Path>
{
   public Paths scale(double scalar)
   {
      for (Path p: this)
      {
         p.x *= scalar;
         p.y *= scalar;
      }
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
}
