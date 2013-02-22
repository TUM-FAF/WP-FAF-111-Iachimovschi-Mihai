# Windows Programming
###### student gr. FAF-111: Mihai Iachimovschi

# Topic: WIN32 API programming basics
## Intro
### Purpose
* Event-driven programming (message-oriented)
* API
* Windows applications development

### Additional Purposes
* Version Control Systems (GIT)
* Programming Style Guidelines
* Debugging
* Testing

## Completed Mandatory Objectives
* Create a Windows application
* Choose Programming Style Guidelines that you'll follow
* Add 2 buttons to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text inputs to window: one with default styles, one with custom styles (size, background, text color, font family, font size)
* Add 2 text elements to window: one with default styles, one with custom styles (size, background, text color, font family, font size)

## Completed Objectives With Points
* Make elements to fit window on resize **(1 pt)**
* Make elements to interact or change other elements (1 pt for 2 different interactions) **(0-2 pt)**
* Change behavior of different window actions (at least 3).
* Write your own PSG (you can take existent one and modify it) and argue why it is better (for you) **(1 pt)**

## The application creation steps
For the development of this app I've read a lot of chapters from the recommended book and remembered almost nothing, but, clearly I've created a vision about Win32 API for myself.
During the process of development I used MSDN, WikiBooks and StackOverflow.com in order to make things happen.
My application tries to be a simple task tracker for the proof of concept.

As a user of Unix-like Operating Systems, I developed my program on Mac OS X using MinGW32 and Wine. The process of setting up this kind of environment is described on my recent [blog post](http://mishunika.blogspot.com/2013/02/cross-compilarea-si-rularea-win32-sub.html).

So, here is a little mockup of the program.

![Mockup](https://raw.github.com/TUM-FAF/WP-FAF-111-Iachimovschi-Mihai/master/Lab%231/pics/mockup.png)

1. Two buttons. The "Add Task" button has adds the text written in the textbox and has default style. The second one opens a MessageBox with the number of tasks.
2. The best thing I think I've done in this laboratory work. This text box has a html5-like grey placeholder that appears on focus lose, and didisappears at focusing on the field.
3. Readonly multiline edit input, with vertical scrollbar which keeps the tasks.
4. Text element that shows in realtime the number of lines in the previous edit input.
5. Window Title.
6. On clicking the close button, the user will be prompted if he is sure that he wants to close the window.
7. The icon is specified to be the default one.
8. There is no resize handler, the window is as is.

![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Iachimovschi-Mihai/master/Lab%231/pics/screenshot.png)

Speaking about PSG, I think it is TL;DR;... I'd better tell you all the details instead of writing them %)

## Outro
Well, I enjoyed executing this laboratory work even if I started it with like 30 hours before the deadline, after a sleepless night.

Anyway, I think that it is very nice to learn different technologies not only for the purpose of using them directly, but for creating some concepts from the architectural point of view. Overall I haven't encountered big problems and anything was quite clear explained on MSDN.

The main concepts of the event-driven programming are clear for me, so, from this point of view I also haven't issues.

All the code was written from the scratch using VIM text editor, and cross-compiled on Unix-Like OS. For the moment there are no compatibility or other windows.h problems and anything works as expected.
