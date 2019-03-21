#! /bin/bash

#SBATCH -o results3.out  # job output
#SBATCH -e errors.log # job error output

#SBATCH -N 1  # number of nodes
#SBATCH -n 1  # number of cores (AKA tasks)
#SBATCH -c 1  # number of cpus per task

#SBATCH --mem 10G
#SBATCH -t 0-02:00  # two hour time limit

cmake3 .
make

./multi_threads 1 ./data/hpc_1m.csv
./multi_threads 2 ./data/hpc_1m.csv
./multi_threads 4 ./data/hpc_1m.csv
./multi_threads 6 ./data/hpc_1m.csv
./multi_threads 8 ./data/hpc_1m.csv

./multi_threads 1 ./data/hpc_2m.csv
./multi_threads 2 ./data/hpc_2m.csv
./multi_threads 4 ./data/hpc_2m.csv
./multi_threads 6 ./data/hpc_2m.csv
./multi_threads 8 ./data/hpc_2m.csv

./multi_threads 1 ./data/hpc_3m.csv
./multi_threads 2 ./data/hpc_3m.csv
./multi_threads 4 ./data/hpc_3m.csv
./multi_threads 6 ./data/hpc_3m.csv
./multi_threads 8 ./data/hpc_3m.csv

./multi_threads 1 ./data/hpc_4m.csv
./multi_threads 2 ./data/hpc_4m.csv
./multi_threads 4 ./data/hpc_4m.csv
./multi_threads 6 ./data/hpc_4m.csv
./multi_threads 8 ./data/hpc_4m.csv

./multi_threads 1 ./data/hpc_5m.csv
./multi_threads 2 ./data/hpc_5m.csv
./multi_threads 4 ./data/hpc_5m.csv
./multi_threads 6 ./data/hpc_5m.csv
./multi_threads 8 ./data/hpc_5m.csv

./multi_threads 1 ./data/hpc_6m.csv
./multi_threads 2 ./data/hpc_6m.csv
./multi_threads 4 ./data/hpc_6m.csv
./multi_threads 6 ./data/hpc_6m.csv
./multi_threads 8 ./data/hpc_6m.csv

./multi_threads 1 ./data/hpc_7m.csv
./multi_threads 2 ./data/hpc_7m.csv
./multi_threads 4 ./data/hpc_7m.csv
./multi_threads 6 ./data/hpc_7m.csv
./multi_threads 8 ./data/hpc_7m.csv

./multi_threads 1 ./data/hpc_8m.csv
./multi_threads 2 ./data/hpc_8m.csv
./multi_threads 4 ./data/hpc_8m.csv
./multi_threads 6 ./data/hpc_8m.csv
./multi_threads 8 ./data/hpc_8m.csv

./multi_threads 1 ./data/hpc_9m.csv
./multi_threads 2 ./data/hpc_9m.csv
./multi_threads 4 ./data/hpc_9m.csv
./multi_threads 6 ./data/hpc_9m.csv
./multi_threads 8 ./data/hpc_9m.csv