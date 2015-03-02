package letter;

import java.util.*;

public class Four implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Letter.HEIGHT);
      paths.add(new Path(Letter.WIDTH * 4 / 5, 0, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH * 4 / 5, Letter.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, Letter.HEIGHT / 3, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
