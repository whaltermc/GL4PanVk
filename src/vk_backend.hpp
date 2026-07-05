#pragma once
#include <cstdint>
#include <string>
#include <vulkan/vulkan.h>

namespace g2p {

struct DeviceSelectionResult {
    bool     found_panvk = false;
        std::string device_name;
            uint32_t vulkan_api_version = 0;
            };

            class VkBackend {
            public:
                static void pin_panvk_icd(const std::string& icd_json_path);

                    // No swapchain/window yet -- this milestone only proves instance +
                        // device creation against panvk specifically. Swapchain creation needs
                            // an ANativeWindow* from the launcher's surface, which comes later.
                                DeviceSelectionResult init();

                                    void shutdown();

                                        static const char* const* required_device_extensions(uint32_t* count);

                                            VkInstance       instance() const { return instance_; }
                                                VkPhysicalDevice physical_device() const { return physical_device_; }
                                                    VkDevice         device() const { return device_; }

                                                    private:
                                                        VkInstance       instance_ = VK_NULL_HANDLE;
                                                            VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
                                                                VkDevice         device_ = VK_NULL_HANDLE;
                                                                    VkQueue          graphics_queue_ = VK_NULL_HANDLE;
                                                                        uint32_t         graphics_queue_family_ = UINT32_MAX;
                                                                        };

                                                                        } // namespace g2p