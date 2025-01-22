import can

def main():
    bus = can.interface.Bus(channel='can0', bustype='socketcan', bitrate=500000)

    try:
        while True:
            message = bus.recv(timeout=1.0)  # Wait for a message with a 1-second timeout
            
            if message:
                if message.arbitration_id == 0x211: 
                    byte_1 = message.data[1]  # Extract byte 0
                    print(f"Received byte 0: {byte_1}")
                    
                    if byte_1 == 0x00:
                        print("Control mode: standby")
                    elif byte_1 == 0x01:
                        print("Control mode: control")
                    elif byte_1 == 0x03:
                        print("Control mode: remote")
                    else:
                        print(f"Unknown system status: {byte_0}")
                        

    
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        bus.shutdown()

if __name__ == "__main__":
    main()

