import can

can_interface = 'can0'  
bus = can.Bus(channel=can_interface, interface='socketcan')

try:
    id = 0x111 
    linear_speed = -100  # Linear speed in mm/s 
    spin_speed = 0     # Spin speed in 0.001 rad/s
    reserved = 0       # Reserved byte

    # Convert linear speed and spin speed to bytes
    linear_speed_bytes = linear_speed.to_bytes(2, byteorder='big', signed=True)
    spin_speed_bytes = spin_speed.to_bytes(2, byteorder='big', signed=True)

    # Create the data payload
    data = [
        linear_speed_bytes[0], linear_speed_bytes[1],  # High and low bytes of linear speed
        spin_speed_bytes[0], spin_speed_bytes[1],      # High and low bytes of spin speed
        reserved, 0, 0, 0                              # Remaining reserved bytes
    ]

    msg = can.Message(arbitration_id=id, data=data, is_extended_id=False)
    bus.send(msg)
    print(f"Message sent on {can_interface}: {msg}")

except can.CanError as e:
    print(f"Failed to send message: {e}")

finally:
    bus.shutdown()

