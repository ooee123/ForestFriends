package letter;

import java.util.*;

public class H implements Letter
{
   public static final int WIDTH = Letter.MAX_HEIGHT * 6 / 10;

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(0, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(0, Letter.MAX_HEIGHT / 2, Path.MovementType.MOVE));
      paths.add(new Path(WIDTH, Letter.MAX_HEIGHT / 2, Path.MovementType.LINE));
      paths.add(new Path(WIDTH, Letter.MAX_HEIGHT, Path.MovementType.MOVE));
      paths.add(new Path(WIDTH, 0, Path.MovementType.LINE));
      return paths;
   }
}
