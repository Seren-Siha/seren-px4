#ifndef SEREN_LOCK_INFO_HPP
#define SEREN_LOCK_INFO_HPP

#include <uORB/topics/seren_lock_info.h>

class MavlinkStreamSerenLockInfo : public MavlinkStream
{
public:
    static MavlinkStream *new_instance(Mavlink *mavlink)
    {
        return new MavlinkStreamSerenLockInfo(mavlink);
    }

    const char *get_name() const
    {
        return MavlinkStreamSerenLockInfo::get_name_static();
    }

    static const char *get_name_static()
    {
        return "SEREN_LOCK_INFO";
    }

    static uint16_t get_id_static()
    {
        return MAVLINK_MSG_ID_SEREN_LOCK_INFO;
    }

    uint16_t get_id()
    {
        return get_id_static();
    }

    unsigned get_size()
    {
        return MAVLINK_MSG_ID_SEREN_LOCK_INFO_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
    }

private:
    // Subscription to the Seren Lock Info uORB topic
    uORB::Subscription _seren_lock_info_sub{ORB_ID::seren_lock_info};

    /* Do not allow copying this class */
    MavlinkStreamSerenLockInfo(MavlinkStreamSerenLockInfo &) = delete;
    MavlinkStreamSerenLockInfo &operator = (const MavlinkStreamSerenLockInfo &) = delete;

protected:
    explicit MavlinkStreamSerenLockInfo(Mavlink *mavlink) : MavlinkStream(mavlink)
    {}

    bool send() override
    {
        bool updated = false;

        // Create a structure to hold the data from the uORB topic
        seren_lock_info_s seren_lock_info;

        // Update the seren_lock_info from the uORB subscription
        if (_seren_lock_info_sub.update(&seren_lock_info)) {
            PX4_INFO("Seren Lock Info updated: Start time: %02d:%02d:%02d:%03d, End time: %02d:%02d:%02d:%03d, Auto lock: %d",
                     seren_lock_info.start_hour, seren_lock_info.start_min,
                     seren_lock_info.start_sec, seren_lock_info.start_ms,
                     seren_lock_info.end_hour, seren_lock_info.end_min,
                     seren_lock_info.end_sec, seren_lock_info.end_ms,
                     seren_lock_info.auto_lock);

            // Define the MAVLink message object to send
            mavlink_seren_lock_info_t lock_info_msg{};

            // Fill the MAVLink message with data
            lock_info_msg.start_hour = seren_lock_info.start_hour;
            lock_info_msg.start_min = seren_lock_info.start_min;
            lock_info_msg.start_sec = seren_lock_info.start_sec;
            lock_info_msg.start_ms = seren_lock_info.start_ms;

            lock_info_msg.end_hour = seren_lock_info.end_hour;
            lock_info_msg.end_min = seren_lock_info.end_min;
            lock_info_msg.end_sec = seren_lock_info.end_sec;
            lock_info_msg.end_ms = seren_lock_info.end_ms;

            lock_info_msg.auto_lock = seren_lock_info.auto_lock;

            // Send the message
            mavlink_msg_seren_lock_info_send_struct(_mavlink->get_channel(), &lock_info_msg);

            updated = true;
        }

        return updated;
    }
};

#endif // SEREN_LOCK_INFO_HPP

