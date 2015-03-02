package letter;

import java.util.*;

public class V implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH, Letter.HEIGHT);
      paths.add(new Path(0, Letter.HEIGHT, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH / 2, 0, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH, Letter.HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
