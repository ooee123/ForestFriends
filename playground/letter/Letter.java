package letter;

import java.util.*;

public interface Letter
{
   public static final int TICKS_PER_REVOLUTION = 30;
   public static final int REVOLUTION_PER_INCH = 8;
   public static final int MAX_HEIGHT = TICKS_PER_REVOLUTION * REVOLUTION_PER_INCH;
   public Paths getPaths();
} 
