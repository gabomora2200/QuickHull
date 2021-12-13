CC=g++
LFLAGS=-fopenmp
OPENMP=Parallel.cpp
OPENMPFOR=ParallelwithFor.cpp
OPENMP12=ParallelwhenS.cpp
SERIAL=serial.cpp

OUT_PARALLEL=mp
OUT_PARALLELWITHFOR=mpfor
OUT_PARALLEL12=when12
OUT_SERIAL=serial


build:
	$(CC) -o $(OUT_PARALLEL) $(OPENMP) $(LFLAGS)
	$(CC) -o $(OUT_PARALLELWITHFOR) $(OPENMPFOR) $(LFLAGS)
	$(CC) -o $(OUT_PARALLEL12) $(OPENMP12) $(LFLAGS)
	$(CC) -o $(OUT_SERIAL) $(SERIAL) $(LFLAGS)

run_serial:
	@echo "======================================================"
	./$(OUT_SERIAL)
	@echo "======================================================"

run_Parallel:
	@echo "======================================================"
	./$(OUT_PARALLEL)
	@echo "======================================================"
	
run_ParallelFor:
	@echo "======================================================"
	./$(OUT_PARALLELWITHFOR)
	@echo "======================================================"

run_Parallel12:
	@echo "======================================================"
	./$(OUT_PARALLEL12)
	@echo "======================================================"
