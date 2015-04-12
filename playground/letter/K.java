package letter;

import java.util.*;

public class K implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Dimensions.MAX_WIDTH, Dimensions.MAX_HEIGHT);
      paths.add(new Path(Dimensions.MAX_WIDTH / 2, Dimensions.MIN_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Dimensions.MAX_WIDTH / 2, Dimensions.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
