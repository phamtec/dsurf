require "service_manager"

Before('@restart') do
   ServiceManager.stop
   ServiceManager.start
end
