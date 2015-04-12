package letter;

import java.util.*;

public class Four implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT);
      paths.add(new Path(Dimensions.MAX_WIDTH * 4 / 5, Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MAX_WIDTH * 4 / 5, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MIN_WIDTH, Dimensions.MAX_HEIGHT / 3, Path.MovementType.LINE));
      paths.add(new Path(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
