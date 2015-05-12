package letter;

import java.util.*;

public class D implements Letter
{
   public static final int D_CURVE_WIDTH = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "D");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(D_CURVE_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      int centerX = D_CURVE_WIDTH;
      int centerY = Dimensions.getRelativeY(1, 2);
      int radiusX = Dimensions.MAX_WIDTH_80 - D_CURVE_WIDTH;
      //int unitCircleOffset = Letter.DISCRETE_POINTS * 3 / 4;
      //int END = Letter.DISCRETE_POINTS * 5 / 4;
      int unitCircleOffset = Letter.DISCRETE_POINTS * 3 / 4 + 0;
      int END = Letter.DISCRETE_POINTS * 5 / 4 - 0;
      for (int point = 1 + unitCircleOffset; point <= END; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Dimensions.getRelativeY(1, 2));
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));

      return paths;
   }
}
