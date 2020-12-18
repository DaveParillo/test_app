/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/
/* async_publisher.cxx

   A publication of data of type async

   This file is derived from code automatically generated by the rtiddsgen
   command:

   rtiddsgen -language C++ -example <arch> async.idl

   Example publication of type async automatically generated by
   'rtiddsgen'. To test them follow these steps:

   (1) Compile this file and the example subscription.

   (2) Start the subscription with the command
       objs/<arch>/async_subscriber <domain_id> <sample_count>

   (3) Start the publication with the command
       objs/<arch>/async_publisher <domain_id> <sample_count>

   (4) [Optional] Specify the list of discovery initial peers and
       multicast receive addresses via an environment variable or a file
       (in the current working directory) called NDDS_DISCOVERY_PEERS.

   You can run any number of publishers and subscribers programs, and can
   add and remove them dynamically from the domain.


   Example:

       To run the example application on domain <domain_id>:

       On Unix:

       objs/<arch>/async_publisher <domain_id> o
       objs/<arch>/async_subscriber <domain_id>

       On Windows:

       objs\<arch>\async_publisher <domain_id>
       objs\<arch>\async_subscriber <domain_id>


modification history
------------ -------
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef RTI_VX653
    #include <vThreadsData.h>
#endif

#include <async_util.h>
#include <echo.h>
#include <ndds/ndds_cpp.h>

// generated sources
#include "async.h"
#include "asyncSupport.h"

extern "C" int publisher_main(int domainId, int sample_count)
{
    DDSDomainParticipant *participant = NULL;
    DDSPublisher *publisher = NULL;
    DDSTopic *topic = NULL;
    DDSDataWriter *writer = NULL;
    asyncDataWriter *async_writer = NULL;
    async *instance = NULL;
    DDS_ReturnCode_t retcode;
    DDS_InstanceHandle_t instance_handle = DDS_HANDLE_NIL;
    const char *type_name = NULL;
    int count = 0;
    struct DDS_Duration_t send_period = { 0, 100000000 };

    /* To customize participant QoS, use
       the configuration file USER_QOS_PROFILES.xml */
    participant = DDSTheParticipantFactory->create_participant(
            domainId,
            DDS_PARTICIPANT_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (participant == NULL) {
        printf("create_participant error\n");
        async_shutdown(participant);
        return -1;
    }

    /* To customize publisher QoS, use
       the configuration file USER_QOS_PROFILES.xml */
    publisher = participant->create_publisher(
            DDS_PUBLISHER_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (publisher == NULL) {
        printf("create_publisher error\n");
        async_shutdown(participant);
        return -1;
    }

    /* Register type before creating topic */
    type_name = asyncTypeSupport::get_type_name();
    retcode = asyncTypeSupport::register_type(participant, type_name);
    if (retcode != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        async_shutdown(participant);
        return -1;
    }

    /* To customize topic QoS, use
       the configuration file USER_QOS_PROFILES.xml */
    topic = participant->create_topic(
            "Example async",
            type_name,
            DDS_TOPIC_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        async_shutdown(participant);
        return -1;
    }

    /* To customize data writer QoS, use
       the configuration file USER_QOS_PROFILES.xml */
    writer = publisher->create_datawriter(
            topic,
            DDS_DATAWRITER_QOS_DEFAULT,
            NULL /* listener */,
            DDS_STATUS_MASK_NONE);
    if (writer == NULL) {
        printf("create_datawriter error\n");
        async_shutdown(participant);
        return -1;
    }

    async_writer = asyncDataWriter::narrow(writer);
    if (async_writer == NULL) {
        printf("DataWriter narrow error\n");
        async_shutdown(participant);
        return -1;
    }

    /* Create data sample for writing */

    instance = asyncTypeSupport::create_data();

    if (instance == NULL) {
        printf("asyncTypeSupport::create_data error\n");
        async_shutdown(participant);
        return -1;
    }

    /* For a data type that has a key, if the same instance is going to be
       written multiple times, initialize the key here
       and register the keyed instance prior to writing */
    /*
        instance_handle = async_writer->register_instance(*instance);
    */

    echo e;
    /* Main loop */
    for (count = 0; (sample_count == 0) || (count < sample_count); ++count) {
        printf("Writing async, count %d\n", count);

        // send count as data.
        instance->x = e.repeater(count);

        retcode = async_writer->write(*instance, instance_handle);
        if (retcode != DDS_RETCODE_OK) {
            printf("write error %d\n", retcode);
        }

        NDDSUtility::sleep(send_period);
    }

    //// Changes for Asynchronous_Publication
    // Give time for publisher to send out last few samples
    send_period.sec = 1;
    NDDSUtility::sleep(send_period);

    /*
        retcode = async_writer->unregister_instance(
            *instance, instance_handle);
        if (retcode != DDS_RETCODE_OK) {
            printf("unregister instance error %d\n", retcode);
        }
    */

    /* Delete data sample */
    retcode = asyncTypeSupport::delete_data(instance);
    if (retcode != DDS_RETCODE_OK) {
        printf("asyncTypeSupport::delete_data error %d\n", retcode);
    }

    /* Delete all entities */
    return async_shutdown(participant);
}

#if defined(RTI_WINCE)
int wmain(int argc, wchar_t **argv)
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */

    if (argc >= 2) {
        domainId = _wtoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = _wtoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
   NDDSConfigLogger::get_instance()->
       set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
                                 NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
   */

    return publisher_main(domainId, sample_count);
}

#elif !(defined(RTI_VXWORKS) && !defined(__RTP__)) && !defined(RTI_PSOS)
int main(int argc, char *argv[])
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */

    if (argc >= 2) {
        domainId = atoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
    NDDSConfigLogger::get_instance()->
        set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
                                  NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */

    return publisher_main(domainId, sample_count);
}
#endif

#ifdef RTI_VX653
const unsigned char *__ctype = *(__ctypePtrGet());

extern "C" void usrAppInit()
{
    #ifdef USER_APPL_INIT
    USER_APPL_INIT; /* for backwards compatibility */
    #endif

    /* add application specific code here */
    taskSpawn(
            "pub",
            RTI_OSAPI_THREAD_PRIORITY_NORMAL,
            0x8,
            0x150000,
            (FUNCPTR) publisher_main,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0);
}
#endif