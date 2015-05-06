package letter;

import java.util.*;

public class Up implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "^");
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 1, 2), Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 1, 2), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.getRelativeY(1, 2), Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH, 1, 2), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.getRelativeY(1, 2), Path.MovementType.LINE));
      return paths;
   }
}
