
#ifndef SEREN_ALTITUDE_HPP
#define SEREN_ALTITUDE_HPP

#include <uORB/topics/telemetry.h>
//#include <iostream>

class MavlinkStreamSerenAltitude : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink)
	{
		return new MavlinkStreamSerenAltitude(mavlink);
	}
	const char *get_name() const
	{
		return MavlinkStreamSerenAltitude::get_name_static();
	}
	static const char *get_name_static()
	{
		return "SEREN_ALTITUDE";
	}
	static uint16_t get_id_static()
	{
		return MAVLINK_MSG_ID_SEREN_ALTITUDE;
	}
	uint16_t get_id()
	{
		return get_id_static();
	}
	unsigned get_size()
	{
		return MAVLINK_MSG_ID_SEREN_ALTITUDE_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
	}

private:

	uORB::Subscription _telemetry_sub{ORB_ID::telemetry};
	// SubscriptionMultiArray subscription is needed because battery has multiple instances.
	// uORB::Subscription is used to subscribe to a single-instance topic

	/* Do not allow copying this class */
	MavlinkStreamSerenAltitude(MavlinkStreamSerenAltitude &);
	MavlinkStreamSerenAltitude &operator = (const MavlinkStreamSerenAltitude &);


protected:
	explicit MavlinkStreamSerenAltitude(Mavlink *mavlink) : MavlinkStream(mavlink)
	{}

	bool send() override
	{
		bool updated = false;

		// Loop through _battery_status_subs (subscription to array of BatteryStatus instances)

		// battery_status_s is a struct that can hold the battery object topic
		telemetry_s telem;

		// Update battery_status and publish only if the status has changed
		if (_telemetry_sub.update(&telem)) {
//			std::cout << "Telemetry: " << telem.altitude << std::endl;
			// mavlink_battery_status_demo_t is the MAVLink message object
			mavlink_seren_altitude_t telem_msg{};

			telem_msg.altitude = telem.altitude;


			//Send the message
			mavlink_msg_seren_altitude_send_struct(_mavlink->get_channel(), &telem_msg);
			updated = true;
		}


		return updated;
	}

};

#endif // SEREN_ALTITUDE_HPP
