# flamp!
flamp is an audio-responsive led light object created by Floating Point Art Collective (Jack Kalish and Mark Kleback).

## Dependencies
Current version compiles on OpenFrameworks 0.9.8

###Required Addons
- ofxWatchdog: for keeping the app alive
- ofxFX: for GPU effects
- ofxArtnet: for sending data to the LED controller

## AlphaPix Setup
1. Connect the AlphaPix to your computer via an ethernet cable. (The AlphaPix has a default network IP address of 192.168.0.50)
2. Configure your ethernet to use a unique static IP address (ie 192.168.0.1)
3. Make sure your computer’s wifi is turned off
4. That's it, now just run the software!

### Program settings:
AlphaPix set to:
192.168.0.50

DMX Signal Protocol Config - ArtNet
Pixel Chip Color Order - GRB

##TODO
change color based on sound frequency using FFT
