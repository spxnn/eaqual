
COMPILER = gcc

EAQUAL : EAQUALMain.o EAQUALArgs.o EAQUALOutput.o libEAQUAL.a 
	$(COMPILER) -lm EAQUALMain.o EAQUALArgs.o EAQUALOutput.o libEAQUAL.a ../../src/libSndFile/src/.libs/libsndfile.a -o ../../bin/EAQUAL
	
EAQUALMain.o : ../../src/EAQUALFrontEnd/EAQUALMain.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/libSndFile/src/ -I ../../src/GlobalHeaders/ ../../src/EAQUALFrontEnd/EAQUALMain.cpp
	
EAQUALArgs.o : ../../src/EAQUALFrontEnd/EAQUALArgs.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/libSndFile/src/ -I ../../src/GlobalHeaders/ ../../src/EAQUALFrontEnd/EAQUALArgs.cpp
	
EAQUALOutput.o : ../../src/EAQUALFrontEnd/EAQUALOutput.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/libSndFile/src/ -I ../../src/GlobalHeaders/ ../../src/EAQUALFrontEnd/EAQUALOutput.cpp
	
libEAQUAL.a : AvgBlock.o AvgTime.o CalcTables.o FBModel.o Fft.o FFTModel.o FileIO.o KogModel.o libEAQUAL.o MOVBlock.o PreProc.o PsychoModel.o RunAnalysis.o
	ar rcs libEAQUAL.a AvgBlock.o AvgTime.o CalcTables.o FBModel.o Fft.o FFTModel.o FileIO.o KogModel.o libEAQUAL.o MOVBlock.o PreProc.o PsychoModel.o RunAnalysis.o
	
AvgBlock.o : ../../src/libEAQUAL/AvgBlock.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/AvgBlock.cpp
	
AvgTime.o : ../../src/libEAQUAL/AvgTime.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/AvgTime.cpp
	
CalcTables.o : ../../src/libEAQUAL/CalcTables.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/CalcTables.cpp
	
FBModel.o : ../../src/libEAQUAL/FBModel.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/FBModel.cpp
	
Fft.o : ../../src/libEAQUAL/Fft.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/Fft.cpp
	
FFTModel.o : ../../src/libEAQUAL/FFTModel.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/FFTModel.cpp
	
FileIO.o : ../../src/libEAQUAL/FileIO.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/FileIO.cpp
	
KogModel.o : ../../src/libEAQUAL/KogModel.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/KogModel.cpp
	
libEAQUAL.o : ../../src/libEAQUAL/libEAQUAL.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/libEAQUAL.cpp
	
MOVBlock.o : ../../src/libEAQUAL/MOVBlock.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/MOVBlock.cpp
	
PreProc.o : ../../src/libEAQUAL/PreProc.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/PreProc.cpp
	
PsychoModel.o : ../../src/libEAQUAL/PsychoModel.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/PsychoModel.cpp
	
RunAnalysis.o : ../../src/libEAQUAL/RunAnalysis.cpp
	$(COMPILER) -c -Wall -O3 -I ../../src/libEAQUAL/ -I ../../src/GlobalHeaders/ ../../src/libEAQUAL/RunAnalysis.cpp
	
clean: 
	rm *.o *.a
