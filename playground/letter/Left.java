package letter;

import java.util.*;

public class Left implements Letter
{
   private static final int height = Dimensions.HEIGHT;
   private static final int width = height * 3 / 2;
   private static final int maxHeight = height - Dimensions.strokeWeight;
   private static final int maxWidth = width - Dimensions.strokeWeight;
   private static final int CENTER_X_REFERENCE = 31;
   private static final int CENTER_X_REFERENCE_DENOMINATOR = 60;

   public Paths getPaths()
   {
      int centerX = width * CENTER_X_REFERENCE / CENTER_X_REFERENCE_DENOMINATOR;
      int centerY = height / 2;
      int curWidth = width;
      int curHeight = height;
      int curCenterX = centerX;
      int curCenterY = centerY;
      Paths paths = new Paths(width, height);
      /* Outline */
      paths.addAll(getLayer(curWidth, curHeight).translatePoints(centerX - curCenterX, centerY - curCenterY));
      paths.add(new Path(maxWidth, maxHeight * 9 / 16, Path.MovementType.START));
      paths.add(new Path(maxWidth * 1 / 2, maxHeight * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 16 / 30, maxHeight, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, maxHeight / 2, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 16 / 30, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 1 / 2, maxHeight * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(maxWidth, maxHeight * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(maxWidth, maxHeight * 9 / 16, Path.MovementType.LINE));

      /* Fill */
      while (curWidth > 0 && curHeight > 0)
      {
         curWidth -= 2 * Dimensions.strokeWeight;
         curHeight -= 2 * Dimensions.strokeWeight;
         curCenterX = curWidth * CENTER_X_REFERENCE / CENTER_X_REFERENCE_DENOMINATOR;
         curCenterY = curHeight / 2 ;
         paths.addAll(getLayer(curWidth, curHeight).translatePoints(centerX - curCenterX, centerY - curCenterY));
      }
      return paths;
   }

   private Paths getLayer(int width, int height)
   {
      Paths paths = new Paths(width, height);
      width -= Dimensions.strokeWeight;
      height -= Dimensions.strokeWeight;
      
      paths.add(new Path(width, height * 9 / 16, Path.MovementType.MOVE));
      paths.add(new Path(width * 1 / 2, height * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(width * 16 / 30, height, Path.MovementType.LINE));
      paths.add(new Path(0, height / 2, Path.MovementType.LINE));
      paths.add(new Path(width * 16 / 30, 0, Path.MovementType.LINE));
      paths.add(new Path(width * 1 / 2, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(width, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(width, height * 9 / 16, Path.MovementType.LINE));
      return paths;
   }
}
