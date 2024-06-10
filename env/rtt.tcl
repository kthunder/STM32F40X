init
halt
resume
# run
rtt setup 0x20100000 1024 {SEGGER RTT}
rtt polling_interval 10
rtt server start 60000 0
rtt start
