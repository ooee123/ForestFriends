JAVA_FILES = *.java letter/*.java
#JAVA_CLASSES = *.class letter/*.class
all:
	javac $(JAVA_FILES)

javadocs:
	generateJavadocs

everything: everything javadocs

clean:
	rm *.class letter/*.class

cleanJavadocs:
	rm -r javadoc/*

cleanAll: clean cleanJavadocs

%.class: %.java
	javac $<
