package letter;

import java.util.*;

public class V implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Height.HEIGHT);
      paths.add(new Path(0, Height.HEIGHT, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH / 2, 0, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Height.HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
