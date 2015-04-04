package letter;

import java.util.*;

public class H implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(0, Height.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, Height.HEIGHT / 2, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH, Height.HEIGHT / 2, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Height.HEIGHT, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH, 0, Path.MovementType.LINE));
      return paths;
   }
}
