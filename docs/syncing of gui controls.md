# MIDI-sequencer / synth

## Synchronize controller settings between GUI server and synth board

Editing of parameters (such as DSP and codec parameters) for the synth application takes place in the GUI. The GUI is currently implemented in Nodered and communicates directly only with the sequencer application. Wherever the GUI is implemented, the sequencer has the central role for ensuring that changed controller settings (e.g. slider settings) in the GUI are sent to the synth board. A first version for this task has been implemented in V32 of the sequencer app.

At the start of editing, the status of the controller settings in the GUI must correspond with the values of the corresponding parameters in the synth app. To achieve this, information on this status must be sent from the synth board to the sequencer, which then transports this to the GUI.

This all requires implementation of an output NimBLE channel in the synth app synth board (see the grayed arrow in the graph below).


![Architecture](https://github.com/goofy2k/MIDI-sequencer/blob/main/resources/architecture%20graphics%202.png)  


## Steps for implementation

The implementation steps are listed here. The complete implementation depends on a chain of events / handlers. 

1. Synth app: Write messages with parameter values to NimBLE characteristics in the synth app
2. Sequencer app: Receive parameter characteristics from synth in the sequencer app
3. Sequencer app: Send parameter values to Nodered
4. Nodered flow: Set controller layout to match with received value

The steps are not necessarilly implemented in the order of the list. It is probably wise to start with the tail (4.), then implement the head (1.) and then "glue" those parts together in the sequencer app (2. and 3.).   

As the sequencer already contains the NimBLE server role, the starting point is to re-use that. Currently the initiation of the server is done in a part that was added to the NicMidi lib. Because of the need for re-use of the server, it is probably wise to make NimBLE server initialization in the sequencer independent of NiCMidi. 



### 1. Synth app: Write messages with parameter values to NimBLE characteristics in the synth app

### 2. Sequencer app: Receive parameter characteristics from synth in the sequencer app

### 3. Sequencer app: Send parameter values to Nodered

For testing purposes, the sequencer will send values to the controls when an edit command incoming from the GUI is passed (via Midi Thru) to the synth board.

### 4. Nodered flow: Set controller layout to match with received value

Received values (0-127) are received on DSP CONTROL CHANGE INPUT HUB and dispatched to the respective slide controls
