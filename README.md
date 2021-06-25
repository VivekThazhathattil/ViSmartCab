<p align = "left">
  <a href="https://github.com/VivekThazhathattil/vismartcab">
    <img height="400" src = "res/execution.gif">
    <img height="400" src = "res/mid_learn.gif">
  </a>
</p>


# ViSmartCab

This project is an attempt to implement OpenAI Taxi-v2 in C++. 

It uses a simple [Q-learning](https://en.wikipedia.org/wiki/Q-learning) ([RL](https://en.wikipedia.org/wiki/Reinforcement_learning)) model written from scratch in C++. For graphical output, SFML (Simple and Fast Multimedia Library) is used.

### Requirements

* C++ compiler
* [SFML library](https://www.sfml-dev.org/)

Edit the `Makefile`, if necessary, to match your specifications.

### Installation

Here are the instructions to build and run this app in any Linux system:
* Clone the repo: `git clone https://github.com/VivekThazhathattil/vismartcab`
* Navigate to the repo directory: `cd vismartcab`	
* Execute `make build` from the current directory. This will build the program.  Make sure you have SFML libraries in your system.

Simply delete the file `saveData/qTable.dat` to start a fresh training session.
You may also replace the file `saveData/qTable.dat` with another file of the same name to resume training that session.

### Features

* Press up arrow and down arrow to slow down/ speed up the simulation/training.
* Autosaves `Reward Table` so that it can be reused if a run is halted for some unforseen reason and the user wishes to continue training.
* Displays important information pertaining to the run alongside the graphical window.

### Issues

Any issues pertaining to the program may be reported via [GitHub Issues](https://github.com/VivekThazhathattil/ViSmartCab/issues). You may also contact me at vivek.thazhathattil@gmail.com for help/feedback.
