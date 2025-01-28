#ifndef SEREN_TELEMETRY_HPP
#define SEREN_TELEMETRY_HPP

#include <uORB/topics/seren_telemetry.h>

class MavlinkStreamSerenTelemetry : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink)
	{
		return new MavlinkStreamSerenTelemetry(mavlink);
	}

	const char *get_name() const
	{
		return MavlinkStreamSerenTelemetry::get_name_static();
	}

	static const char *get_name_static()
	{
		return "SEREN_TELEMETRY";
	}

	static uint16_t get_id_static()
	{
		return MAVLINK_MSG_ID_SEREN_TELEMETRY;
	}

	uint16_t get_id()
	{
		return get_id_static();
	}

	unsigned get_size()
	{
		return MAVLINK_MSG_ID_SEREN_TELEMETRY_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
	}

private:
	// Subscription to the seren_telemetry uORB topic
	uORB::Subscription _seren_telemetry_sub{ORB_ID::seren_telemetry};

	/* Do not allow copying this class */
	MavlinkStreamSerenTelemetry(MavlinkStreamSerenTelemetry &);
	MavlinkStreamSerenTelemetry &operator = (const MavlinkStreamSerenTelemetry &);

protected:
	explicit MavlinkStreamSerenTelemetry(Mavlink *mavlink) : MavlinkStream(mavlink)
	{}

	bool send() override
	{
		bool updated = false;

		// Create a structure to hold the data from the uORB topic
		seren_telemetry_s seren_telemetry;

		// Update the seren_telemetry from the uORB subscription
		if (_seren_telemetry_sub.update(&seren_telemetry)) {
			PX4_INFO("Seren Telemetry updated");

			// Define the MAVLink message object to send
			mavlink_seren_telemetry_t telemetry_msg{};

			// Fill the MAVLink message with data
			telemetry_msg.latitude = seren_telemetry.latitude;
			telemetry_msg.longitude = seren_telemetry.longitude;
			telemetry_msg.altitude = seren_telemetry.altitude;
			telemetry_msg.pitch = seren_telemetry.pitch;
			telemetry_msg.yaw = seren_telemetry.yaw;
			telemetry_msg.roll = seren_telemetry.roll;
			telemetry_msg.speed = seren_telemetry.speed;
			telemetry_msg.autonomous = seren_telemetry.autonomous ? 1 : 0;
			telemetry_msg.locked = seren_telemetry.locked ? 1 : 0;
			telemetry_msg.target_x = seren_telemetry.target_x;
			telemetry_msg.target_y = seren_telemetry.target_y;
			telemetry_msg.target_width = seren_telemetry.target_width;
			telemetry_msg.target_height = seren_telemetry.target_height;
			telemetry_msg.gps_hour = seren_telemetry.gps_hour;
			telemetry_msg.gps_minute = seren_telemetry.gps_minute;
			telemetry_msg.gps_second = seren_telemetry.gps_second;
			telemetry_msg.gps_millisecond = seren_telemetry.gps_millisecond;

			// Send the message
			mavlink_msg_seren_telemetry_send_struct(_mavlink->get_channel(), &telemetry_msg);

			updated = true;
		}

		return updated;
	}
};

#endif // SEREN_TELEMETRY_HPP

