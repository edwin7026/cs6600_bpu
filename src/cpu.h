/**
 * @file cpu.h
 * @details This file contains the class declaration for a cpu that initiates memory accesses
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#ifndef CPU_H
#define CPU_H

// standard includes
#include <string>
#include <fstream>
#include <sstream>

// local includes
#include <base.h>
#include <common.h>

/**
 * @details This struct holds the info from trace file
 */
struct if_packet
{
    unsigned b_addr;
    unsigned is_taken;
    if_packet() : b_addr(0), is_taken(0) {}
};

/**
 * @details This class mimics a CPU issuing memory requests to the next memory module
 */
class cpu : base
{
    private:
        std::string _trace_file_path;
        logger _log;
    public:

        /**
         * @details Constructor that takes in path of the trace file and logger
         */
        cpu(const std::string& path, const logger& log);

        /**
         * @details Sequencer that sequences memory accesses
         */
        void sequencer();
};

#endif // CPU_H