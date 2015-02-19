package letter;

import java.util.*;

public class A implements Letter
{
   private List<Path> paths;
   public static final int WIDTH = 60;
   public A()
   {
      paths = new ArrayList<Path>();
      paths.add(new Path(0, 0, Path.MovementType.START));
      paths.add(new Path(WIDTH / 2, Letter.MAX_HEIGHT, Path.MovementType.LINE));
      paths.add(new Path(WIDTH, 0, Path.MovementType.LINE));
      paths.add(new Path(WIDTH / 3, Letter.MAX_HEIGHT / 3, Path.MovementType.MOVE));
      paths.add(new Path(2 * WIDTH / 3, Letter.MAX_HEIGHT / 3, Path.MovementType.LINE));
   }

   public List<Path> getPaths()
   {
      return paths;
   }
}
