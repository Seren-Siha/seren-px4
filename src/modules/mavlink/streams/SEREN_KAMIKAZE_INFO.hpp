#ifndef SEREN_KAMIKAZE_INFO_HPP
#define SEREN_KAMIKAZE_INFO_HPP

#include <uORB/topics/seren_kamikaze_info.h>

class MavlinkStreamSerenKamikazeInfo : public MavlinkStream
{
public:
    static MavlinkStream *new_instance(Mavlink *mavlink)
    {
        return new MavlinkStreamSerenKamikazeInfo(mavlink);
    }

    const char *get_name() const
    {
        return MavlinkStreamSerenKamikazeInfo::get_name_static();
    }

    static const char *get_name_static()
    {
        return "SEREN_KAMIKAZE_INFO";
    }

    static uint16_t get_id_static()
    {
        return MAVLINK_MSG_ID_SEREN_KAMIKAZE_INFO;
    }

    uint16_t get_id()
    {
        return get_id_static();
    }

    unsigned get_size()
    {
        return MAVLINK_MSG_ID_SEREN_KAMIKAZE_INFO_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES;
    }

private:
    // Subscription to the Seren Kamikaze Info uORB topic
    uORB::Subscription _seren_kamikaze_info_sub{ORB_ID::seren_kamikaze_info};

    /* Do not allow copying this class */
    MavlinkStreamSerenKamikazeInfo(MavlinkStreamSerenKamikazeInfo &) = delete;
    MavlinkStreamSerenKamikazeInfo &operator = (const MavlinkStreamSerenKamikazeInfo &) = delete;

protected:
    explicit MavlinkStreamSerenKamikazeInfo(Mavlink *mavlink) : MavlinkStream(mavlink)
    {}

    bool send() override
    {
        bool updated = false;

        // Create a structure to hold the data from the uORB topic
        seren_kamikaze_info_s seren_kamikaze_info;

        // Update the seren_kamikaze_info from the uORB subscription
        if (_seren_kamikaze_info_sub.update(&seren_kamikaze_info)) {
            PX4_INFO("Seren Kamikaze Info updated: Start time: %02d:%02d:%02d:%03d, End time: %02d:%02d:%02d:%03d",
                     seren_kamikaze_info.start_hour, seren_kamikaze_info.start_minute,
                     seren_kamikaze_info.start_second, seren_kamikaze_info.start_millisecond,
                     seren_kamikaze_info.end_hour, seren_kamikaze_info.end_minute,
                     seren_kamikaze_info.end_second, seren_kamikaze_info.end_millisecond);

            // Define the MAVLink message object to send
            mavlink_seren_kamikaze_info_t kamikaze_info_msg{};

            // Fill the MAVLink message with data
            kamikaze_info_msg.start_hour = seren_kamikaze_info.start_hour;
            kamikaze_info_msg.start_minute = seren_kamikaze_info.start_minute;
            kamikaze_info_msg.start_second = seren_kamikaze_info.start_second;
            kamikaze_info_msg.start_millisecond = seren_kamikaze_info.start_millisecond;

            kamikaze_info_msg.end_hour = seren_kamikaze_info.end_hour;
            kamikaze_info_msg.end_minute = seren_kamikaze_info.end_minute;
            kamikaze_info_msg.end_second = seren_kamikaze_info.end_second;
            kamikaze_info_msg.end_millisecond = seren_kamikaze_info.end_millisecond;

            // Copy the QR text to the MAVLink message
            for (size_t i = 0; i < sizeof(seren_kamikaze_info.qr_text); i++) {
                kamikaze_info_msg.qr_text[i] = seren_kamikaze_info.qr_text[i];
            }

            // Send the message
            mavlink_msg_seren_kamikaze_info_send_struct(_mavlink->get_channel(), &kamikaze_info_msg);

            updated = true;
        }

        return updated;
    }
};

#endif // SEREN_KAMIKAZE_INFO_HPP

