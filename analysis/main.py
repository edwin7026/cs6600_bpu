import subprocess
import os
import re
import multiprocessing

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

src_path = '/home/edwin/cs6600/cs6600_bpu/NS24Z045/'

def run_bp_sim(m, n, trace):
    command = "./bpsim "
    command += f"bimodal {m} {trace}" if n == 0 else f"gshare {m} {n} {trace}"
    result = subprocess.run(command, shell=True, capture_output=True, text=True, cwd=src_path)

    lines = result.stdout.split('\n')

    return {
        'pred' : int(lines[3].replace('number of predictions:', '').replace(' ', '')),
        'miss' : int(lines[4].replace('number of mispredictions:', '').replace(' ', '')),
        'rate' : float(lines[5].replace('misprediction rate:', '').replace(' ', '').replace('%', '')),
    }


def generate_plot1(trace, name):
    m_vals = [7, 8, 9, 10, 11, 12]  # 2KB to 1MB
    mis_rate = []

    for m in m_vals:
        out  = run_bp_sim(m, 0, trace)
        mis_rate.append(out['rate'])

    plt.plot(m_vals, mis_rate, '-o')

    plt.xlabel('Parameter m')
    plt.ylabel('Misprediction rate (%)')
    plt.title(f'Bimodal Predictor: Misprediction rate vs m for {trace}')
    plt.legend()
    plt.grid(True)
    plt.savefig(name)
    plt.close()

def generate_plot2(trace, name):
    m_vals = [7, 8, 9, 10, 11, 12]  # 2KB to 1MB
    mis_rate = []

    import copy

    plt.figure(figsize=(10, 6))

    for n in range(2, m_vals[-1] + 1, 2):
        m_vals_d = copy.deepcopy(m_vals)
        mis_rate = []
        for m in m_vals:
            if n > m:
                m_vals_d.remove(m)
                continue
            out  = run_bp_sim(m, n, trace)
            mis_rate.append(out['rate'])
        plt.plot(m_vals_d, mis_rate, '-o', label=f'n={n}')

    plt.xlabel('Parameter m')
    plt.ylabel('Misprediction rate (%)')
    plt.title(f'Gshare Predictor: Misprediction rate vs m for {trace} keeping n constant')
    plt.legend()
    plt.grid(True)
    plt.savefig(name)
    plt.close()

if __name__ == '__main__':
    generate_plot1('gcc_trace.txt', 'plot1.png')
    generate_plot1('jpeg_trace.txt', 'plot2.png')
    generate_plot2('gcc_trace.txt', 'plot3.png')
    generate_plot2('jpeg_trace.txt', 'plot4.png')

    
    
    