package letter;

import java.util.*;

public class J implements Letter
{
   public static final int STARTING_ANGLE = Letter.DISCRETE_POINTS * 3 / 5;
   public static final int ENDING_ANGLE = Letter.DISCRETE_POINTS;

   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "J");
      int centerX = Dimensions.getRelativeX(1, 2);
      int centerY = Dimensions.getRelativeY(3, 8);
      double actualWidth = centerX + Math.cos(ENDING_ANGLE * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX; //Don't modify
      double scalar = Dimensions.MAX_WIDTH / actualWidth; //Don't modify
      int point = STARTING_ANGLE;
      int x = (int)Math.round((centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX) * scalar);
      int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerY);
      paths.add(new Path(x, y, Path.MovementType.START));
      for (point++; point <= ENDING_ANGLE; point++)
      {
         x = (int)Math.round((centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX) * scalar);
         y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));

      return paths;
   }
}
