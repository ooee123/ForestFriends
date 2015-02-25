package letter;

import java.util.*;

public class Q implements Letter
{
   public static final int WIDTH = Letter.MAX_HEIGHT * 6 / 10;
   public static final int DISCRETE_POINTS = 32;

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(WIDTH, Letter.MAX_HEIGHT / 2, Path.MovementType.START));
      int centerX = WIDTH / 2;
      int centerY = Letter.MAX_HEIGHT / 2;
      for (int point = 1; point <= DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / DISCRETE_POINTS) * WIDTH / 2);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / DISCRETE_POINTS) * Letter.MAX_HEIGHT / 2);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(centerX, centerY, Path.MovementType.MOVE));
      paths.add(new Path(WIDTH, 0, Path.MovementType.LINE));

      return paths;
   }
}
