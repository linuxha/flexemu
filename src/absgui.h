/*
    absgui.h: abstract gui interface.

    flexemu, an MC6809 emulator running FLEX
    Copyright (C) 1997-2018  W. Schwotzer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#ifndef __absgui_h__
#define __absgui_h__

#include "misc1.h"
#include <stdio.h>
#include <string.h>

#include "e2.h"
#include "sguiopts.h"
#include <string>
#include <memory>

#ifdef _WIN32
    #define COPYTEXT "press Licence button"
#else
    #define COPYTEXT "look at file COPYING"
#endif

#define HEADER1 "                 " PROGRAMNAME "\n\
an MC6809 emulator running FLEX\n                 "
#define HEADER2 "\n\n\
  Copyright (C) 1997-2018 W. Schwotzer\n\n\
" PROGRAMNAME " comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome\n\
    to redistribute it under certain\n\
    conditions. For more information\n\
        " COPYTEXT ".\n\n\
https://aladur.neocities.org/flexemu\n"

#define CPU_LINES   (15)
#define CPU_LINE_SIZE   (39)

class Mc6809;
class Scheduler;
class Inout;
class Memory;
class E2video;
class Mc6809CpuStatus;
class JoystickIO;
class KeyboardIO;

class AbstractGui
{

    // private instance variables:

protected:
    Mc6809 &cpu;    // pointer to cpu to send interrupts
    Memory &memory; // pointer to memory (video memory access)
    Scheduler &scheduler;
    Inout &inout; // pointer to IO-class handling input/output
    E2video &e2video;// pointer to video control registers
    JoystickIO &joystickIO; // joystick data provider.
    KeyboardIO &keyboardIO; // keyboard data provider.
    const char *program_name;
    unsigned char unused_block[YBLOCK_SIZE];
    int switch_sp;
    struct sGuiOptions &options;
    bool exit_flag; // exit application:
    char cpustring[CPU_LINES * (CPU_LINE_SIZE + 1)];
    Byte bp_input[2];
    std::string color;
    int pixelSizeX; // x-size of one pixel on the screen
    int pixelSizeY; // y-size of one pixel on the screen
    int nColors; // Number of colors or gray scale values { 2, 8, 64 }
    bool withColorScale; // true: use color scale
                     // false: use one color in different shades
    int timebase;
    int cpu_line_size;
    const char *cpu_line_delim;
    unsigned long pen[1 << COLOR_PLANES];
    Word conv_2byte_tab[256];
    DWord conv_3byte_tab[256];
    DWord conv_4byte_tab[256];

    // Initialisation functions

protected:

    virtual void    update_block(int block_number); // update one video blk
    virtual void    initialize(struct sGuiOptions &options);
    virtual void    initialize_conv_tables();
    void    CopyToZPixmap(int block_number,
                          Byte *dest, const Byte *src,
                          int depth, const unsigned long *pen);
    virtual void    clear_cpuview();
    virtual void    redraw_cpuview(const Mc6809CpuStatus &stat);
    virtual void    redraw_cpuview_contents(const Mc6809CpuStatus &stat);
    virtual void    redraw_cpuview_impl(const Mc6809CpuStatus &stat);
    virtual void    text(int x, int y, const char *str, int rev = 0);
    virtual void   set_line_delim(const char *delim);
    // public interface

public:
    virtual void    update_cpuview(const Mc6809CpuStatus &stat);
    // update cpu view
    virtual void    set_exit(bool b = true);    // set exit flag
    virtual void    update();           // update video and
    // event loop
    virtual void    set_new_state(Byte user_input); // set cpu to new state
    virtual void    set_bell(int x_percent);    // give a short
    // acoustic signal
    virtual void    output_to_terminal();   // set output to terminal
    virtual void    output_to_graphic();    // set output to gui
    virtual void    main_loop();        // enter the msg loop
    virtual int gui_type();         // return type of gui

    // constructor and destructor

public:
    AbstractGui(
        Mc6809 &x_cpu,
        Memory &x_memory,
        Scheduler &x_scheduler,
        Inout &x_inout,
        E2video &x_video,
        JoystickIO &x_joystickIO,
        KeyboardIO &x_keyboardIO,
        struct sGuiOptions &options);
    virtual ~AbstractGui();
};

#endif // __absgui_h__

