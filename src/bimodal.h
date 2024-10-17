/**
 * @file bimodial.h
 * @details This file defines bimodial branch predictor
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#ifndef BIMODIAL_H
#define BIMODIAL_H

// standard includes
#include <vector>
#include <cmath>
#include <iostream>

// local includes
#include <bpu.h>

class bimodal : public bpu
{
    private:
        unsigned _param_m;
        unsigned _num_entries;
        std::vector<unsigned> _pred_table;
    
    protected:
        /**
         * @details Gives the prediction based on local history
         * @arg pc The pc of the branch to be predicted
         * @return bool True if taken, else not taken
         */
        virtual bool predict(unsigned pc)
        {
            // increment number of predictions
            inc_num_predictions();
            
            uint index = (pc >> 2) % _num_entries;
            if (_pred_table[index] > 1) {
                _log.log(this, verbose::DEBUG, "Index: " + std::to_string(index) + " Predicting branch is taken");
                return true;
            }
            else {
                _log.log(this, verbose::DEBUG, "Index: " + std::to_string(index) + " Predicting branch is not taken");
                return false;
            }
        }

        /**
         * @details Updates the counters based on if taken or not
         */
        virtual void update(unsigned pc, bool is_taken)
        {
            // get index
            uint index = (pc >> 2) % _num_entries;

            _log.log(this, verbose::DEBUG, "Updating counter for index: " + std::to_string(index));

            if (is_taken) {
                // increment counter if taken
                if (_pred_table[index] < 3) {
                    _pred_table[index]++;
                }
            }
            else {
                // decrement counter if not taken
                if (_pred_table[index] > 0) {
                    _pred_table[index]--;
                }
            }
        }

        /**
         * @details Pretty print the counters for every index
         */
        virtual void print_table()
        {
            unsigned index = 0;
            for(auto& count : _pred_table){
                std::cout << index << "\t" << count << std::endl;
                index++;
            }
        }
    
    public:
        /**
         * @details constructor that initializes the branch predictor
         * it also sets all counters to weakly taken i.e. 2
         */
        bimodal(const std::string& name, const logger& log, unsigned param_m) : 
            bpu(name, log),
            _param_m(param_m),
            _num_entries(static_cast<unsigned>(pow(2, param_m))),
            _pred_table(_num_entries, 2) {}
};

#endif // BIMODIAL_H