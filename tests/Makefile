
tests: test test1 test2

test: test.o launcher-item.o
	g++ -o $@ $^ $(GTKLIBS) $(DBUSLIBS) $(LIBS) -losso

test1: test1.o settings-page-items.o launcher-item.o
	g++ -g -o $@ $^ $(GTKLIBS) $(DBUSLIBS) $(LIBS)

test2: test2.o gconf-wrapper.o
	g++ -g -o $@ $^ $(GCONFLIBS)

clean:
	rm -f *.d *.o test test1 test2
