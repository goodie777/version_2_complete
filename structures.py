from ctypes import *

""" This class defines a C-like struct for a TC(101) """
class TC_101(Structure):
    _fields_ = [("command_id", c_uint32)]

class TC_102(Structure):
   _fields_ = [
    ("command_id", c_uint32),
    ("DeltaVx", c_float),
    ("DeltaVy", c_float),
    ("DeltaVz", c_float)
    ]

class TC_103(Structure):
    _fields_ = [
    ("command_id", c_uint32),
    ("operation_code", c_uint32),
    ("payload_id", c_uint32),
    ]

# Telemetry Messages (TM). System Health Status 

class TM_201(Structure):
    _fields_ = [
        ("status_code", c_uint32),
        ("cpu_usage", c_float),
        ("memory_usage", c_float),
        ("battery_level", c_float)
    ]

class TM_202(Structure):
    _fields_ = [
        ("status_code", c_uint32),
        ("current_altitude", c_float ),
        ("velocity_vector", c_float * 3)
    ]

class TM_203(Structure):
    _fields_ = [
        ("status_code", c_uint32),
        ("payload_id", c_uint32 ),
        ("operational_status", c_uint32),
        ("data_measurements", c_float * 3)
    ]


