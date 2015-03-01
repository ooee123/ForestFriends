package letter;

import java.util.*;

public class H implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Letter.HEIGHT);
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(0, Letter.HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, Letter.HEIGHT / 2, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT / 2, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH, 0, Path.MovementType.LINE));
      return paths;
   }
}
