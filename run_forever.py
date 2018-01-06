import os
import time

counter = 0
run_times = 0

while True:
    Re = os.system("./dht22")
    if Re == 0:
        counter = counter + 1
    time.sleep(3)
    run_times = run_times + 1
    if run_times % 50 == 0:
        outfile = open("error_testfile",'w')
        per = counter * 1.0/run_times
        outfile.write(str(per) + " ")
        outfile.close

