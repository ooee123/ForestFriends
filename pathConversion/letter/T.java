package letter;

import java.util.*;

public class T implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH, Dimensions.HEIGHT, "T");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(4, 5), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.getRelativeX(2, 5), Dimensions.MIN_HEIGHT, Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.getRelativeX(2, 5), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
