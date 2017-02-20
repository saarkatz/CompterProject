printf './images/\nimg\n5\n.png\n16\n1\nqueryA.png\n\nqueryB.png\nqueryC.png\nqueryD.png\n#\n' | valgrind --tool=memcheck --leak-check=yes ./ex3 
