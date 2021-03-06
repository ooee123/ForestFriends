package letter;

import java.util.*;

public class X implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_80, Dimensions.HEIGHT, "X");
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MAX_WIDTH_80, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH_80, Dimensions.MAX_HEIGHT, Path.MovementType.MOVE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
