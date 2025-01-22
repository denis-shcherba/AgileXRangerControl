# AgileXRangerControl

After connecting the CAN bus to the usb adapter check
ifconfig can0

if the flags flags=128<NOARP> are output, its not properly initializd

try
sudo ip link set can0 up type can bitrate 500000

with the bitrate given in the manual

afterwards the flag UP should be output
you can test listening to the CAN bus via 

candump can0