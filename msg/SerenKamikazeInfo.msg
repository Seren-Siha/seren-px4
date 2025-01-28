# Kamikaze mission timing and QR code data
uint64 timestamp
uint8 start_hour        # Kamikaze start hour
uint8 start_minute      # Kamikaze start minute
uint8 start_second      # Kamikaze start second
uint16 start_millisecond# Kamikaze start millisecond

uint8 end_hour          # Kamikaze end hour
uint8 end_minute        # Kamikaze end minute
uint8 end_second        # Kamikaze end second
uint16 end_millisecond  # Kamikaze end millisecond

uint8[245] qr_text      # QR code content
