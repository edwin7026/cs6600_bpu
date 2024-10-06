/**
 * @file main.cpp
 * @details This file has the main function definition
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#include <iostream>
#include <string>

#include <cpu.h>
#include <common.h>

int main(int argc, char* argv[])
{
    // initialize logger
    logger log(verbose::DEBUG);

    // check argument count
    if (!(argc == 4 || argc == 5)) {
        log.log(nullptr, verbose::FATAL, "Number of arguments is invalid");
        exit(1);
    }
 
    std::string trace_file_path = "";
    std::string bpu_type = argv[1];

    unsigned m_param = 0;
    unsigned n_param = 0;

    // get arguments
    if (bpu_type == std::string("bimodal")) {
        n_param = 0;
        m_param = std::stoul(argv[2]);
        trace_file_path = argv[3];
    } 
    else if (bpu_type == std::string("gshare")) {
        m_param = std::stoul(argv[2]);
        n_param = std::stoul(argv[3]);
        trace_file_path = argv[3];
    } 
    else 
    {
        log.log(nullptr, verbose::FATAL, "Invalid predictor!");
        exit(2);
    }

    // initialize cpu here
    cpu cpu_inst(trace_file_path, log);

    // start the sequence
    cpu_inst.sequencer();


    return 0;
}