package letter;

import java.util.*;

public class Four implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(Letter.WIDTH * 4 / 5, 0, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH * 4 / 5, Height.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, Height.HEIGHT / 3, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Height.HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
