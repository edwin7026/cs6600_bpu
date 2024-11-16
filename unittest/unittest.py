import os
import subprocess

src_path = '/home/edwin/cs6600/cs6600_bpu/NS24Z045/'
ref_sample_path = '/home/edwin/cs6600/cs6600_bpu/ref/sample_outputs/'
ref_val_path = '/home/edwin/cs6600/cs6600_bpu/ref/val_outputs/'

def run_test(cmd : str, ref : str):
    # run and compare
    result = subprocess.run(f'cd {src_path} && ' + cmd + f' > log.txt && diff -iw log.txt {ref}', capture_output=True, shell=True)
    print(result.stdout.rstrip())

def run(ref_path : str):
    
    file_list = os.listdir(ref_path)

    for file in file_list:
        print('For file:', ref_path + file)
        with open(ref_path + file, 'r') as fp:
            lines = fp.readlines()
            cmd = lines[1].rstrip()
            run_test(cmd, ref_path + file)


if __name__ == '__main__':
    run(ref_sample_path)
    run(ref_val_path)