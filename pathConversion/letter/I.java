package letter;

import java.util.*;

public class I implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_20, Dimensions.HEIGHT, "I");
      //paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_20, 1, 2), Dimensions.MIN_HEIGHT, Path.MovementType.START));
      //paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_20, 1, 2), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
