package letter;

import java.util.*;

public interface Letter
{
   public static final int TICKS_PER_REVOLUTION = 30;
   public static final int REVOLUTION_PER_INCH = 8;
   public static final int MAX_HEIGHT = TICKS_PER_REVOLUTION * REVOLUTION_PER_INCH;
   public static final int DISCRETE_POINTS = 32;
   public static final int WIDTH = MAX_HEIGHT * 3 / 5;
   public Paths getPaths();
} 
