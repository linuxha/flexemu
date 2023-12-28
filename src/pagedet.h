/*                                                                              
    pagedet.h


    flexemu, an MC6809 emulator running FLEX
    Copyright (C) 2023  W. Schwotzer

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

#ifndef PAGEDETECTOR_INCLUDED
#define PAGEDETECTOR_INCLUDED

#include "poverhlp.h"
#include "pagedetd.h"

extern const char * white_space;

class PageDetector
{
public:
    PageDetector(const RichLines &lines);

    bool HasLinesPerPageDetected() const;
    size_t GetLinesPerPage() const;

private:
    void EstimateLinesPerPage(PageDetectorData &data);
    void CollectData(PageDetectorData &data);
    void EstimateScore(PageDetectorData &data);
    void EmptyLinesScore(PageDetectorData &data,
                         const std::vector<uint32_t> &emptyLines);
    void FirstNonEmptyLinesScore(PageDetectorData &data,
                          const std::map<std::string, uint32_t> &nonEmptyLines);
    void NumberOnlyLinesScore(PageDetectorData &data);
    uint32_t GetTopEmptyLines(PageDetectorData &data, uint32_t page);
    uint32_t GetBottomEmptyLines(PageDetectorData &data, uint32_t page);
    template<typename T>
    static double MeanValue(const std::vector<T> &values);
    template<typename T>
    static double Variance(const std::vector<T> &values);

    void DebugPrint(PageDetectorData &data);

    bool hasLinesPerPageDetected;
    size_t linesPerPage;
    int verbose;
};

#endif
