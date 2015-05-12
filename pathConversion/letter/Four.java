package letter;

import java.util.*;

public class Four implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.WIDTH_90, Dimensions.HEIGHT, "4");
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 4, 5), Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.getRelativeX(Dimensions.MAX_WIDTH_90, 4, 5), Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.getRelativeY(1 , 3), Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH_90, Dimensions.getRelativeY(1,  3), Path.MovementType.LINE));
      return paths;
   }
}
