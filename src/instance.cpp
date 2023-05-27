
#include "instance.hpp"
#include <exception>
#include <system_error>
#include <algorithm>
#include <string>
#include <system_error>

namespace neu {
    Instance::Instance(Window window, Logger& log): logger_(log) {    
        std::vector<vk::LayerProperties> instanceLayerProperties = context_.enumerateInstanceLayerProperties();
        
        if ( !checkLayers(instanceLayerProperties) )
        {
            throw std::system_error("No available layers.");
        }

        /* Enable debug callback extension */
        vk::ApplicationInfo appInfo(
            "Neuton grapher",
            VK_MAKE_VERSION(0, 0, 1),
            "Own",
            VK_MAKE_VERSION(0, 0, 1),
            VK_API_VERSION_1_3
        );
        instance_ = setupInstance( appInfo );
        
    }
    std::shared_ptr<vk::raii::Instance> Instance::setupInstance(vk::ApplicationInfo appInfo) {
        vk::InstanceCreateInfo instanceInfo({}, &appInfo, validationLayers, instanceExtensions );
        instanceCreateInfoChain = vk::su::makeInstanceCreateInfoChain( appInfo, enabledLayers, enabledExtensions );
        return std::make_unique(context_, );
    }
    bool Instance::checkLayers(std::vector<vk::LayerProperties> &layersProps) {
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
}