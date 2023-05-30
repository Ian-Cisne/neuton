
#include "instance.hpp"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <memory>
#include <string>
#include <limits>
#include <iterator>
#include <optional>
#include <set>

namespace neu {

    VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                                            VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                                            void * pUserData )
    {
        Logger * logger = static_cast<Logger*>(pUserData);

        if ( pCallbackData->messageIdNumber == 648835635 )
        {
            // UNASSIGNED-khronos-Validation-debug-build-warning-message
            return VK_FALSE;
        }
        if ( pCallbackData->messageIdNumber == 767975156 )
        {
            // UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
            return VK_FALSE;
        };
        
        *logger << LogLevel::debug << vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": "
            + vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
        *logger << std::string( "\t" ) << "message         = <" << pCallbackData->pMessage << ">\n";
        return VK_TRUE;
    }

    Instance::Instance(Window &window, Logger& log): window_{window}, logger_{log} {    
        
        logger_ << LogLevel::info << "Setting up instance..." << "\n";

        if (!checkLayers()) {
            throw std::runtime_error("The instance hasn't the required layers to work propertly.");
        }

        logger_ << LogLevel::success << "Required layers verified." << "\n";

        if (!checkExtensions()) {
            throw std::runtime_error("The instance hasn't the required extensions to work propertly.");
        }

        logger_ << LogLevel::success << "Required extensions verified." << "\n";


        setupInstance();

        logger_ << LogLevel::success << "Instance created." << "\n";

#if !defined( NDEBUG )

        RegisterDebugUtilsMessenger();

        logger_ << LogLevel::success << "Debug report callback registered." << "\n";
#endif
    
        pickPhysicalDevice();
        
        logger_ << LogLevel::success << "Physical device setted." << "\n";

        logger_ << LogLevel::info << "Creating surface..." << "\n";

        surface_ = window.createWindowSurface(instance_);
    
        logger_ << LogLevel::success << "Surface created." << "\n";

    }

    Instance::~Instance() {
     
    }
    
    void Instance::setupInstance() {
        logger_ << LogLevel::info << "Creating instance..." << "\n";
        vk::ApplicationInfo appInfo(
            "Neuton grapher",
            VK_MAKE_VERSION(0, 0, 1),
            "Own",
            VK_MAKE_VERSION(0, 0, 1),
            VK_API_VERSION_1_3
        );
        
        vk::InstanceCreateInfo instanceInfo({}, &appInfo, validationLayers, instanceExtensions );
        //instanceCreateInfoChain = vk::su::makeInstanceCreateInfoChain( appInfo, enabledLayers, enabledExtensions );
        instance_ = vk::raii::Instance(context_, instanceInfo, nullptr);
        
    }

    void Instance::RegisterDebugUtilsMessenger() {
        logger_ << LogLevel::info << "Registering debug utils messenger..." << "\n";

        
        vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( instance_, { {},
               vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
               vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
               &debugUtilsMessengerCallback , &logger_} );
    }

    bool Instance::checkLayers() {
        
        std::vector<vk::LayerProperties> layersProps = context_.enumerateInstanceLayerProperties();
        logger_ << LogLevel::info << "Checking required layers..." << "\n";

        return std::all_of( validationLayers.begin(),
                      validationLayers.end(),
                      [&layersProps]( char const * name )
                      {
                        return std::find_if( layersProps.begin(),
                                             layersProps.end(),
                                             [&name]( vk::LayerProperties const & property )
                                             { return strcmp( property.layerName, name ) == 0; } ) != layersProps.end();
                      } );
    }

    bool Instance::checkExtensions() {
        
        std::vector<vk::ExtensionProperties> extensionsProps = context_.enumerateInstanceExtensionProperties();
        logger_ << LogLevel::info << "Checking required extensions..." << "\n";

        std::vector<const char*> extensionsForGLFW = window_.getRequiredExtensions();
        instanceExtensions.insert( instanceExtensions.end(), extensionsForGLFW.begin(), extensionsForGLFW.end() );;

        return std::all_of( instanceExtensions.begin(),
                      instanceExtensions.end(),
                      [&extensionsProps]( char const * name ) {
                        return std::find_if( extensionsProps.begin(),
                                             extensionsProps.end(),
                                             [&name]( vk::ExtensionProperties const & property )
                                             { return strcmp( property.extensionName, name ) == 0; } ) != extensionsProps.end();
                      } );
    }
 
    void Instance::pickPhysicalDevice() {
        logger_ << LogLevel::info << "Picking physical device..." << "\n";
        auto physicalDevices = instance_.enumeratePhysicalDevices();
        //We took the first device with the required extensions we find for the moment.
        for (const auto pd: physicalDevices) {
            auto availableExtensions = pd.enumerateDeviceExtensionProperties();
            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

            for (const auto &extension : availableExtensions) {
                requiredExtensions.erase(extension.extensionName);
            }
            if (requiredExtensions.empty()) {
                physicalDevice_ = vk::raii::PhysicalDevice(physicalDevices[0]);
                break;
            }

        }
        
    }  

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> compute;
        bool isComplete() { 
            return graphics.has_value() && present.has_value() && compute.has_value();
        }
    };

    void Instance::createDevice() {
        logger_ << LogLevel::info << "Creating device...\n";

        logger_ << LogLevel::info << "Picking queue families...\n";
        
        QueueFamilyIndices indices;

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice_.getQueueFamilyProperties();

        if (queueFamilyProperties.size() >= std::numeric_limits<uint32_t>::max()) {
            throw std::runtime_error("The number of properties exceedes the limit of uint32.");
        }

        for (int i = 0; i < queueFamilyProperties.size(); i++) {
            if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics ) {
                indices.graphics = i;
            }
            if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute) {
                indices.compute = i;
            }
            if (physicalDevice_.getSurfaceSupportKHR( i, *surface_ ) ) {
                indices.present = i;   
            }
            if (indices.isComplete()) {
                logger_ << LogLevel::success << "Queue families picked succesfully.\n";
                break;
            }
        }
        if (!indices.graphics.has_value()) {
            logger_ << LogLevel::warn << "Queue families with graphics capabilities were not found.\n";
        }

        if (!indices.compute.has_value()) {
            logger_ << LogLevel::warn << "Queue families with compute capabilities were not found.\n";
        }

        if (!indices.present.has_value()) {
            logger_ << LogLevel::warn << "Queue families with present capabilities were not found.\n";
        }
        
    }   
}