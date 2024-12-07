# Project Template

This will be fetch from the utility program describe at <https://github.com/LorenzoPiombini/isam.db-C-language>, and will be the base for a custom project on this database system.
you will have to implement your own bussines logic and define your own keygeneration policies and so on.

## What's already there

the project template is already wired with a socket connection on a port 5555 (for developing)
to process incoming instructions on a 'strict' json format, if the format does not comply woth the rules on the custom json parser, the instruction will be dropped.
The main program also create a thread pool to manage concurrent request from the socket.

this project works based on shared library created from the software described at <https://github.com/LorenzoPiombini/isam.db-C-language>, so if you download this repo, the program will not work.
