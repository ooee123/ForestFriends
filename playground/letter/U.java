package letter;

import java.util.*;

public class U implements Letter
{
   public static final int CURVE_MAX_HEIGHT = Letter.HEIGHT * 1 / 3;

   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH);
      paths.add(new Path(0, Letter.HEIGHT, Path.MovementType.START));
      paths.add(new Path(0, CURVE_MAX_HEIGHT, Path.MovementType.LINE));
      int centerX = Letter.WIDTH / 2;
      int centerY = CURVE_MAX_HEIGHT;
      int unitCircleOffset = Letter.DISCRETE_POINTS / 2;
      for (int point = unitCircleOffset; point <= Letter.DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
