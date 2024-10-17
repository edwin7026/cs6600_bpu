/**
 * @file gshare.h
 * @details This file implements gshare branch predictor
 * @author Edwin Joy <edwin7026@gmail.com>
 */

#ifndef GSHARE_H
#define GSHARE_H

// standard includes
#include <vector>
#include <cmath>

// local includes
#include <bpu.h>

class gshare : public bpu
{
    private:
        unsigned _param_m;
        unsigned _param_n;
        unsigned _num_entries;
        
        unsigned _gbhr_mask;
        unsigned _global_bh_reg;
        std::vector<unsigned> _global_bh_table;

    protected:
        /**
         * @details Gives the prediction
         * @arg pc The pc of the branch to be predicted
         * @return bool True if taken, else not taken
         */
        virtual bool predict(unsigned pc)
        {
            // increment number of predictions
            inc_num_predictions();
            
            unsigned index = (pc >> 2) % _num_entries;
            index = index ^ (_global_bh_reg << (_param_m - _param_n));

            if (_global_bh_table[index] > 1) {
                _log.log(this, verbose::DEBUG, "Index: " + std::to_string(index) + " Predicting branch is taken");
                return true;
            }
            else {
                _log.log(this, verbose::DEBUG, "Index: " + std::to_string(index) + " Predicting branch is not taken");
                return false;
            }
        }

        /**
         * @details Update the GBHR and global table
         */
        virtual void update(unsigned pc, bool is_taken)
        {
            // update gshare table
            unsigned index = (pc >> 2) % _num_entries;
            index = index ^ (_global_bh_reg << (_param_m - _param_n));

            if (is_taken) {
                // increment counter if taken
                if (_global_bh_table[index] < 3) {
                    _global_bh_table[index]++;
                }
            }
            else {
                // decrement counter if not taken
                if (_global_bh_table[index] > 0) {
                    _global_bh_table[index]--;
                }
            }

            // update gbhr
            if (is_taken) {
                _global_bh_reg = (_global_bh_reg >> 1) | _gbhr_mask;
            } else {
                _global_bh_reg = _global_bh_reg >> 1;
            }
        }

        /**
         * @details Pretty print the counters for every index
         */
        virtual void print_table()
        {
            unsigned index = 0;
            for(auto& count : _global_bh_table){
                std::cout << index << "\t" << count << std::endl;
                index++;
            }
        }

    public:
        gshare(const std::string& name, const logger& log, 
            unsigned param_m, unsigned param_n) : 
                bpu(name, log),
                _param_m(param_m),
                _param_n(param_n),
                _num_entries(static_cast<unsigned>(pow(2, param_m))),
                _global_bh_reg(0),
                _gbhr_mask(_param_n == 0 ? 0 : 1<<(_param_n-1)),
                _global_bh_table(_num_entries, 2) {}
};

#endif // GSHARE_H