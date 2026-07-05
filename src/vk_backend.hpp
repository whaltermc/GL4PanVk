#pragma once
#include <cstdint>
#include <string>
#include <vulkan/vulkan.h>

namespace g2p {

// This is the part that makes the project "target panvk" rather than "let
// the Vulkan loader pick whatever ICD it feels like." On a device with
// multiple installable ICDs present (vendor blob + panvk side-loaded),
// loader enumeration order isn't something you want to depend on.
struct DeviceSelectionResult {
    bool     found_panvk = false;
    std::string device_name;
    int      vulkan_api_version = 0; // VK_API_VERSION_1_x packed value
};

class VkBackend {
public:
    // Call before vkCreateInstance. Two ways to pin panvk explicitly:
    //  1. Set VK_ICD_FILENAMES (pre-1.x loaders) or VK_DRIVER_FILES
    //     (loader >= 1.3.234) to point *only* at the panvk manifest json,
    //     so the loader never even sees the vendor ICD.
    //  2. If you must coexist with other ICDs, enumerate
    //     vkEnumeratePhysicalDevices and pick the one whose
    //     VkPhysicalDeviceProperties.deviceName matches "Mali" AND whose
    //     driver is confirmed panvk (check VK_KHR_driver_properties'
    //     driverID == VK_DRIVER_ID_MESA_PANVK once that's exposed -- as of
    //     early panvk versions this may not be populated, so falling back
    //     to a deviceName substring match is the pragmatic option).
    static void pin_panvk_icd(const std::string& icd_json_path);

    DeviceSelectionResult init(/* ANativeWindow* window -- Android-specific,
                                   passed in from the launcher's surface */);

    // Required extensions to request at device-creation time given what
    // Minecraft/Sodium/Iris shaders actually need. Keep this list minimal --
    // every extension you request that panvk doesn't yet support is a
    // device-creation failure, not a soft fallback.
    static const char* const* required_device_extensions(uint32_t* count);

private:
    // Real fields once vulkan.h is in scope:
    //   VkInstance instance_;
    //   VkPhysicalDevice physical_device_;
    //   VkDevice device_;
    //   VkQueue graphics_queue_;
    //   VkCommandPool command_pool_;
    //   VkSwapchainKHR swapchain_;
};

} // namespace g2p
