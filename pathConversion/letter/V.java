package letter;

import java.util.*;

public class V implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "V");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(1, 2), Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
