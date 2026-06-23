import struct
import socket
from structures import *


def prints():
    data = TC_101(101)

    print(data.command_id)


    data2 = TC_102(102, 2.5,7.4,8.7)

    print(data2.command_id)
    print(data2.DeltaVx)
    print(data2.DeltaVy)
    print(data2.DeltaVz)


    data3 = TC_103(7,16,58)

    print(data3.command_id)
    print(data3.operation_code)
    print(data3.payload_id)

    # TM 

    TM_data1 = TM_201(201,7.5,8.9,10.0 )

    print(TM_data1.status_code)
    print(TM_data1.cpu_usage)
    print(TM_data1.memory_usage )
    print(TM_data1.battery_level)

    TM_data2 = TM_202(202,100.0,(c_float * 3)(9.8, 7.7, 5.4))

    print(TM_data2.status_code)
    print(TM_data2.current_altitude)
    print(list(TM_data2.velocity_vector))


    TM_data3 = TM_203(203,200,300, (c_float * 3)(7.7, 9.9, 11.7))

    print(TM_data3.status_code )
    print(TM_data3.payload_id)
    print(TM_data3.operational_status)
    print(list(TM_data3.data_measurements)) 

def main():
    # define our TC data
    data = TC_101(101)
    data2 = TC_102(102, 1, 2, 3)
    

    # pack the data into the byte stream
    packet = bytes(data)
    packet2 = bytes(data2)


    # connect to our socket 
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 5001))

    # user input send 
    while True:
        input("Press enter to send TC_101: ")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 5001))
        # s.sendall(packet)
        s.sendall(packet2)
        data = s.recv(sizeof(TM_201))
        tm = TM_201.from_buffer_copy(data)
        print("status_code:", tm.status_code)
        print("cpu_usage:", tm.cpu_usage)
        print("memory_usage:", tm.memory_usage)
        print("battery_level:", tm.battery_level)
        s.close()

if __name__ == "__main__":
    main()
















