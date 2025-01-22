import can

def main():
    bus = can.interface.Bus(channel='can0', bustype='socketcan', bitrate=500000)

    try:
        id_to_send = 0x421 
        data_to_send =[0x01] + [0] * 7    # Byte 0 = 0x00, remaining bytes = 0

        msg_to_send = can.Message(arbitration_id=id_to_send, data=data_to_send, is_extended_id=False)
        bus.send(msg_to_send)
        print(f"Message sent: {msg_to_send}")

        while True:
            
            message = bus.recv(timeout=1.0)  # Wait for a message with a 1-second timeout

            if message.arbitration_id == 0x211: 

                
                if message:
                    byte_1 = message.data[1] 
                    print(f"Received byte 1: {byte_1}") # if 1, control mode is active
    
    except KeyboardInterrupt:
        print("Exiting...")
    except can.CanError as e:
        print(f"Failed to send message: {e}")
    finally:
        bus.shutdown()

if __name__ == "__main__":
    main()
