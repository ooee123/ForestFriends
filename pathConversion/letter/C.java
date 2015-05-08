package letter;

import java.util.*;

public class C implements Letter
{
   public static final int STARTING_ANGLE = Letter.DISCRETE_POINTS * 1 / 6;
   public static final int ENDING_ANGLE = Letter.DISCRETE_POINTS - STARTING_ANGLE;

   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "C");
      int centerX = Dimensions.MAX_WIDTH / 2;
      int centerY = Dimensions.MAX_HEIGHT / 2;
      double actualWidth = centerX + Math.cos(STARTING_ANGLE * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX; //Don't modify
      double scalar = Dimensions.MAX_WIDTH / actualWidth; //Don't modify
      int point = STARTING_ANGLE; //Initialize the small line segment
      int x = (int)Math.round((centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX) * scalar);
      int y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerY);
      paths.add(new Path(x, y, Path.MovementType.START));
      for (point++; point <= ENDING_ANGLE; point++)
      {
         x = (int)Math.round((centerX + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX) * scalar);
         y = (int)Math.round(centerY + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerY);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }

      return paths;
   }
}
