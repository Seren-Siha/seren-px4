
#ifndef TARGET_LOCKING_STATUS_HPP
#define TARGET_LOCKING_STATUS_HPP

#include <uORB/topics/target_locking_status.h>

class MavlinkStreamTargetLockingStatus : public MavlinkStream
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink)
	{
		return new MavlinkStreamTargetLockingStatus(mavlink);
	}

	const char *get_name() const
	{
		return MavlinkStreamTargetLockingStatus::get_name_static();
	}

	static const char *get_name_static()
	{
		return "TARGET_LOCKING_STATUS";
	}

	static uint16_t get_id_static()
	{
		return MAVLINK_MSG_ID_TARGET_LOCKING_STATUS;
	}

	uint16_t get_id()
	{
		return get_id_static();
	}

	unsigned get_size()
	{
		return MAVLINK_MSG_ID_TARGET_LOCKING_STATUS_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
	}

private:
	// Subscription to the target locking status uORB topic
	uORB::Subscription _target_locking_status_sub{ORB_ID::target_locking_status};

	/* Do not allow copying this class */
	MavlinkStreamTargetLockingStatus(MavlinkStreamTargetLockingStatus &);
	MavlinkStreamTargetLockingStatus &operator = (const MavlinkStreamTargetLockingStatus &);

protected:
	explicit MavlinkStreamTargetLockingStatus(Mavlink *mavlink) : MavlinkStream(mavlink)
	{}

	bool send() override
	{
		bool updated = false;

		// Create a structure to hold the data from the uORB topic
		target_locking_status_s target_locking_status;


		// Update the target_locking_status from the uORB subscription
		if (_target_locking_status_sub.update(&target_locking_status)) {
			PX4_INFO("Target locking status updated to: %s", target_locking_status.is_locked ? "Locked" : "Not locked");

			// Define the MAVLink message object to send
			mavlink_target_locking_status_t lock_status_msg{};
			mavlink_named_value_int_t named_value_int_msg{};

			// Fill the MAVLink message with data
			lock_status_msg.is_locked = target_locking_status.is_locked ? 1 : 0; // 1 if locked, 0 if not locked
			named_value_int_msg.value = lock_status_msg.is_locked;
			strcpy(named_value_int_msg.name, "is_locked");
			named_value_int_msg.time_boot_ms = target_locking_status.timestamp;

			// Send the message
			mavlink_msg_target_locking_status_send_struct(_mavlink->get_channel(), &lock_status_msg);
			//mavlink_msg_named_value_int_send_struct(_mavlink->get_channel(), &named_value_int_msg);

			updated = true;
		}

		return updated;
	}
};














#endif // TARGET_LOCKING_STATUS_HPP
