/*
    command.cpp


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


#include "misc1.h"
#ifdef ultrix
    #include <strings.h>
#endif
#include <ctype.h>
#include <new>

#include "e2.h"
#include "command.h"
#include "typedefs.h"
#include "absgui.h"
#include "flexerr.h"
#include "e2floppy.h"
#include "inout.h"
#include "schedule.h"
#include "mc6809.h"
#include <sstream>
#include <iomanip>


Command::Command(
        Inout &x_inout,
        Mc6809 &x_cpu,
        Scheduler &x_scheduler,
        E2floppy &x_fdc) :
    cpu(x_cpu), inout(x_inout), scheduler(x_scheduler), fdc(x_fdc),
    command_index(0), answer_index(0)
{
    memset(command, 0, sizeof(command));

    err[UNKNOWN]        = "Unknown command";
    err[PARAM]      = "Parameter invalid";
    err[PATH]       = "Nonexistent path";
    err[UNABLE_MOUNT]   = "Unable to mount drive";
    err[UNABLE_UMOUNT]  = "Unable to umount drive";
    err[UNABLE_UPDATE]  = "Unable to update drive. There are open files";
    err[CANT_CHANGE_GRAPHIC] = "Unable to change to graphic mode";
    err[UNABLE_FORMAT]  = "Unable to format disk";
    err[MEMORY_ERROR]   = "Not enough memory to execute";
}

Command::~Command()
{
}

void Command::resetIo()
{
    command_index = 0;
    answer_index  = 0;
    answer.clear();
}

Byte Command::readIo(Word /*offset*/)
{

    if (!answer.empty())
    {
        const Byte character = answer[answer_index++];

        if (character == '\0')
        {
            answer.clear();
            answer_index = 0;
        }
        else if (character == '\n')
        {
            return CR;
        }
        else
        {
            return character;
        }
    }

    return 0x00;
}

const char *Command::next_token(char **pp, int *pcount)
{
    while (*pp != nullptr && **pp != '\0' && **pp == ' ')
    {
        (*pp)++;
    }

    if (*pp != nullptr && **pp != '\0')
    {
        (*pcount)++;
    }

    return *pp;
}


void Command::skip_token(char **pp)
{
    while (*pp != nullptr && **pp != '\0' && **pp != ' ')
    {
        (*pp)++;
    }

    if (*pp != nullptr && **pp == ' ')
    {
        **pp = '\0';
        (*pp)++;
    }
}

// remove drive id and file extension
// from command token
const char *Command::modify_command_token(char *p)
{
    char *p1 = p;
    int i = 0;

    if (p1 == nullptr)
    {
        return nullptr;
    }

    if (*p1 == '\0')
    {
        return p1;
    }

    if (isdigit(*p1) && *(p1 + 1) == '.')
    {
        p1 += 2;
    }

    do
    {
        if (*(p1 + i) == '.')
        {
            *(p1 + i) = '\0';
            break;
        }

        i++;
    }
    while (*(p1 + i) != '\0');

    return p1;
}

