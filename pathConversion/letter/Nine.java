package letter;

import java.util.*;

public class Nine implements Letter
{
   public static final int CENTER_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public static final int SECOND_ENDING_ANGLE = Letter.DISCRETE_POINTS * 340/360;
   public static final int SECOND_RADIUS_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public static final int SECOND_STARTING_ANGLE = Letter.DISCRETE_POINTS * 700/360;
   public static final int SECOND_RADIUS_Y = (Dimensions.MAX_HEIGHT - 2 * Dimensions.getRelativeY(7, 32)) / 2;
   public static final int SECOND_CENTER_Y = Dimensions.MAX_HEIGHT - SECOND_RADIUS_Y;
   public static final int SECOND_CENTER_X = CENTER_X;


   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "9");
      int point = SECOND_STARTING_ANGLE;
      int x = (int)Math.round(SECOND_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_X);
      int y = (int)Math.round(SECOND_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_Y);
      paths.add(new Path(x, y, Path.MovementType.START));
      for (point = SECOND_STARTING_ANGLE; point >= SECOND_ENDING_ANGLE; point--)
      {
         x = (int)Math.round(SECOND_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_X);
         y = (int)Math.round(SECOND_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_Y);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 3, 8), Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
