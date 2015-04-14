package letter;

import java.util.*;

public class A implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "A");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MAX_WIDTH / 2, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH / 6, Dimensions.MAX_HEIGHT / 3, Path.MovementType.MOVE));
      paths.add(new Path(5 * Dimensions.MAX_WIDTH / 6, Dimensions.MAX_HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
