CMP = g++ -std=c++11
CLASS = fighter
MAIN = project
EXEC = FightToTheDeath

$(EXEC): $(CLASS).o $(MAIN).o
	$(CMP) $(CLASS).o $(MAIN).o gfx.o -lX11 -o $(EXEC)

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CMP) -c $(CLASS).cpp -o $(CLASS).o

$(MAIN).o: $(MAIN).cpp $(CLASS).h
	$(CMP) -c $(MAIN).cpp -o $(MAIN).o

clean:
	rm $(EXEC) project.o fighter.o

