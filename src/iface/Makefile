#FC = ifort
FC = gfortran
FDEFINES =
FFLAGS = -fPIC
#FDEBUG = -g -traceback


forthread.o: forthread.f03
	$(FC) $(FDEFINES) $(FFLAGS) $(FDEBUG) -c $< -o $@

clean:
	rm -f *.o 
