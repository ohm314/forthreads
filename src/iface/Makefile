
%.o: %.mod
	true

forthread_types.o: forthread_types.f03 fortypes.h
	$(FC) $(FDEFINES) $(FFLAGS) $(FDEBUG) -c $< -o $@

forthread_data.o: forthread_data.f03 forthread_types.f03 forthread_types.o ciface.h
	$(FC) $(FDEFINES) $(FFLAGS) $(FDEBUG) -c $< -o $@

forthread.o: forthread.f03 \
			ciface.h \
			forthread_data.f03 \
			forthread_data.o \
			forthread_types.f03 \
			forthread_types.o
	$(FC)  $(FFLAGS) $(FDEBUG) -c $< -o $@

#forthread_data.o: forthread_data.f03 
#	$(FC) $(FDEFINES) $(FFLAGS) $(FDEBUG) -c -Tf$< -o $@
#
#forthread.o: forthread.f03 ciface.h forthread_data.f03 forthread_data.o
#	$(FC) $(FDEFINES) $(FFLAGS) $(FDEBUG) -c -Tf$< -o $@

clean:
	rm -f *.o *.mod
