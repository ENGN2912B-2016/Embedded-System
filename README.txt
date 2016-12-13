Brief documentation for concurrent processing code (still under development)

RUNNING THE CODE:

To run current iteration concurrent data processing, simply ensure a proper "input.txt" and "modRatio.txt" are in the current directory and call "ThreadPipeline.exe".


DEVELOPMENT NOTES:

Development on this threaded implementation of the audio processing is at a standstill while tracing erroneous behavior. The program fails roughly a third of the way through computation when run on the RPi. When compiled and run under Debug in Visual Studio, the process completes successfully 9 out of 10 times. That 10th failure is difficult to reproduce, and often fails on a different error on consecutive trials, although typically with double free/corrupted memory or nullptr. Attempts to use 'DrMemory' on Windows and 'Valgrind' on the RPi met with little success. The errors occured exclusively in the computation threads and the related stack traces did not extend all the way back up out of included libraries to our program files even for large stack report sizing options. Excessive time was spent attempting to explore and debug this memory leak with little to no progress, severely limiting development and improvement of other project features.