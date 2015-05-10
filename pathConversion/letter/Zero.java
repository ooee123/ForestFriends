package letter;

import java.util.*;

public class Zero implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "0");
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT / 2, Path.MovementType.START));
      int centerX = Dimensions.MAX_WIDTH / 2;
      int centerY = Dimensions.MAX_HEIGHT / 2;
      for (int point = 1; point <= Letter.DISCRETE_POINTS; point++)
      {
         int x = (int)Math.round(centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Dimensions.MAX_WIDTH / 2);
         int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * Dimensions.MAX_HEIGHT / 2);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      return paths;
   }
}
