//structures.h is filename 

#include <stdint.h>


typedef struct {
    uint32_t command_id;
} TC_101;

typedef struct {
    uint32_t command_id;
    float DeltaVx;
    float DeltaVy;
    float DeltaVz;
} TC_102;


typedef struct {
    uint32_t command_id;
    uint32_t operation_code;
    uint32_t payload_id;
} TC_103;

// TM 

typedef struct {
    uint32_t status_code;
    float cpu_usage;
    float memory_usage;
    float battery_level;
} TM_201;

typedef struct {
    uint32_t status_code;
    float current_altitude;
    float velocity_vector[3];
} TM_202;

typedef struct {
    uint32_t status_code;
    uint32_t payload_id;
    uint32_t operational_status;
    float data_measurements[3];
} TM_203;






