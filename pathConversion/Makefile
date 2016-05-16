JAVA_FILES = *.java letter/*.java
JAVA_CLASSES = *.class letter/*.class
FLAGS = -source 1.7 -target 1.7

all:
	javac $(FLAGS) $(JAVA_FILES)

javadocs:
	generateJavadocs

everything: everything javadocs

clean:
	rm $(JAVA_CLASSES)

cleanJavadocs:
	rm -r javadocs/*

cleanAll: clean cleanJavadocs

%.class: %.java
	javac $(FLAGS) $<
