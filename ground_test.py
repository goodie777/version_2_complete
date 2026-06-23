#filename is ground_test.py

import socket
from structures import *

HOST = "127.0.0.1"
PORT = 5001


def main():

    while True:

        choice = input(
            "\nSend TC (1=TC_101, 2=TC_102, 3=TC_103, q=quit): "
        ).strip()

        if choice.lower() == "q":
            print("Exiting...")
            break

        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((HOST, PORT))

        except ConnectionRefusedError:
            print(f"Could not connect to satellite on {HOST}:{PORT}")
            continue

       
        # Send telecommand
       

        if choice == "1":

            print("[GROUND STATION] Sending TC_101")

            tc = TC_101(
                command_id=101
            )

            s.sendall(bytes(tc))

        elif choice == "2":

            print("[GROUND STATION] Sending TC_102")

            tc = TC_102(
                command_id=102,
                DeltaVx=1.0,
                DeltaVy=2.0,
                DeltaVz=3.0
            )

            s.sendall(bytes(tc))


        elif choice == "3":

            print("[GROUND STATION] Sending TC_103")

            tc = TC_103(
                 command_id=103,
                 operation_code =7,
                 payload_id =8
                
            )

            s.sendall(bytes(tc))

            

        else:
            print("Invalid choice")
            s.close()
            continue

        
        # Receive telemetry
    

        data = s.recv(1024)

        if not data:
            print("No telemetry received")
            s.close()
            continue

        print("[GROUND STATION] Telemetry received")

        status = int.from_bytes(
            data[0:4],
            byteorder="little"
        )

        if status == 201:

            print("[GROUND STATION] Decoding TM_201")

            tm = TM_201.from_buffer_copy(data)

            print(f"CPU Usage     : {tm.cpu_usage:.2f}%")
            print(f"Memory Usage  : {tm.memory_usage:.2f}%")
            print(f"Battery Level : {tm.battery_level:.2f}%")

        elif status == 202:

            print("[GROUND STATION] Decoding TM_202")

            tm = TM_202.from_buffer_copy(data)

            print(f"Altitude : {tm.current_altitude:.2f}")
            print(f"Velocity : {list(tm.velocity_vector)}")

        elif status == 203:

            print("[GROUND STATION] Decoding TM_203")

            tm = TM_203.from_buffer_copy(data)

            print(f"Payload ID         : {tm.payload_id}")
            print(f"Operational Status : {tm.operational_status}")
            print(f"Measurements       : {list(tm.data_measurements)}")

        else:
            print(f"Unknown telemetry type: {status}")

        s.close()


if __name__ == "__main__":
    main()