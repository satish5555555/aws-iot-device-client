// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

#ifndef DEVICE_CLIENT_SHAREDCRTRESOURCEMANAGER_H
#define DEVICE_CLIENT_SHAREDCRTRESOURCEMANAGER_H

#include "config/Config.h"

#include <atomic>
#include <aws/crt/Api.h>
#include <aws/iot/MqttClient.h>
#include <iostream>

namespace Aws
{
    namespace Iot
    {
        namespace DeviceClient
        {
            /**
             * \brief Utility class for managing the CRT SDK Resources
             *
             * The SharedCrtResourceManager wraps around the handle to the MQTT connection
             * and other CRT resources and handles both initialization and maintenance of the connection.
             */
            class SharedCrtResourceManager
            {
              private:
                const char *TAG = "SharedCrtResourceManager.cpp";
                const char *BINARY_NAME = "IoTDeviceClient";
                const char *BINARY_VERSION = "1.2";

                static constexpr int DEFAULT_WAIT_TIME_SECONDS = 10;
                bool initialized = false;
                std::atomic<bool> initializedAWSHttpLib{false};
                std::promise<void> connectionClosedPromise;
                std::unique_ptr<Aws::Crt::ApiHandle> apiHandle;
                std::unique_ptr<Aws::Crt::Io::EventLoopGroup> eventLoopGroup;
                std::unique_ptr<Aws::Crt::Io::DefaultHostResolver> defaultHostResolver;
                std::unique_ptr<Aws::Crt::Io::ClientBootstrap> clientBootstrap;
                std::unique_ptr<Aws::Iot::MqttClient> mqttClient;
                std::shared_ptr<Crt::Mqtt::MqttConnection> connection;
                struct aws_allocator *allocator;

                bool locateCredentials(const PlainConfig &config);
                int buildClient(const PlainConfig &config);
                void initializeAllocator();

              public:
                static const int SUCCESS = 0;
                static const int RETRY = 1;
                static const int ABORT = 2;
                bool initialize(const PlainConfig &config);
                void initializeAWSHttpLib();
                int establishConnection(const PlainConfig &config);
                std::shared_ptr<Crt::Mqtt::MqttConnection> getConnection();
                Aws::Crt::Io::EventLoopGroup *getEventLoopGroup();
                struct aws_allocator *getAllocator();
                Aws::Crt::Io::ClientBootstrap *getClientBootstrap();
                void disconnect();
            };
        } // namespace DeviceClient
    }     // namespace Iot
} // namespace Aws

#endif // DEVICE_CLIENT_SHAREDCRTRESOURCEMANAGER_H
