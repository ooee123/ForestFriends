package letter;

import java.util.*;

public class C implements Letter
{
   public static final int STARTING_ANGLE = Letter.DISCRETE_POINTS * 55 / 360;
   public static final int ENDING_ANGLE = Letter.DISCRETE_POINTS - STARTING_ANGLE;

   public Paths getPaths()
   {
      int centerX = Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 1, 2);
      int centerY = Dimensions.getRelativeY(1, 2);
      double actualWidth = centerX + Math.cos(STARTING_ANGLE * 2 * Math.PI / Letter.DISCRETE_POINTS) * centerX; //Don't modify
      double scalar = Dimensions.MAX_WIDTH_90 / actualWidth; //Don't modify
      //Paths paths = new Paths(Dimensions.WIDTH_90, Dimensions.HEIGHT, "C");
      Paths paths = new Paths((int)Math.round(actualWidth) + Dimensions.strokeWeight, Dimensions.HEIGHT, "C");
      scalar = 1;
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
