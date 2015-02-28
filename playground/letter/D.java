package letter;

import java.util.*;

public class D implements Letter
{
   public static final int D_CURVE_WIDTH = Letter.WIDTH / 2;

   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH);
      paths.add(new Path(0, Letter.HEIGHT, Path.MovementType.START));
      paths.add(new Path(0, 0, Path.MovementType.LINE));
      paths.add(new Path(D_CURVE_WIDTH, 0, Path.MovementType.LINE));
      int centerX = D_CURVE_WIDTH;
      int centerY = Letter.HEIGHT / 2;
      int radiusX = Letter.WIDTH - D_CURVE_WIDTH;
      int unitCircleOffset = Letter.DISCRETE_POINTS * 3 / 4;
      int END = Letter.DISCRETE_POINTS * 5 / 4;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Letter.HEIGHT / 2);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(0, Letter.HEIGHT, Path.MovementType.LINE));

      return paths;
   }
}
