#ifndef SEREN_SPEED_HPP
#define SEREN_SPEED_HPP

#include <uORB/topics/telemetry.h>

class MavlinkStreamSerenSpeed : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink)
	{
		return new MavlinkStreamSerenSpeed(mavlink);
	}

	const char *get_name() const
	{
		return MavlinkStreamSerenSpeed::get_name_static();
	}

	static const char *get_name_static()
	{
		return "SEREN_SPEED";
	}

	static uint16_t get_id_static()
	{
		return MAVLINK_MSG_ID_SEREN_SPEED;
	}

	uint16_t get_id()
	{
		return get_id_static();
	}

	unsigned get_size()
	{
		return MAVLINK_MSG_ID_SEREN_SPEED_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
	}

private:
	uORB::Subscription _telemetry_sub{ORB_ID::telemetry};

	MavlinkStreamSerenSpeed(MavlinkStreamSerenSpeed &);
	MavlinkStreamSerenSpeed &operator=(const MavlinkStreamSerenSpeed &);

protected:
	explicit MavlinkStreamSerenSpeed(Mavlink *mavlink) : MavlinkStream(mavlink)
	{}

	bool send() override
	{
		bool updated = false;
		telemetry_s telem;

		if (_telemetry_sub.update(&telem)) {
			mavlink_seren_speed_t speed_msg{};
			speed_msg.uav_speed = telem.altitude;
			PX4_INFO("SEREN_SPEED sent: UAV Speed = %f", (double)telem.altitude);
			mavlink_msg_seren_speed_send_struct(_mavlink->get_channel(), &speed_msg);
			updated = true;
		}

		return updated;
	}
};

#endif // SEREN_SPEED_HPP
