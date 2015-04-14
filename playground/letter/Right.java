package letter;

import java.util.*;

public class Right implements Letter
{
   private static final int height = Dimensions.HEIGHT;
   private static final int width = height * 3 / 2;
   private static final int maxHeight = height - Dimensions.strokeWeight;
   private static final int maxWidth = width - Dimensions.strokeWeight;
   private static final int CENTER_X_REFERENCE = 16;

   public Paths getPaths()
   {
      int centerX = width * CENTER_X_REFERENCE / 30;
      int centerY = height / 2;
      int curWidth = width;
      int curHeight = height;
      int curCenterX = centerX;
      int curCenterY = centerY;
      Paths paths = new Paths(width, height, "Right");
      /* Outline */
      paths.add(new Path(Dimensions.MIN_WIDTH, maxHeight * 9 / 16, Path.MovementType.START));
      paths.add(new Path(maxWidth * 1 / 2, maxHeight * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 14 / 30, maxHeight, Path.MovementType.LINE));
      paths.add(new Path(maxWidth, maxHeight * 1 / 2, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 14 / 30, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(maxWidth * 1 / 2, maxHeight * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, maxHeight * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, maxHeight * 9 / 16, Path.MovementType.LINE));

      /* Fill */
      while (curWidth > 0 && curHeight > 0)
      {
         curWidth -= 2 * Dimensions.strokeWeight * 7 / 8;
         curHeight -= 2 * Dimensions.strokeWeight * 7 / 8;
         curCenterX = curWidth * CENTER_X_REFERENCE / 30;
         curCenterY = curHeight / 2 ;
         paths.addAll(getLayer(curWidth, curHeight).translatePoints(centerX - curCenterX, centerY - curCenterY));
      }
      return paths;
   }

   private Paths getLayer(int width, int height)
   {
      Paths paths = new Paths(width, height, "Right");
      width -= Dimensions.strokeWeight;
      height -= Dimensions.strokeWeight;

      paths.add(new Path(Dimensions.MIN_WIDTH, height * 9 / 16, Path.MovementType.START));
      paths.add(new Path(width * 1 / 2, height * 9 / 16, Path.MovementType.LINE));
      paths.add(new Path(width * 14 / 30, height, Path.MovementType.LINE));
      paths.add(new Path(width, height * 1 / 2, Path.MovementType.LINE));
      paths.add(new Path(width * 14 / 30, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(width * 1 / 2, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, height * 7 / 16, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, height * 9 / 16, Path.MovementType.LINE));
      return paths;
   }
}
