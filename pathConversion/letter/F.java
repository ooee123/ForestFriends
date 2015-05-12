package letter;

import java.util.*;

public class F implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "F");
      paths.add(new Path(Dimensions.MAX_WIDTH_80, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_80, 2, 3), Dimensions.getRelativeY(1, 2), Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.getRelativeY(1, 2), Path.MovementType.LINE));
      return paths;
   }
}
