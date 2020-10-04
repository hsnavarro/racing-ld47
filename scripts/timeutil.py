import sys
from datetime import datetime

if len(sys.argv) == 1:
    print(datetime.now().timestamp())
else:
    start_time = datetime.fromtimestamp(float(sys.argv[1]))
    delta_time = datetime.now() - start_time
    milli = int(delta_time.total_seconds() * 1000) / 1000.0
    print("%.3f" % milli, "s", sep = '')
