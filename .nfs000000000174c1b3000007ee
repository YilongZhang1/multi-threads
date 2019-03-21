Data Files:

/data/hpc_1m.csv
/data/hpc_2m.csv
/data/hpc_3m.csv
/data/hpc_4m.csv
/data/hpc_5m.csv
/data/hpc_6m.csv
/data/hpc_7m.csv
/data/hpc_8m.csv
/data/hpc_9m.csv


1. to create the executable, run cmake3 and make in command line.
2. then call the executable like ./multi_threads 4 /data/hpc_3m.csv
3. to run all the cases just run "sbatch analysis.sh" in command line.
4. timing results for all cases will be stored in results.out file after running completion.
5. I found running the executable directly in command line and srun resulted in different time statistics. It seems timings in srun are cost by total threads rather than the parallel timings. 
6. some timing statistics are shown below.

time in segmenting data into buckets:
     process count        file size(in million lines)      time (s)
------------------------------------------------------------------------     
           1                        1                      0.301227 
           2                        1                      0.2886
           4                        1                      0.201929 
           6                        1                      0.201361 
           8                        1                      0.206796 
           1                        2                      0.60074                     
           2                        2                      0.582599  
           4                        2                      0.394187 
           6                        2                      0.377774 
           8                        2                      0.37067 
           1                        3                      0.779396
           2                        3                      0.663621
           4                        3                      0.603966
           6                        3                      0.574138 
           8                        3                      0.570167 
           1                        4                      1.0426 
           2                        4                      0.972256 
           4                        4                      0.822314
           6                        4                      0.793247 
           8                        4                      0.764718 
           1                        5                      1.29645    
           2                        5                      1.19868 
           4                        5                      1.1821
           6                        5                      0.938141 
           8                        5                      0.908881  
           1                        6                      1.55212 
           2                        6                      1.52875 
           4                        6                      1.48764
           6                        6                      1.14027 
           8                        6                      1.09905 
           1                        7                      2.02613 
           2                        7                      1.69001  
           4                        7                      1.54589
           6                        7                      1.47328
           8                        7                      1.45951 
           1                        8                      2.20577 
           2                        8                      2.05876
           4                        8                      1.84068 
           6                        8                      1.74308 
           8                        8                      1.50419
           1                        9                      2.51647 
           2                        9                      2.3766 
           4                        9                      2.3345 
           6                        9                      2.129 
           8                        9                      1.96769
           
time in creating distance matrices:     
     process count        file size(in million lines)      time (s)
------------------------------------------------------------------------       
           1                        1                      2.3325
           2                        1                      1.13455            
           4                        1                      0.576844
           6                        1                      0.401897
           8                        1                      0.307138
           1                        2                      2.10427 
           2                        2                      1.05104            
           4                        2                      0.540263
           6                        2                      0.37999
           8                        2                      0.295614 
           1                        3                      1.30726
           2                        3                      0.66841     
           4                        3                      0.361363
           6                        3                      0.26376
           8                        3                      0.210875
           1                        4                      2.26484
           2                        4                      1.13172
           4                        4                      0.621197
           6                        4                      0.440217
           8                        4                      0.344035
           
time in calculating averages:                                                                                                                                           process count        file size(in million lines)      time (s)
------------------------------------------------------------------------       
           1                        1                      0.0995998
           2                        1                      0.0452408            
           4                        1                      0.0251934 
           6                        1                      0.0183299
           8                        1                      0.014819
           1                        2                      0.0974827
           2                        2                      0.0411181            
           4                        2                      0.0225442
           6                        2                      0.0169037
           8                        2                      0.013141
           1                        3                      0.0485496 
           2                        3                      0.025103         
           4                        3                      0.013529
           6                        3                      0.00986781
           8                        3                      0.00739471
           1                        4                      0.0822545
           2                        4                      0.0433484            
           4                        4                      0.0227176
           6                        4                      0.0161671
           8                        4                      0.012317
           
           