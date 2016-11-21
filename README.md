# Speech Processing

The included code represents a working MATLAB implementation of the proposed VAD algorithm. 
To run it, load the Test01.wav waveform into MATLAB and call the supplied VAD function on the
test data and its sampling rate of 44100 Hz.

Included as well is a first attempt at adapting the algorithm to a C++ implementation. This
piece of code still requires some debugging to get the algorithm itself working correctly, 
but it will still run on the supplied testspeech.wma file and output a similar waveform. You
can compare the two datasets, but as we mentioned, the algorithm is not yet fully working.
