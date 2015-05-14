package letter;

import java.util.*;

public class Two implements Letter
{
   public static final int CENTER_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);

   public static final int FIRST_STARTING_ANGLE = Letter.DISCRETE_POINTS * 165/360; 
   public static final int FIRST_ENDING_ANGLE = Letter.DISCRETE_POINTS * -40/ 360; 
   public static final int FIRST_RADIUS_X = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 1, 2);
   public static final int FIRST_RADIUS_Y = Dimensions.getRelativeY(7, 32);

   public static final int FIRST_CENTER_X = CENTER_X;
   //public static final int SECOND_CENTER_Y = Dimensions.MAX_HEIGHT - SECOND_RADIUS_Y;
   public static final int FIRST_CENTER_Y = Dimensions.MAX_HEIGHT - FIRST_RADIUS_Y;


   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "2");
      int point = FIRST_STARTING_ANGLE;
      int x = (int)Math.round(FIRST_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_X);
      int y = (int)Math.round(FIRST_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_Y);
      paths.add(new Path(x, y, Path.MovementType.START));
      for (point--; point >= FIRST_ENDING_ANGLE; point--)
      {
         x = (int)Math.round(FIRST_CENTER_X + Math.cos(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_X);
         y = (int)Math.round(FIRST_CENTER_Y + Math.sin(point * 2 * Math.PI / Letter.DISCRETE_POINTS) * FIRST_RADIUS_Y);
         paths.add(new Path(x, y, Path.MovementType.LINE));
      }
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH_80, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
