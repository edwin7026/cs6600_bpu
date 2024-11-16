/**
 * @file bpu.h
 * @details This file declares the bpu class
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#ifndef BPU_H
#define BPU_H

// standard includes
#include <string>
#include <iomanip>

// local includes
#include <base.h>
#include <common.h>
#include <cpu.h>

/**
 * @struct btb_line
 * @details This struct defines contents of an entry in branch history table
 */
struct btb_line
{
    bool _valid;
    unsigned tag;

    // default constructor 
    btb_line() : _valid(0), tag(0) {}
};

/**
 * @class bpu
 * @details abstract class with pure virtual functions for all other child classes
 */
class bpu : public base
{
    protected:
        // data members
        logger _log;

        // performance counters
        unsigned _num_predictions;
        unsigned _num_mispredictions;

        // function declararations
        virtual bool predict(unsigned pc) = 0;
        virtual void update(unsigned pc, bool is_taken) = 0;
        virtual void print_table() = 0;

        /**
         * @details increment number of predictions counter
         */
        void inc_num_predictions() {
            _num_predictions++;
        }

        /**
         * @details increment misprediction counter
         */
        void inc_num_mispredictions() {
            _num_mispredictions++;
        }

        /**
         * @details function to print stats
         */
        void print_stats() {
            std::cout << "number of predictions: " << _num_predictions << std::endl;
            std::cout << "number of mispredictions: " << _num_mispredictions << std::endl;
            std::cout << "misprediction rate: " << std::setprecision(16) << (_num_mispredictions / (float)_num_predictions) * 100 << "%" << std::endl;
        }
    public:
        // constructor
        bpu(std::string name, const logger& log) : base(name) {
            _log = log;

            // initialize counters
            _num_predictions = 0;
            _num_mispredictions = 0;

        };
        
        // allow cpu to trigger bpu functions
        friend class cpu;
};

#endif // BPU_H