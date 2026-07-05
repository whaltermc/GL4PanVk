#include "vk_backend.hpp"
#include <cstdlib>

namespace g2p {

void VkBackend::pin_panvk_icd(const std::string& icd_json_path) {
    // Both env vars set for compatibility across loader versions in the
    // wild on Android (different launcher forks bundle different loader
    // ages). VK_DRIVER_FILES is the modern name; VK_ICD_FILENAMES is kept
    // for loaders < 1.3.234.
    setenv("VK_DRIVER_FILES", icd_json_path.c_str(), /*overwrite=*/1);
    setenv("VK_ICD_FILENAMES", icd_json_path.c_str(), /*overwrite=*/1);

    // Belt-and-suspenders: also disable the loader's implicit layer
    // discovery so a stray validation/overlay layer someone side-installed
    // doesn't silently tank frame times.
    setenv("VK_LOADER_LAYERS_DISABLE", "~all~", /*overwrite=*/0);
}

DeviceSelectionResult VkBackend::init(/* ANativeWindow* window */) {
    DeviceSelectionResult result;
    // TODO once vulkan.h is available in the build:
    //   1. vkCreateInstance with VK_KHR_surface + VK_KHR_android_surface
    //   2. vkEnumeratePhysicalDevices
    //   3. For each: vkGetPhysicalDeviceProperties2 with
    //      VkPhysicalDeviceDriverProperties chained in, check driverID /
    //      deviceName for Mali+panvk
    //   4. vkCreateDevice with required_device_extensions()
    //   5. Create swapchain against the passed ANativeWindow
    result.found_panvk = false;
    return result;
}

static const char* kRequiredExtensions[] = {
    "VK_KHR_swapchain",
    "VK_KHR_maintenance1", // used heavily by GL-flip-Y / viewport semantics
    "VK_EXT_extended_dynamic_state", // reduces pipeline permutations for
                                       // GL's mutable blend/depth toggles --
                                       // check this is in panvk's Mesa
                                       // version before relying on it
};

const char* const* VkBackend::required_device_extensions(uint32_t* count) {
    *count = sizeof(kRequiredExtensions) / sizeof(kRequiredExtensions[0]);
    return kRequiredExtensions;
}

} // namespace g2p
