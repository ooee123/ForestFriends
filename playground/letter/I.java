package letter;

import java.util.*;

public class I implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(Letter.WIDTH / 2, 0, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH / 2, Height.HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
