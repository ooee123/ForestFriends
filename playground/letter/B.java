package letter;

import java.util.*;

public class B implements Letter
{
   public static final int B_CURVE_WIDTH = Letter.WIDTH * 3 / 5;

   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(0, Height.HEIGHT / 2, Path.MovementType.START));
      paths.add(new Path(B_CURVE_WIDTH, Height.HEIGHT / 2, Path.MovementType.LINE));


      int centerX = B_CURVE_WIDTH;
      int centerY = Height.HEIGHT * 3 / 4;
      final int radiusX = Letter.WIDTH - B_CURVE_WIDTH;
      final int radiusY = Height.HEIGHT / 4;
      int unitCircleOffset = Letter.DISCRETE_POINTS * 3 / 4;
      int END = Letter.DISCRETE_POINTS * 5 / 4;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(0, Height.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, 0, Path.MovementType.LINE));
      paths.add(new Path(B_CURVE_WIDTH, 0, Path.MovementType.LINE));

      centerX = B_CURVE_WIDTH;
      centerY = Height.HEIGHT * 1 / 4;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      return paths;
   }
}