void Command::writeIo(Word /*offset*/, Byte val)
{
    try
    {
        answer.clear();

        if (command_index < MAX_COMMAND - 1)
        {
            command[command_index++] = val;
        }
        else
        {
            command[command_index] = val;
        }

        if (val == '\0')
        {
            char       *p;
            const char *arg1, *arg2, *arg3, *arg4;
            int         count, number;

            command_index = 0;
            answer_index  = 0;
            number        = INVALID_DRIVE;
            count         = 0;
            p             = (char *)command;
            arg1 = next_token(&p, &count);  // get arg1
            skip_token(&p);
            arg2 = next_token(&p, &count);  // get arg2
            skip_token(&p);
            arg3 = next_token(&p, &count);  // get arg3
            skip_token(&p);
            arg4 = next_token(&p, &count);  // get arg4
            skip_token(&p);
            next_token(&p, &count);

            switch (count)
            {
                case 1:
                    if (stricmp(arg1, "exit") == 0)
                    {
                        scheduler.request_new_state(CpuState::Exit);
                        return;
                    }
                    else if (stricmp(arg1, "irq")  == 0)
                    {
                        cpu.set_irq();
                        return;
                    }
                    else if (stricmp(arg1, "firq")  == 0)
                    {
                        cpu.set_firq();
                        return;
                    }
                    else if (stricmp(arg1, "nmi")  == 0)
                    {
                        cpu.set_nmi();
                        return;
                    }
                    else if (stricmp(arg1, "terminal") == 0)
                    {
                        inout.output_to_terminal();
                        return;
                    }
                    else if (stricmp(arg1, "graphic") == 0)
                    {
                        if (!inout.output_to_graphic())
                        {
                            ANSWER_ERR(CANT_CHANGE_GRAPHIC);
                        }

                        return;
                    }
                    else if (stricmp(arg1, "freq") == 0)
                    {
                        std::stringstream answer_stream;

                        answer_stream << std::fixed << std::setprecision(2)
                                      << scheduler.get_frequency() << " MHz";
                        answer = answer_stream.str();
                        return;
                    }
                    else if (stricmp(arg1, "cycles") == 0)
                    {
                        std::stringstream answer_stream;

                        answer_stream << scheduler.get_total_cycles()
                                      << " cycles";
                        answer = answer_stream.str();
                        return;
                    }
                    else if (stricmp(arg1, "info") == 0)
                    {
                        std::stringstream answer_stream;

                        for (number = 0; number <= 3; number++)
                        {
                            answer_stream << fdc.drive_info(number);
                        }

                        answer = answer_stream.str();
                        return;
                    }
                    else if (stricmp(arg1, "update") == 0)
                    {
                        if (!fdc.update_all_drives())
                        {
                            ANSWER_ERR(UNABLE_UPDATE);
                        }

                        return;
                    }

                    break;

                case 2:
                    if (stricmp(arg1, "freq") == 0)
                    {
                        float freq;

                        if ((sscanf(arg2, "%f", &freq) == 1) &&
                            freq >= 0.0)
                        {
                            scheduler.set_frequency(freq);
                        }

                        return;
                    }

                    if ((sscanf(arg2, "%d", &number) != 1) ||
                        number < 0 || number > 3)
                    {
                        ANSWER_ERR(PARAM);
                        return;
                    }

                    if (stricmp(arg1, "umount") == 0)
                    {
                        if (!fdc.umount_drive(number))
                        {
                            ANSWER_ERR(UNABLE_UMOUNT);
                        }

                        return;
                    }
                    else if (stricmp(arg1, "info") == 0)
                    {
                        answer = fdc.drive_info(number);
                        return;
                    }
                    else if (stricmp(arg1, "update") == 0)
                    {
                        if (!fdc.update_drive(number))
                        {
                            ANSWER_ERR(UNABLE_UPDATE);
                        }

                        return;
                    }

                    break;

                case 3:
                    if (stricmp(arg1, "mount") == 0)
                    {
                        if ((sscanf(arg3, "%d", &number) != 1) ||
                            number < 0 || number > 3)
                        {
                            ANSWER_ERR(PARAM);
                            return;
                        }

                        if (!fdc.mount_drive(arg2, number))
                        {
                            ANSWER_ERR(UNABLE_MOUNT);
                        }

                        return;
                    }
                    else if (stricmp(arg1, "rmount") == 0)
                    {
                        if ((sscanf(arg3, "%d", &number) != 1) ||
                            number < 0 || number > 3)
                        {
                            ANSWER_ERR(PARAM);
                            return;
                        }

                        if (!fdc.mount_drive(arg2, number, MOUNT_RAM))
                        {
                            ANSWER_ERR(UNABLE_MOUNT);
                        }

                        return;
                    }

                    break;

                case 4:
                    if (stricmp(arg1, "format") == 0)
                    {
                        int trk, sec;

                        if ((sscanf(arg3, "%d", &trk) != 1) ||
                            (sscanf(arg4, "%d", &sec) != 1) ||
                            trk < 2 || sec < 5)
                        {
                            ANSWER_ERR(PARAM);
                            return;
                        }

                        if (!fdc.format_disk(trk, sec, arg2,
                                             TYPE_DSK_CONTAINER))
                        {
                            ANSWER_ERR(UNABLE_FORMAT);
                        }

                        return;
                    }

                    break;
            } // switch

            ANSWER_ERR(UNKNOWN);
        } // if
    }
    catch (std::bad_alloc UNUSED(&e))
    {
        answer = gMemoryAllocationErrorString;
    }
}

