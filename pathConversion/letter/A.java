package letter;

import java.util.*;

public class A implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "A");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 1, 2), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 1, 6), Dimensions.getRelativeY(1, 3), Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 5, 6), Dimensions.getRelativeY(1, 3), Path.MovementType.LINE));
      return paths;
   }
}
