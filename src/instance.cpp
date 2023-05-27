
#include "instance.hpp"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <memory>
#include <string>

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
        }
        *logger << LogLevel::debug;
        *logger << vk::to_string( static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity ) ) + ": "
            + vk::to_string( static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageTypes ) ) << ":\n";
        *logger << std::string( "\t" ) << "message         = <" << pCallbackData->pMessage << ">\n";
        return VK_TRUE;
    }

    Instance::Instance(Window window, Logger& log): window_{window}, logger_{log} {    

        logger_ << LogLevel::info << "Setting up instance..." << "\n";

        if (!checkLayers()) {
            throw std::runtime_error("The instance hasn't the required layers to work propertly.");
        }

        logger_ << LogLevel::success << "Required layers verified." << "\n";

        if (!checkExtensions()) {
            throw std::runtime_error("The instance hasn't the required extensions to work propertly.");
        }

        logger_ << LogLevel::success << "Required extensions verified." << "\n";
        
        if (!setupInstance()) {
            throw std::runtime_error("The instance couldn't be created.");
        }

        logger_ << LogLevel::success << "Instance created." << "\n";

#if !defined( NDEBUG )

        RegisterDebugUtilsMessenger();

        logger_ << LogLevel::success << "Debug report callback registered." << "\n";
#endif
        //for now, we took the first device we find with
        logger_ << LogLevel::info << "Finding a physical device..." << "\n";
    
        if (!pickPhysicalDevice()) {
            throw std::runtime_error("The instance couldn't be created.");
        }
        
        logger_ << LogLevel::success << "Physical device setted." << "\n";

        window.createWindowSurface(*instance_, &surface);
    }
        
    
    bool Instance::setupInstance() {
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
        instance_ = std::shared_ptr<vk::raii::Instance>(new vk::raii::Instance(context_, instanceInfo, nullptr));
        return instance_ != nullptr;
    }

    void Instance::RegisterDebugUtilsMessenger() {
        logger_ << LogLevel::info << "Registering debug utils messenger..." << "\n";

        
        vk::raii::DebugUtilsMessengerEXT debugUtilsMessenger( *instance_, { {},
               vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
               vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
               &debugUtilsMessengerCallback , &logger_} );
    }

    bool Instance::checkLayers() {
        
        std::vector<vk::LayerProperties> layersProps = context_.enumerateInstanceLayerProperties();
        logger_ << LogLevel::info << "Checking required extensions...";

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
        logger_ << LogLevel::info << "Checking required layers..." << "\n";

        return std::all_of( instanceExtensions.begin(),
                      instanceExtensions.end(),
                      [&extensionsProps]( char const * name )
                      {
                        return std::find_if( extensionsProps.begin(),
                                             extensionsProps.end(),
                                             [&name]( vk::ExtensionProperties const & property )
                                             { return strcmp( property.extensionName, name ) == 0; } ) != extensionsProps.end();
                      } );
    }
 
    bool Instance::pickPhysicalDevice() {
        logger_ << LogLevel::info << "Picking physical device..." << "\n";
        auto physicalDevices = instance_->enumeratePhysicalDevices();
        //for now we took the first device we find
        physicalDevice_ = std::shared_ptr<vk::raii::PhysicalDevice>( new vk::raii::PhysicalDevice(physicalDevices[0]));
        return physicalDevice_ != nullptr;
    }   
}