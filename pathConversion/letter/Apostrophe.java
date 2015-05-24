package letter;

import java.util.*;

public class Apostrophe implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_30, Dimensions.HEIGHT, "'");
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_30, 1, 2), Dimensions.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_30, 1, 2), Dimensions.getRelativeY(7, 10), Path.MovementType.LINE));
      return paths;
   }
}
