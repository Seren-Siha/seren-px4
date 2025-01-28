# Locking information for SEREN system
uint64 timestamp
uint8 start_hour         # Start time (hour)
uint8 start_min          # Start time (minute)
uint8 start_sec          # Start time (second)
uint16 start_ms          # Start time (millisecond)

uint8 end_hour           # End time (hour)
uint8 end_min            # End time (minute)
uint8 end_sec            # End time (second)
uint16 end_ms            # End time (millisecond)

uint8 auto_lock          # Autonomous lock status (1 = active, 0 = inactive)
