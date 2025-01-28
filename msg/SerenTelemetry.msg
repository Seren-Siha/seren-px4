# Telemetry data including vehicle status, target info, and GPS time
uint64 timestamp
float32 latitude           # Latitude of the UAV (degrees)
float32 longitude          # Longitude of the UAV (degrees)
float32 altitude           # Altitude of the UAV (meters)
float32 pitch              # UAV pitch angle (degrees)
float32 yaw                # UAV yaw angle (degrees)
float32 roll               # UAV roll angle (degrees)
float32 speed              # UAV speed (m/s)

uint8 autonomous           # Autonomous status (1 = Yes, 0 = No)
uint8 locked               # Lock status (1 = Locked, 0 = Not Locked)

uint16 target_x            # Target center X in image (pixels)
uint16 target_y            # Target center Y in image (pixels)
uint16 target_width        # Target bounding box width (pixels)
uint16 target_height       # Target bounding box height (pixels)

uint8 gps_hour             # GPS time (hours)
uint8 gps_minute           # GPS time (minutes)
uint8 gps_second           # GPS time (seconds)
uint16 gps_millisecond     # GPS time (milliseconds)
