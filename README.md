# keylog-master

"klog" folder contains program to record keystrokes and its timestamp

### Goal
Through collected data, train a model that can tell what the user typed with only the timestamps of keystroke.

Ultimately, with only the recorded sound of typing, extract the timestamps, then guess the sequence of keys.

### still under development
Tried HMM and 3-layer NN, but resulted with accuracy far below expectations

=> plan to divide Korean/English typings + treat each onset, nucleus, and coda(3 components of one Korean syllable) differently

=> + deeper NN
