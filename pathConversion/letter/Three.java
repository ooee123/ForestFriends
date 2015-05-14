package letter;

import java.util.*;

public class Three implements Letter
{
   public static final int CENTER_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public static final int FIRST_STARTING_ANGLE = Letter.DISCRETE_POINTS * 290/360; 
   public static final int FIRST_ENDING_ANGLE = Letter.DISCRETE_POINTS * 525/360; 
   public static final int FIRST_RADIUS_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 4, 9);
   public static final int FIRST_RADIUS_Y = Dimensions.getRelativeY(7, 32);

   public static final int SECOND_ENDING_ANGLE = Letter.DISCRETE_POINTS * 430/360;
   public static final int SECOND_RADIUS_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public static final int SECOND_STARTING_ANGLE = Letter.DISCRETE_POINTS * 210/360;
   public static final int FIRST_CENTER_X = CENTER_X;
   public static final int FIRST_CENTER_Y = Dimensions.MAX_HEIGHT - FIRST_RADIUS_Y;
   public static final int SECOND_CENTER_X = CENTER_X;
   public static final int SECOND_RADIUS_Y = (Dimensions.MAX_HEIGHT - 2 * FIRST_RADIUS_Y) / 2;
   public static final int SECOND_CENTER_Y = FIRST_CENTER_Y - FIRST_RADIUS_Y - SECOND_RADIUS_Y;


   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "3");
      int point = SECOND_STARTING_ANGLE;
      int x = (int)Math.round(SECOND_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_X);
      int y = (int)Math.round(SECOND_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_Y);
      paths.add(new Path(x, y, Path.MovementType.START));
      for (point++; point <= SECOND_ENDING_ANGLE; point++)
      {
         x = (int)Math.round(SECOND_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_X);
         y = (int)Math.round(SECOND_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * SECOND_RADIUS_Y);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      for (point = FIRST_STARTING_ANGLE; point <= FIRST_ENDING_ANGLE; point++)
      {
         x = (int)Math.round(FIRST_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_X);
         y = (int)Math.round(FIRST_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_Y);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      return paths;
   }
}
