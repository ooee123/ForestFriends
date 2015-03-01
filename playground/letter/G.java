package letter;

import java.util.*;

public class G implements Letter
{
   public static final int STARTING_ANGLE = Letter.DISCRETE_POINTS * 1 / 6;
   public static final int ENDING_ANGLE = Letter.DISCRETE_POINTS * 31 / 32;

   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Letter.HEIGHT);
      int centerX = Letter.WIDTH / 2;
      int centerY = Letter.HEIGHT / 2;
      double actualWidth = centerX + Math.cos(ENDING_ANGLE * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX;
      double scalar = Letter.WIDTH / actualWidth;
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
      paths.add(new Path(Letter.WIDTH / 2, y, Path.MovementType.LINE));

      return paths;
   }
}
