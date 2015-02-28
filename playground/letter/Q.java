package letter;

import java.util.*;

public class Q implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH);
      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT / 2, Path.MovementType.START));
      int centerX = Letter.WIDTH / 2;
      int centerY = Letter.HEIGHT / 2;
      for (int point = 1; point <= Letter.DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Letter.WIDTH / 2);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Letter.HEIGHT / 2);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(centerX, centerY, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH, 0, Path.MovementType.LINE));

      return paths;
   }
}
