package letter;

import java.util.*;

public class F implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "F");
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH * 5 / 6, Dimensions.MAX_HEIGHT / 2, Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT / 2, Path.MovementType.LINE));
      return paths;
   }
}
