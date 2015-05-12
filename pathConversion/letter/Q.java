package letter;

import java.util.*;

public class Q implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_90, Dimensions.HEIGHT, "Q");
      paths.add(new Path(Dimensions.MAX_WIDTH_90, Dimensions.getRelativeY(1, 2), Path.MovementType.START));
      int centerX = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 1, 2);
      int centerY = Dimensions.getRelativeY(1, 2);
      final int radiusX = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 1, 2);
      final int radiusY = Dimensions.getRelativeY(1, 2);
      for (int point = 1; point <= Letter.DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusX);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * radiusY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 5, 8), Dimensions.getRelativeY(1, 3), Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 14, 16), Dimensions.getRelativeY(-1, 18), Path.MovementType.LINE));

      return paths;
   }
}
