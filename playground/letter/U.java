package letter;

import java.util.*;

public class U implements Letter
{
   public static final int WIDTH = Letter.MAX_HEIGHT * 6 / 10;
   public static final int DISCRETE_POINTS = 16;
   public static final int CURVE_MAX_HEIGHT = Letter.MAX_HEIGHT * 1 / 2;

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(0, Letter.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(0, CURVE_MAX_HEIGHT, Path.MovementType.LINE));
      int centerX = WIDTH / 2;
      int centerY = CURVE_MAX_HEIGHT;
      int xRadius = WIDTH / 2;
      int yRadius = CURVE_MAX_HEIGHT / 2;
      int unitCircleOffset = DISCRETE_POINTS / 2;
      for (double point = unitCircleOffset; point <= DISCRETE_POINTS; point += 0.5)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / DISCRETE_POINTS) * xRadius);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / DISCRETE_POINTS) * yRadius);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      paths.add(new Path(WIDTH, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
