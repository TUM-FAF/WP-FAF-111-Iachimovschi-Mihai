# Windows Programming
###### student gr. FAF-111: Mihai Iachimovschi

# Topic: Advanced Form Elements. Child Windowses. Basics of Working With Keyboard.
## Intro

## Completed Mandatory Objectives
* Display a dialog box on some event (ex. on clicking some button)
* Add a system menu to your application with at least 3 items (add actions to that items)
* Add a scroll bar that will change any visible parameter of any other element (color of a text)
* Hook keyboard input. Add 2 custom events for 2 different keyboard combinations (ex. change window background on ctrl+space)

## Completed Objectives With Points
* Add a listbox and attach some events when any element is accessed (clicked) **(2 pt)**
* Add 2 scroll bars that will manage main window size or position **(1 pt)**
* Customize your application by adding an icon and using different cursor in application **(1 pt)**

## The application creation steps
At this laboratory work I decided to create a Makefile script in order to compile easily the resource files in a final project from the command line.
The program of this laboratory work is based on the first one, and it is extended to fit the requirements.

So, here is a little mockup of the program.

![Mockup](https://raw.github.com/TUM-FAF/WP-FAF-111-Iachimovschi-Mihai/master/Lab%232/pics/mockup.png)

1. Custom application icon.
2. Application menu. On File Popup we have Exit Menuitem, on Data - Clearing all data, on Help - About. About calls a dialog. About and data cleaner have accelerators (shortcuts) assigned.
3. Listbox. Selecting a item makes it to appear on region 8. Double clicking an element erases it.
4. Text input for the task. The same old good placeholder.
5. Button that appends the task in the listbox.
6. Red, Green, Blue indicators. These text fields are updating in real time on scrollbars position change.
7. Scrollbars for Red, Green, Blue colors. The range is 0-255, inversed.
8. Static text. It updates on changing the selection on the listbox.
9. Task counter. Updates on real time basis.

![Result](https://raw.github.com/TUM-FAF/WP-FAF-111-Iachimovschi-Mihai/master/Lab%232/pics/screenshot.png)

## Outro
This laboratory work was a little bit more complicated than the previous one, but I liked it. The main problem was using the scrollbars, I think there are some little compatibility issues on \*nix systems. Anyway, I managed the issues, and now scrollbars work as expected.

Overall anything worked as expected, and the main problems were fixed after reading a little bit of documentation.

Makefile - that's a great thing! It really helps to make things happen. I am glad that I understood quite rapidly the concepts of the Makefile and I think I'll use gained knowledge in future.

I've done almost all tasks with bonus points, but by the fact of time constrains, I think there are a little bit space for improvements, and I'll make them ASAP. Anyway, almost all requirements are fulfilled, so the work will be submited now.

