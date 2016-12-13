README

Please follow the instructions below to run this project.
If you encounter any issues please call Olha at (503) 310-1143.

Setting Up Hadware and Software:
1. Download 'vadMod_noThread.zip' from the Speech-Processing repository to your RPi.
2. Download 'release.zip', 'build.zip', and all 'Qtplugins#.zip' folders from the GUI_dev repository.
3. Download 'concurrency.zip' from the Concurrency repository.
4. Place all downloaded zip files on your PC desktop and unzip them there.
5. If your PC has Qt creator ingore this instruction: go into each 'Qtplugins#' folder and copy it's subfolders and put these subfolders in the 'release' folder.
NOTE: if you do not do this, the GUI executable with not run!
6. Connect your PC and your RPi to the same network via ethernet ports.
7. Determine the IP address of your RPi.
8. For VNC connection between the RPi and your PC:
Login to your RPi via putty. Username: pi , Password: raspberry. Allow for a VNC connection by typing 'vncserver :1' into the command line.
On your PC, create a connection to the RPi by entering 'IP:1' into the VNC Server field, where IP is the address of your RPi.
9. Place the unzipped folder 'vadMod_noThread' on to the Desktop of your RPi.
10. Make sure your 'release' folder is on the Desktop of your PC with all the various plugin subfolders inside it.
11. Make sure you have your command prompt open on the PC as you will need to be running rsync.



Running:
1. Open 'audio.exe' from 'release/'.
2. You can enter a modulation ratio in the GUI and press record to record your test speech.
Note you must record for at least 5 seconds. An error message will ask you to record again if you do not. If you do not record for at least 5 seconds but press play anyway, some test data files will be plotted for illustration purposes. 
3*. Once you have finished recording you will need to sync files from your PC to your RPi. Specifically, you are syncing the folder 'syncFiles'. Therefore, in your command prompt type the command:

rsync -avz /Desktop/release/syncFiles/ pi@IP:/home/pi/Desktop/vadMod_noThread/Debug/syncFiles/

password: raspberry.

4. Once rsync has finished running, navigate to your pi and run 'vadMod_noThread.exe' from 'home/pi/Desktop/vadMod_noThread/Debug/'.
5. Once 'vadMod_noThread.exe' has finished running, navigate to the command prompt on your PC and rsync your RPi files with your PC files by using the command:

rsync -avz pi@IP:/home/pi/Desktop/vadMod_noThread/Debug/syncFiles/ /Desktop/release/syncFiles/ 

password: raspberry.


6. Once the rsync has finished running you can go back to your open GUI and hit the play button. The GUI will plot processed outputs and play back your modulated recording.


*NOTE: for the rsync function your PC directory may be a little different, please alter accordingly to where your command promt will recognize the path.
**NOTE: our version of the RPi speech processing code with concurrency would not compile in Eclipse but compiled in Visual Studio.
If you open the unzipped Concurency folder and run the executable ThreadPipeline.exe from the Debug folder, it will run on some test code to just show it is working.