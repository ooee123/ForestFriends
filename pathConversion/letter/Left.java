package letter;

import java.util.*;

public class Left implements Letter
{
   private static final int maxWidth = Dimensions.WIDTH * 6/5 - Dimensions.strokeWeight;
   public Paths getPaths()
   {
      Paths paths = new Paths(maxWidth + Dimensions.strokeWeight, Dimensions.HEIGHT, "<");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.getRelativeY(1, 2), Path.MovementType.START));
      paths.add(new Path(maxWidth, Dimensions.getRelativeY(1, 2), Path.MovementType.LINE));
      paths.add(new Path(Dimensions.getRelativeX(maxWidth, 1, 2), Dimensions.getRelativeY(1, 12), Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.getRelativeY(1, 2), Path.MovementType.LINE));
      paths.add(new Path(Dimensions.getRelativeX(maxWidth, 1, 2), Dimensions.getRelativeY(11,12), Path.MovementType.LINE));
      return paths;
   }
}
