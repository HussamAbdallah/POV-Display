
# POV Display

POV Display 

The software is designed using Microship Studio. First,
to display any charachter, all alphabet is stored such that
every alphabet is stored as array of size five of unsigned
char . Each element in the array represent what the rod
will draw in a certain degree. Each alphabet is divided
into 5 columns where the first column will be displayed
at a chosen degree(θ) then the next column of the char
will be displayed at (θ +3) and so on until the 5 columns
of the character is finished. the following figure describes how the
character ”F” can be stored. The alphabet may be also
stored as (mxn) array, but since each element will be
one bit stored in an unsigned char(8bit), this will be an
inefficient way of using the memory.
![Character ”F” in the memory](https://i.postimg.cc/nz8jPJLH/char-ex.png)



# Full rotation duration    


when the hall effect sensor trigger the magnet an interrupt service
routine(ISR) is entered where the time is recorded then
the main loop continues, when the ISR is entered again
time difference between the current interruption and the
last interruption represents the full rotation duration, the
duration of one degree can then be measured by dividing
the full rotation duration over 360

. The main program is always checking if the desired angle is reached by calculating the time the last
time the sensor interruption occurred (0◦
reference) and
the current time in the main loop, then when the desired
angle is reached the character is displayed.


![Simple flowchart of the program](https://i.postimg.cc/mr6Y8qvj/flowchart1.png)
for further details please check the documentation section.

## Documentation

[Design and Construction of Spinning LED String Display](https://drive.google.com/file/d/1ucd9Ag6lVDAbJRt8sK39x25FbjjneYXc/view?usp=sharing)


