package letter;

import java.util.*;

public class A implements Letter
{
   public static final int WIDTH = (int)(0.6 * Letter.MAX_HEIGHT);

   public Paths getPaths()
   {
      Paths paths = new Paths(WIDTH);
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(WIDTH / 2, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(WIDTH, 0, Path.MovementType.LINE));
      paths.add(new Path(WIDTH / 6, Letter.MAX_HEIGHT / 3, Path.MovementType.MOVE));
      paths.add(new Path(5 * WIDTH / 6, Letter.MAX_HEIGHT / 3, Path.MovementType.LINE));
      return paths;
   }
}
