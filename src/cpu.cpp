/**
 * @file cpu.cpp
 * @details This file contains definitions for cpu class
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#include "cpu.h"

cpu::cpu(const std::string &path, const logger& log_obj, bpu* bpu_inst) : base("CPU")
{
    _log = log_obj;
    _log.log(this, verbose::DEBUG, "Constructing CPU");
    // trace file path
    _trace_file_path = path;

    if (bpu_inst != nullptr) {
        _bpu_inst = bpu_inst;
    } else {
        _log.log(this, verbose::FATAL, "BPU not initialized");
        exit(1);
    }
}

void cpu::sequencer()
{
    _log.log(this, verbose::DEBUG, "Reading trace file: " + _trace_file_path);
    
    std::ifstream stream(_trace_file_path);

    // temp variables 
    std::string line;
    std::string opr;

    // line counter
    unsigned count = 1;

    // error flag
    bool err = false;

    if (stream.is_open())
    {   
        while (getline(stream, line))
        {
            
            if_packet* b_inst_packet = new if_packet;

            _log.log(this, verbose::DEBUG, "Reading line: " + line);
            count = count + 1;

            std::stringstream ss(line);

            ss >> opr;
            try {
                b_inst_packet -> b_addr = std::stoul(opr, nullptr, 16);
            }
            catch (const std::invalid_argument&) {
                _log.log(this, verbose::FATAL, _trace_file_path + ": Cannot convert address hext ot unsigned int at line " + std::to_string(count));
                err = false;
                break;
            }

            // get taken or not taken
            ss >> opr;
            if (opr == "t") {
                b_inst_packet->is_taken = true;
            }
            else if (opr == "n") {
                b_inst_packet->is_taken = false;
            }
            else 
            {
                // log an error
                _log.log(this, verbose::FATAL, _trace_file_path + ": Invalid request format at line " + std::to_string(count));
                err = true;
                break;
            }

            // push to the branch predictor here
            bool pred;

            // get the prediction
            pred = _bpu_inst->predict(b_inst_packet->b_addr);

            // if misprediction, increment counter
            if (pred != b_inst_packet->is_taken){
                _log.log(_bpu_inst, verbose::DEBUG, "Mispredicted!");
                _bpu_inst->inc_num_mispredictions();
            }
            
            // update/train the branch predictor
            _bpu_inst->update(b_inst_packet->b_addr, b_inst_packet->is_taken);

            //_bpu_inst->print_table();

            // delete the inst packet
            delete b_inst_packet;
        }
    }
    else {
        // file is not open yet
        _log.log(this, verbose::FATAL, _trace_file_path + ": Unable to find file");
    }

    // execution is done
    if (err) {
        _log.log(this, verbose::ERROR, "Ending with errors");
    } else {
        _log.log(this, verbose::DEBUG, "Execution completed!");
        
        std::cout << "OUTPUT" << std::endl;
        _bpu_inst->print_stats();

        // print out the BPU contents
        std::cout << "FINAL " << _bpu_inst->get_name() << " CONTENTS" << std::endl;
        _bpu_inst->print_table();
    }
}