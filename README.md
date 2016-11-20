# Embedded-System Team
# Topic: An Embedded Platform for Real-time Speech processing

This file contains a summary of what you will find in each of the branches within the project. For ease of coding, the project has been divided to three branches- I/O handeling, Speech processing and GUI_development.

Pre-processing
    This branch currently contains project file that implements a bandpass FIR filter for noise reduction of input/output signals.

Speech Processing 
    This branch contains files with the main numerical computations for speech processing like calculation of Fast Fourier Transform         (FFT), windowing, energy in the frequency domain, dominant frequency component, and the spectral flatness measure.
    
GUI_dev
    This branch contains the project file that develops a GUI for Data Visualization.
    
-------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------
# GUI-Development

Voice_modulator.pro
    This file (the main project file) contains information about project level 
    and is used to build a single project or subproject. This also contains the
    makefiles informations for qmake.
    
Widget.h
    This file contains header information for the application.It includes
    other project specific headers and declares the Widget class.

Widget.cpp
    This is the main application source file that contains the application
    class for Widget. This file controls the GUI screen and functionality of
    the widgets on the screen.

IODevice.h
    This file contains header information for I/O recognition and declares 
    the IODevice class.

IODevice.cpp
    This source files reads data from the input device so it can be used in
    Widget Class

