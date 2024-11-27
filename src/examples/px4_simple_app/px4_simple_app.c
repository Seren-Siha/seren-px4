#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <drivers/drv_hrt.h>
#include <uORB/uORB.h>
#include <uORB/topics/target_locking_status.h>
#include <pthread.h>
#include <string.h>

pthread_t publishing_thread;  // Thread handle for publishing
bool is_running = false;      // Flag to control the publishing thread

// Thread function for publishing
void* target_locking_publisher_thread(void* arg)
{
    // Create topic structure
    struct target_locking_status_s target_locking_status;
    memset(&target_locking_status, 0, sizeof(target_locking_status));

    // Advertise the topic
    orb_advert_t target_locking_status_pub = orb_advertise(ORB_ID(target_locking_status), &target_locking_status);

    int count = 0;
    while (true) {
        if (!is_running) {  // Check if the thread should stop
            PX4_INFO("Publishing thread stopping.");
            break;  // Stop the thread if is_running is false
        }

        // Update timestamp and alternate is_locked status
        target_locking_status.timestamp = hrt_absolute_time();
        target_locking_status.is_locked = (count % 2 == 0);

        // Publish the topic
        orb_publish(ORB_ID(target_locking_status), target_locking_status_pub, &target_locking_status);

        // Small delay (1000ms)
        px4_usleep(1000000);

        count++;
    }

    PX4_INFO("Publishing thread has stopped.");
    return NULL;
}

__EXPORT int px4_simple_app_main(int argc, char *argv[])
{
    if (argc < 2) {
        PX4_ERR("Usage: px4_simple_app <start|stop>");
        return 1;
    }

    // Parse command line arguments
    if (strcmp(argv[1], "start") == 0) {
        if (is_running) {
            PX4_INFO("Publishing thread is already running.");
            return 0;
        }

        is_running = true;  // Start the publishing thread

        // Initialize and create the publishing thread
        int ret = pthread_create(&publishing_thread, NULL, target_locking_publisher_thread, NULL);
        if (ret != 0) {
            PX4_ERR("Failed to create publishing thread: %d", ret);
            return 1;
        }

        PX4_INFO("Publishing thread started.");
    }
    else if (strcmp(argv[1], "stop") == 0) {
        if (!is_running) {
            PX4_INFO("Publishing thread is not running.");
            return 0;
        }

        is_running = false;  // Stop the publishing thread

        // Wait for the publishing thread to finish
        int ret = pthread_join(publishing_thread, NULL);
        if (ret != 0) {
            PX4_ERR("Failed to join publishing thread: %d", ret);
            return 1;
        }

        PX4_INFO("Publishing thread stopped.");
    }
    else {
        PX4_ERR("Invalid argument. Usage: px4_simple_app <start|stop>");
        return 1;
    }

    return 0;
}
