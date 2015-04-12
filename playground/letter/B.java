package letter;

import java.util.*;

public class B implements Letter
{
   public static final int B_CURVE_WIDTH = Dimensions.MAX_WIDTH * 3 / 5;

   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT);
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT / 2, Path.MovementType.START));
      paths.add(new Path(B_CURVE_WIDTH, Dimensions.MAX_HEIGHT / 2, Path.MovementType.LINE));


      int centerX = B_CURVE_WIDTH;
      int centerY = Dimensions.MAX_HEIGHT * 3 / 4;
      final int radiusX = Dimensions.MAX_WIDTH - B_CURVE_WIDTH;
      final int radiusY = Dimensions.MAX_HEIGHT / 4;
      int unitCircleOffset = Letter.DISCRETE_POINTS * 3 / 4;
      int END = Letter.DISCRETE_POINTS * 5 / 4;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(B_CURVE_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));

      centerX = B_CURVE_WIDTH;
      centerY = Dimensions.MAX_HEIGHT * 1 / 4;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      return paths;
   }
}
