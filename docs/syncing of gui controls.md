# MIDI-sequencer

## synchronize controller settings between GUI server and synth board

Editing of parameters (such as DSP and codec parameters) for the synth application takes place in the GUI. The GUI is currently implemented in Nodered and communicates directly only with the sequencer application. Wherever the GUI is implemented, the sequencer has the central role for ensuring that changed controller settings (e.g. slider settings) in the GUI are sent to the synth board. A first version for this task has been implemented in V32 of the sequencer app.

At the start of editing, the status of the controller settings in the GUI must correspond with the values of the corresponding parameters in the synth app. To achieve this, information on this status must be sent from the synth board to the sequencer, which then transports this to the GUI.

This all requires implementation of an output NimBLE channel in the synth app synth board (see the grayed arrow in the graph below)


![Architecture](https://github.com/goofy2k/MIDI-sequencer/blob/main/resources/architecture%20graphics%202.png)  

