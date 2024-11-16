/**
 * @file main.cpp
 * @details This file has the main function definition
 * @author Edwin Joy <edwin7026@gmail.com>
 */

// standard imports
#include <iostream>
#include <string>

// local imports
#include <cpu.h>
#include <bimodal.h>
#include <gshare.h>

#include <common.h>

int main(int argc, char* argv[])
{
    // initialize logger
    logger log(verbose::INFO);

    // print out command
    std::cout << "COMMAND" << std::endl;
    for (int i=0; i < argc; i++)
        std::cout << argv[i] << " ";
    std::cout << std::endl;

    // check argument count
    if (!(argc == 4 || argc == 5)) {
        log.log(nullptr, verbose::FATAL, "Number of arguments is invalid");
        exit(1);
    }
 
    std::string trace_file_path = "";
    std::string bpu_type = argv[1];

    unsigned m_param = 0;
    unsigned n_param = 0;
    
    // bpu to be augmented with cpu
    bpu* bpu_inst;

    // get arguments
    if (bpu_type == std::string("bimodal")) {
        m_param = std::stoul(argv[2]);
        trace_file_path = argv[3];
        bpu_inst = new bimodal("BIMODAL", log, m_param);
    } 
    else if (bpu_type == std::string("gshare")) {
        m_param = std::stoul(argv[2]);
        n_param = std::stoul(argv[3]);
        trace_file_path = argv[4];
        bpu_inst = new gshare("GSHARE", log, m_param, n_param);
    } 
    else 
    {
        log.log(nullptr, verbose::FATAL, "Invalid predictor!");
        exit(2);
    }

    // initialize cpu here
    cpu cpu_inst(trace_file_path, log, bpu_inst);

    // start the sequence
    cpu_inst.sequencer();

    return 0;
}