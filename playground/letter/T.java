package letter;

import java.util.*;

public class T implements Letter
{
   public Paths getPaths()
   {
      Paths paths = new Paths(Letter.WIDTH);
      paths.add(new Path(0, Letter.MAX_HEIGHT, Path.MovementType.START));
      paths.add(new Path(Letter.WIDTH, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(Letter.WIDTH / 2, 0, Path.MovementType.MOVE));
      paths.add(new Path(Letter.WIDTH / 2, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      return paths;
   }
}
