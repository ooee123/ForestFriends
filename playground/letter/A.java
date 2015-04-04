package letter;

import java.util.*;

public class A implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH / 2, Height.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, 0, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH / 6, Height.HEIGHT / 3, Path.MovementType.MOVE));
      paths.add(new Path(5 * Letter.WIDTH / 6, Height.HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
