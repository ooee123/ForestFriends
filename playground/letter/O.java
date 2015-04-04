package letter;

import java.util.*;

public class O implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(Letter.WIDTH, Height.HEIGHT / 2, Path.MovementType.START));
      int centerX = Letter.WIDTH / 2;
      int centerY = Height.HEIGHT / 2;
      for (int point = 1; point <= Letter.DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Letter.WIDTH / 2);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Height.HEIGHT / 2);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      return paths;
   }
}
